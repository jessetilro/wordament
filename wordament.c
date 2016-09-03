#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "printer.h"
#include "puzzle.h"
#include "queue.h"
#include "box.h"
#include "mergesort.h"
#include "linkedlist.h"
#include "hashmap.h"
#include "hash.h"

// Define color codes for printing.
#define C_NRM  "\x1B[0m"
#define C_RED  "\x1B[31m"
#define C_GRN  "\x1B[32m"
#define C_BLU  "\x1B[34m"
#define C_YEL  "\x1B[33m"
#define C_MAG  "\x1B[35m"
#define C_CYN  "\x1B[36m"

// The datastructure built to represent the puzzle.
Puzzle* puzzle;

// The datastructure to contain an efficiently searchable dictionary.
HashMap* dictionary;

// Number of threads to use.
const int tc = 16;

// Size of the puzzle
const int puzzle_size = 4;

// Minimum length of a valid word.
const int min_word_length = 3;

// Datastructure for the threads to write results to.
// Not thread safe, so a concurrency control mechanism should be used!
HashMap* results_map;
LinkedList results_list;

// Declare struct to be used for passing parameters to thread.
// Passing an index is enough for a thread to know which region to validate
// (i.e. which row, column or grid depending on the thread function).
// Additionally a thread needs to know its index (tid) in the result array.
typedef struct {
   int index;
} parameters;



// Declare main subroutines and thread functions.

void find_words();
void* find_words_from(void* params);

int validate_path_word(PuzzlePath* path);
int validate_word(char* word);

void add_path_to_results_map(HashMap* results, PuzzlePath* path);
void add_path_to_results_list(LinkedList results, PuzzlePath* path);

void destroy_wordlist(void* data);

void print_header(const char header[]);

// Mutex, since printf is not a reentrant function.
pthread_mutex_t printf_mutex;
pthread_mutex_t result_mutex;
pthread_mutex_t dictionary_mutex;

/**
 * Main program 2_bonus.
 * Validates the completed puzzle encoded in the specified text file.
 */
int main(int argc, char* argv[]) {
  // Print a nice header before all other output.
  print_header("*** WORDAMENT ***");
  // Try to read the puzzle file.
  if (argc < 3) {
    printf("Usage: './wordament <puzzle_file_url> <dictionary_file_url>'\n");
    return 3;
  }
  // int n[8] = {2,3,1,4,4,9,7,9};
  // void** test_array = calloc(8, sizeof(int*));
  // for (int i = 0; i < 8; i++) {
  //   test_array[i] = box_int(n[i]);
  // }
  // mergesort(test_array, 8, &ms_compare_ints);
  // for (int i = 0; i < 8; i++) {
  //   printf("%i", remove_int(test_array[i]));
  // }
  // free(test_array);
  // printf("\n");
  // return 0;

  puzzle = pz_create(4);
  parse_puzzle_from_file(argv[1], puzzle);
  printf("The parsed puzzle is:\n");
  print_puzzle(puzzle);

  // Create Prefix HashMap.
  dictionary = create_hashmap(1024);
  results_map = create_hashmap(1024);
  set_hash_function(dictionary, hash_polynomial);
  set_hash_function(results_map, hash_polynomial);

  parse_dictionary_from_file(argv[2], dictionary);

  results_list = ll_create();

  find_words();

  PuzzleResults* results = pz_build_results_from_list(results_list);

  printf("Sorting words by length (descending order)\n");
  mergesort((void**) results->paths, results->size, &pz_compare_paths_length_desc);

  printf("Printing words:\n");
  for (int i = 0; i < results->size; i++) {
    PuzzlePath* path = results->paths[i];
    char* word = pz_path_to_word(path);
    if (get_bucket(results_map, word) == NULL) {
      printf("%s\n", word);
      insert_data(results_map, word, NULL, NULL);
    }
    free(word);
  }

  pz_destroy_results(results);

  ll_destroy(results_list, pz_ll_destroy_path);

  delete_hashmap(results_map, NULL);
  delete_hashmap(dictionary, destroy_wordlist);

  pz_destroy(puzzle);

  return 0;
}

/**
 * Main program that controls all threads that validate the puzzle.
 */
void find_words() {
  // Keep handles of all created threads.
  pthread_t threads[tc];
  // Keep track of all spaces in memory allocated for thread parameters.
  parameters* param[tc];

  // Create a new thread for each region,
  // allocate necessary memory space.
  int tid = 0;
  while (tid < tc) {
    // Allocate memory for thread parameters.
    param[tid] = (parameters*) malloc(sizeof(parameters));
    param[tid]->index = tid;
    // Determine thread function by region (row, column or subgrid).
    void* (*func_ptr)(void*);
    func_ptr = &find_words_from;
    // Create thread
    if (pthread_create(&threads[tid], NULL, func_ptr, param[tid])) {
      printf("Error: failed to create thread %i.\n", tid);
      exit(1);
    }
    //printf("Launching thread finding words starting at cell %i\n", tid);
    ++tid;
  }

  // Join the threads for each region,
  // free used memory space.
  tid = 0;
  while (tid < tc) {
    // Wait for thread to finish by joining it,
    // report error if this fails.
    if (pthread_join(threads[tid], NULL)) {
      printf("Error: failed to join thread %i.\n", tid);
      exit(2);
    }
    // Free memory allocated for thread parameters.
    free(param[tid]);
    ++tid;
  }
}

