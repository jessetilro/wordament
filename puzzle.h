#include "linkedlist.h"
#include "box.h"

#ifndef PUZZLE
#define PUZZLE

// Datastructure for a single cell in a wordament puzzle.
typedef struct PuzzleCell PuzzleCell;
struct PuzzleCell {
  int id;
  int type;
  char c1;
  char c2;
};

// Datastructure for an entire wordament puzzle instance.
typedef struct Puzzle Puzzle;
struct Puzzle {
  int size;
  PuzzleCell** cells;
};

// Datastructure for a path through a wordament puzzle.
typedef struct PuzzlePath PuzzlePath;
struct PuzzlePath {
  LinkedList cells;
  LinkedList word;
};

// Datastructure for a solution to the puzzle.
typedef struct PuzzleResults PuzzleResults;
struct PuzzleResults {
  int size;
  PuzzlePath** paths;
};

Puzzle* pz_create(int size);
void pz_destroy(Puzzle* puzzle);

PuzzleCell* pz_create_cell(int id, int type, char c1, char c2);
void pz_destroy_cell(PuzzleCell* cell);

PuzzlePath* pz_create_path();
void pz_destroy_path(PuzzlePath*);
PuzzlePath* pz_copy_path(PuzzlePath*);

void pz_ll_destroy_cell(void* data);
void pz_ll_destroy_path(void* data);

void pz_add_cell(Puzzle* pz, PuzzleCell* cell);
PuzzleCell* pz_get_cell(Puzzle* pz, int id);
PuzzleCell* pz_get_cell_xy(Puzzle* pz, int x, int y);
LinkedList pz_get_cell_neighbors(Puzzle* pz, PuzzleCell* cell);
int pz_valid_cell_id(Puzzle* pz, int id);
int pz_valid_cell_xy(Puzzle* pz, int x, int y);
int pz_xy_to_id(Puzzle* pz, int x, int y);
int pz_cell_x(Puzzle* pz, PuzzleCell* cell);
int pz_cell_y(Puzzle* pz, PuzzleCell* cell);

LinkedList pz_add_cell_to_path(PuzzlePath* path, PuzzleCell* cell);
int pz_path_contains_cell(PuzzlePath* path, PuzzleCell* cell);
int pz_compare_cells(void* data1, void* data2);
char* pz_path_to_word(PuzzlePath* path);

int pz_compare_paths_length_desc(void* data1, void* data2);
int pz_compare_paths_length_asc(void* data1, void* data2);

char* pz_ll_collapse_word(LinkedList word);
void pz_ll_collapse_word_callback(int idx, void* data, void* params);

PuzzleResults* pz_build_results_from_list(LinkedList results_list);
void pz_destroy_results(PuzzleResults* results);
void pz_destroy_results_data(PuzzleResults* results);

#endif
