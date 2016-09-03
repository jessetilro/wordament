#include "parser.h"

/**
 * Reads through all characters in a specified file sequentially,
 * putting them into a puzzle matrix from left to right, top to bottom.
 * Also supports cells with multiple letters.
 */
void parse_puzzle_from_file(const char file_name[], Puzzle* puzzle) {
  FILE* fp = fopen(file_name, "r");

  printf("Building puzzle datastructure from: '%s'\n", file_name);
  if (fp == NULL) {
    printf("Error: unable to open file '%s'\n", file_name);
    exit(-1);
  }

  char ch;
  int id = 0;
  int expecting = 1;
  PuzzleCell* cell = pz_create_cell(id, 0, '\0', '\0');
  while ((ch = fgetc(fp)) != EOF) {
    if (ch == '(' || ch == '/') {
      expecting = 2;
      cell->type = (ch == '(') ? 1 : 2;
      continue;
    }
    if (expecting && isalpha(ch)) {
      if (cell->c1 == '\0') {
        cell->c1 = ch;
      } else {
        cell->c2 = ch;
      }
      --expecting;
    }
    if (!expecting) {
      pz_add_cell(puzzle, cell);
      expecting = 1;
      ++id;
      cell = pz_create_cell(id, 0, '\0', '\0');
    }
    if (id >= puzzle->size * puzzle->size) break;
  }

  fclose(fp);
}

/**
 * Group all words with the same three letter prefix in a linked list
 * in the bucket of a hashmap.
 */
void parse_dictionary_from_file(const char file_name[], HashMap* hm) {
  FILE* fp = fopen(file_name, "r");

  printf("Building dictionary datastructure from: '%s'\n", file_name);
  if (fp == NULL) {
    printf("Error: unable to open file '%s'\n", file_name);
    exit(-1);
  }

  char ch;
  char* prefix = (char*) calloc(4, sizeof(char));
  LinkedList word = ll_create();
  prefix[3] = '\0';
  int count = 0;
  int pos = 0;
  int reading = 1;
  while ((ch = fgetc(fp)) != EOF) {
    if (isalpha(ch) && reading) {
      if (pos <= 2) {
        prefix[pos] = ch;
        pos += 1;
      }
      ll_enqueue(word, box_char(ch));
    } else {
      reading = 0;
    }
    if (ch == '\n') {
      if (pos > 2) {
        if (get_bucket(hm, prefix) == NULL) {
          //printf("Prefix found: %s\n", prefix);
          LinkedList words = ll_create();
          insert_data(hm, prefix, (void*) words, NULL);
          count++;
        }
        LinkedList words = (LinkedList) get_data(hm, prefix);
        char* string = pz_ll_collapse_word(word);
        //printf("Adding word to dictionary: %s\n", string);
        ll_enqueue(words, string);
      }
      ll_clear(word, destroy_char);
      reading = 1;
      pos = 0;
    }
  }

  ll_destroy(word, destroy_char);
  free(prefix);

  fclose(fp);
}
