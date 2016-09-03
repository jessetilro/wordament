#include "printer.h"

/**
 * Print a puzzle.
 */
void print_puzzle(Puzzle* puzzle) {
  for (int i = 0; i < puzzle->size * puzzle->size; i++) {
    PuzzleCell* cell = pz_get_cell(puzzle, i);
    if (cell->type == 0) {
      printf("[ %c ] ", cell->c1);
    } else if (cell->type == 1) {
      printf("[%c %c] ", cell->c1, cell->c2);
    } else {
      printf("[%c/%c] ", cell->c1, cell->c2);
    }
    if ((i+1) % 4 == 0) printf("\n");
  }
}

/**
 * Print a puzzle cell.
 */
void print_puzzle_cell(PuzzleCell* cell) {
  printf("[%i:%c%c%c]", cell->id, cell->c1, (cell->type == 2) ? '/' : '\0', cell->c2);
}

/**
 * Print a puzzle path.
 */
 void print_puzzle_path(PuzzlePath* path) {
   LinkedNode* cell = (*path->cells);
   while (cell != NULL) {
     print_puzzle_cell((PuzzleCell*) cell->data);
     cell = cell->next;
   }
   printf("\n");
 }