void* find_words_from(void* params) {
  int start_id = ((parameters*) params)->index;
  PuzzleCell* start_cell = pz_get_cell(puzzle, start_id);
  PuzzlePath* start_path = pz_create_path();
  LinkedList start_paths = pz_add_cell_to_path(start_path, start_cell);

  LinkedList bfs = ll_create();
  ll_enqueue_all(bfs, start_paths);

  ll_destroy(start_paths, NULL);

  while (!ll_is_empty(bfs)) {
    PuzzlePath* path = (PuzzlePath*) ll_dequeue(bfs, NULL);
    PuzzleCell* cell = (PuzzleCell*) ll_get_last(path->cells);
    LinkedList neighbors = pz_get_cell_neighbors(puzzle, cell);
    while (!ll_is_empty(neighbors)) {
      PuzzleCell* nb = ll_dequeue(neighbors, NULL);
      LinkedList new_paths = pz_add_cell_to_path(path, nb);
      while (!ll_is_empty(new_paths)) {
        PuzzlePath* new_path = ll_dequeue(new_paths, NULL);
        int length = ll_size(new_path->word);
        int valid = 1;
        if (length >= 3) valid = validate_path_word(new_path);
        if (valid) {
          ll_enqueue(bfs, new_path);
          if (valid == 2) {
            add_path_to_results_list(results_list, new_path);
          }
        } else {
          pz_destroy_path(new_path);
        }
      }
      ll_destroy(new_paths, NULL);
    }
    ll_destroy(neighbors, NULL);
    pz_destroy_path(path);
  }
  ll_destroy(bfs, pz_ll_destroy_path);

  return NULL;
}

int validate_path_word(PuzzlePath* path) {
  char* word = pz_path_to_word(path);
  int result = validate_word(word);
  free(word);
  return result;
}

/**
 * Validates whether this is either a correct word or the start of one.
 * Correct words yield a different result.
 */
int validate_word(char* word) {
  pthread_mutex_lock(&dictionary_mutex);

  char prefix[4];
  for (int i = 0; i < 3; i++) {
    prefix[i] = word[i];
  }
  prefix[3] = '\0';

  LinkedList candidates = (LinkedList) get_data(dictionary, prefix);
  if (candidates == NULL) {
    pthread_mutex_unlock(&dictionary_mutex);
    return 0;
  }
  LinkedNode* candidate = (*candidates);

  int matches = 0;
  while (candidate != NULL) {
    char* other = (char*) candidate->data;
    int pos = 3;
    while (word[pos] != '\0' && other[pos] != '\0') {
      if (word[pos] != other[pos]) {
        if (pos - 3 < matches || word[pos] < other[pos]) {
          pthread_mutex_unlock(&dictionary_mutex);
          return 0;
        }
        break;
      }
      if (pos - 2 > matches) matches = pos - 2;
      pos++;
    }
    if (word[pos] == '\0') {
      int result = 1;
      if (other[pos] == '\0' && pos >= min_word_length) {
        // pthread_mutex_lock(&printf_mutex);
        // printf("%s\n", word);
        // pthread_mutex_unlock(&printf_mutex);
        result = 2;
      }
      pthread_mutex_unlock(&dictionary_mutex);
      return result;
    }
    candidate = candidate->next;
  }
  pthread_mutex_unlock(&dictionary_mutex);
  return 0;
}

/**
 * Add a path to a results hashmap datastructure.
 */
void add_path_to_results_map(HashMap* results, PuzzlePath* path) {
  char* key = pz_path_to_word(path);
  insert_data(results, key, (void*) pz_copy_path(path), NULL);
  free(key);
}

/**
 * Add a path to a results linked list datastructure.
 */
void add_path_to_results_list(LinkedList results, PuzzlePath* path) {
  ll_enqueue(results, (void*) pz_copy_path(path));
}

/**
 * Print a header while modulating text color.
 */
void print_header(const char header[]) {
  int i = 0;
  while (*header != '\0') {
    printf("%s%c" C_NRM, C_CYN, *header);
    header++;
    ++i;
  }
  printf(C_NRM "\n");
}

void destroy_wordlist(void* data) {
  LinkedList wordlist = (LinkedList) data;
  ll_destroy(wordlist, destroy_char);
}
