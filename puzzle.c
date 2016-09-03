#include "puzzle.h"

void pz_init(Puzzle* pz) {
  for (int i = 0; i < pz->size * pz->size; i++) {
    pz->cells[i] = pz_create_cell(i, 0, '\0', '\0');
  }
}

Puzzle* pz_create(int size) {
  Puzzle* pz = malloc(sizeof(Puzzle));
  pz->size = size;
  pz->cells = calloc(size * size, sizeof(PuzzleCell));
  pz_init(pz);
  return pz;
}

void pz_destroy(Puzzle* pz) {
  for (int i = 0; i < pz->size * pz->size; i++) {
    pz_destroy_cell(pz->cells[i]);
  }
  free(pz);
}

PuzzlePath* pz_create_path() {
  PuzzlePath* path = malloc(sizeof(PuzzlePath));
  path->cells = ll_create();
  path->word = ll_create();
  return path;
}

PuzzlePath* pz_copy_path(PuzzlePath* path) {
  PuzzlePath* new_path = (PuzzlePath*) malloc(sizeof(PuzzlePath));
  new_path->cells = ll_copy(path->cells);
  new_path->word = ll_copy_data(path->word, copy_char);
  return new_path;
}

void pz_destroy_path(PuzzlePath* path) {
  ll_destroy(path->cells, NULL);
  ll_destroy(path->word, destroy_char);
  free(path);
}
void pz_ll_destroy_path(void* data) {
  pz_destroy_path((PuzzlePath*) data);
}

PuzzleCell* pz_create_cell(int id, int type, char c1, char c2) {
  PuzzleCell* cell = malloc(sizeof(PuzzleCell));
  cell->id = id;
  cell->type = type;
  cell->c1 = c1;
  cell->c2 = c2;
  return cell;
}

void pz_destroy_cell(PuzzleCell* cell) {
  free(cell);
}
void pz_ll_destroy_cell(void* data) {
  pz_destroy_cell((PuzzleCell*) data);
}

void pz_add_cell(Puzzle* pz, PuzzleCell* cell) {
  if (pz_valid_cell_id(pz, cell->id)) {
    if (pz->cells[cell->id] != NULL) {
      pz_destroy_cell(pz->cells[cell->id]);
    }
    pz->cells[cell->id] = cell;
  }
}

PuzzleCell* pz_get_cell(Puzzle* pz, int id) {
  if (pz_valid_cell_id(pz, id)) {
    return pz->cells[id];
  }
  return NULL;
}
PuzzleCell* pz_get_cell_xy(Puzzle* pz, int x, int y) {
  return pz_get_cell(pz, pz_xy_to_id(pz, x, y));
}

LinkedList pz_get_cell_neighbors(Puzzle* pz, PuzzleCell* cell) {
  int pz_cell_neighbors_x[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
  int pz_cell_neighbors_y[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
  LinkedList neighbors = ll_create();
  for (int i = 0; i < 8; i++) {
    int x = pz_cell_x(pz, cell) + pz_cell_neighbors_x[i];
    int y = pz_cell_y(pz, cell) + pz_cell_neighbors_y[i];
    PuzzleCell* cell = pz_get_cell_xy(pz, x, y);
    if (cell != NULL) ll_enqueue(neighbors, cell);
  }
  return neighbors;
}

int pz_xy_to_id(Puzzle* pz, int x, int y) {
  if (pz_valid_cell_xy(pz, x, y)) {
    return x + y * pz->size;
  }
  return -1;
}

int pz_valid_cell_id(Puzzle* pz, int id) {
  return (0 <= id && id < pz->size * pz->size);
}
int pz_valid_cell_xy(Puzzle* pz, int x, int y) {
  return (0 <= x && x < pz->size && 0 <= y && y < pz->size);
}

int pz_cell_x(Puzzle* pz, PuzzleCell* cell) {
  return cell->id % pz->size;
}
int pz_cell_y(Puzzle* pz, PuzzleCell* cell) {
  return cell->id / pz->size;
}

/**
 * Get all possible new paths when adding a new cell to a given path.
 */
LinkedList pz_add_cell_to_path(PuzzlePath* path, PuzzleCell* cell) {
  LinkedList results = ll_create();
  if (pz_path_contains_cell(path, cell)) return results;
  PuzzlePath* new_path = pz_copy_path(path);
  ll_enqueue(results, new_path);
  ll_enqueue(new_path->cells, cell);
  ll_enqueue(new_path->word, box_char(cell->c1));

  if (cell->type == 1) {
    ll_enqueue(new_path->word, box_char(cell->c2));
  } else if (cell->type == 2) {
    new_path = pz_copy_path(path);
    ll_enqueue(results, new_path);
    ll_enqueue(new_path->cells, cell);
    ll_enqueue(new_path->word, box_char(cell->c2));
  }

  return results;
}

int pz_path_contains_cell(PuzzlePath* path, PuzzleCell* cell) {
  return ll_contains_data(path->cells, cell, pz_compare_cells);
}

int pz_compare_cells(void* data1, void* data2) {
  PuzzleCell* cell1 = (PuzzleCell*) data1;
  PuzzleCell* cell2 = (PuzzleCell*) data2;
  return (cell1->id != cell2->id);
}

char* pz_path_to_word(PuzzlePath* path) {
  return pz_ll_collapse_word(path->word);
}

int pz_compare_paths_length_desc(void* data1, void* data2) {
  PuzzlePath* path1 = (PuzzlePath*) data1;
  PuzzlePath* path2 = (PuzzlePath*) data2;
  return ll_size(path1->word) < ll_size(path2->word);
}
int pz_compare_paths_length_asc(void* data1, void* data2) {
  return (!pz_compare_paths_length_desc(data1, data2));
}

/**
 * Collapse a linked list of characters into an array of characters.
 */
char* pz_ll_collapse_word(LinkedList word) {
  int size = ll_size(word) + 1;
  char* string = calloc(size, sizeof(char));
  ll_iterate(word, pz_ll_collapse_word_callback, (void*) string);
  string[size - 1] = '\0';
  return string;
}
void pz_ll_collapse_word_callback(int idx, void* data, void* params) {
  ((char*) params)[idx] = unbox_char(data);
}

PuzzleResults* pz_build_results_from_list(LinkedList results_list) {
  PuzzleResults* results = (PuzzleResults*) malloc(sizeof(PuzzleResults));
  results->size = ll_size(results_list);
  results->paths = (PuzzlePath**) ll_to_array(results_list);
  return results;
}

void pz_destroy_results(PuzzleResults* results) {
  free(results->paths);
  free(results);
}

void pz_destroy_results_data(PuzzleResults* results) {
  for (int i = 0; i < results->size; i++) {
    pz_destroy_path(results->paths[i]);
  }
  pz_destroy_results(results);
}
