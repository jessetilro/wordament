#include "mergesort.h"

void mergesort(void** array, int size, MSCompareFunction compare) {
  // Use two threads for sorting.
  int tc = 2;
  // Build list datastructure to work with.
  MergeSortList* list = ms_build_list(size, array);
  // Keep handles of all created threads.
  pthread_t threads[tc+1];
  // Keep track of spaces in memory allocated for thread parameters.
  thread_params* params[tc];
  // Function pointers for the threads.
  thread_func_ptr thread_funcs[] = {&thread_sort, &thread_sort, &thread_merge};

  // Create a new thread for each thread function.
  int index = 0;
  while (index < tc) {
    // Allocate memory space for thread parameters.
    int idx = ((size - 1) / 2);
    params[index] = (index == 0) ? ms_build_params(0, idx, list, compare) : ms_build_params(idx + 1, size - 1, list, compare);
    // Create thread
    if (pthread_create(&threads[index], NULL, thread_funcs[index], params[index])) {
      printf("Error while merging: failed to create thread %i.\n", index);
      ms_destroy_list(list);
      exit(1);
    }
    ++index;
  }

  // Join all the threads.
  index = 0;
  while (index < tc) {
    // Wait for thread to finish by joining it,
    // report error if this fails.
    if (pthread_join(threads[index], NULL)) {
      printf("Error while merging: failed to join thread %i.\n", index);
      ms_destroy_list(list);
      exit(2);
    }
    // Free memory allocated for current thread's parameters.
    ms_destroy_params(params[index]);
    ++index;
  }

  // Combine thread results in a dedicated thread.
  thread_params* merge_params = ms_build_params(0, size - 1, list, compare);
  pthread_create(&threads[tc], NULL, thread_funcs[tc], merge_params);
  if (pthread_join(threads[tc], NULL)) {
    printf("Error while merging: failed to join thread %i.\n", tc);
    ms_destroy_list(list);
    exit(5);
  }
  ms_destroy_params(merge_params);
  ms_destroy_list(list);
}

/**
 * Thread: sort assigned subset of the input numbers
 * using a merge sort algorithm.
 */
void* thread_sort(void* params_void) {
  // Get numbers from params.
  thread_params* p = (thread_params*) params_void;
  // Sort assigned subset of the input numbers.
  merge_sort(&p->list->array[p->idx1], (p->idx2 - p->idx1) + 1, sizeof(void*), p->compare);

  return NULL;
}

/**
 * Thread: merge two sorted halves of an array.
 * Takes a nums pointer instead of a thread_params pointer.
 */
void* thread_merge(void* params_void) {
  // Get numbers from params.
  thread_params* p = (thread_params*) params_void;
  // Merge sorted halves of the sequence of numbers.
  merge_sort_combine(p->list->array, p->list->size, sizeof(void*), p->compare);

  return NULL;
}

/**
 *  Sort a given array using a merge sort algorithm.
 *  Part of the algorithm (merging phase) is reused in the main program,
 *  namely for combining the solutions found by the different threads.
 *  I implemented this because I was not sure whether or not it is allowed
 *  to use library functions for the sorting in this assignment (I assumed not).
 */
void merge_sort(void* base, size_t nitems, size_t size, MSCompareFunction compar) {
  if (nitems > 2) {
    // Split the array, solve subproblems.
    int idx = ((nitems - 1) / 2) + 1;
    void* base2 = (char*) base + idx * size;
    merge_sort(base, idx, size, compar);
    merge_sort(base2, nitems - idx, size, compar);
    // Combine solutions.
    merge_sort_combine(base, nitems, size, compar);
  } else if (nitems == 2) {
    void* succ = (char*) base + size;
    // Swap the two elements if they are out of order.
    if (ms_compare_wrapper(base, succ, compar) > 0) {
      void* buffer = malloc(size);
      memcpy(buffer, base, size);
      memcpy(base, succ, size);
      memcpy(succ, buffer, size);
      free(buffer);
    }
  }
}

/**
 * Merge two sorted subsets of an array.
 * Assumes that the two halves of the array have already been sorted.
 * This function is also used by the main program in order
 * to combine the solutions of the different threads.
 */
void merge_sort_combine(void* base, size_t nitems, size_t size, MSCompareFunction compar) {
  int idx = ((nitems - 1) / 2) + 1;
  void* base2 = (char*) base + idx * size;
  void* buffer = (void*) calloc(nitems, size);
  memcpy(buffer, base, nitems * size);
  int p1 = 0;
  int p2 = 0;
  while (p1 < idx || p2 < nitems - idx) {
    if (p1 < idx && p2 < nitems - idx) {
      void* el1 = (char*) base + p1 * size;
      void* el2 = (char*) base2 + p2 * size;
      if (ms_compare_wrapper(el1, el2, compar) > 0) {
        memcpy((char*) buffer + (p1 + p2) * size, el2, size);
        ++p2;
      } else {
        memcpy((char*) buffer + (p1 + p2) * size, el1, size);
        ++p1;
      }
    } else if (p1 < idx) {
      memcpy((char*) buffer + (p1 + p2) * size, (char*) base + p1 * size, size);
      ++p1;
    } else {
      memcpy((char*) buffer + (p1 + p2) * size, (char*) base2 + p2 * size, size);
      ++p2;
    }
  }
  memcpy(base, buffer, nitems * size);
  free(buffer);
}

int ms_compare_wrapper(void* data1, void* data2, MSCompareFunction func) {
  return func(*((void**) data1), *((void**) data2));
}

/**
 * Compare two floats. Used by the merge_sort algorithm.
 */
int compare_floats(void* float1, void* float2) {
  return (*(float*) float1) > (*(float*) float2);
}
/**
 * Compare two floats. Used by the merge_sort algorithm.
 */
int ms_compare_ints(void* int1, void* int2) {
  return (*(int*) int1) > (*(int*) int2);
}

/**
 * Build a nums struct for input data.
 */
MergeSortList* ms_build_list(int size, void** array) {
  MergeSortList* list = (MergeSortList*) malloc(sizeof(MergeSortList));
  list->size = size;
  list->array = array;
  return list;
}

/**
 * Destroy a nums struct for input data.
 */
void ms_destroy_list(MergeSortList* list) {
  free(list);
}

/**
 * Build a thread_params struct for thread parameters.
 */
thread_params* ms_build_params(int idx1, int idx2, MergeSortList* list, MSCompareFunction compare) {
  thread_params* p = (thread_params*) malloc(sizeof(thread_params));
  p->idx1 = idx1;
  p->idx2 = idx2;
  p->list = list;
  p->compare = compare;
  return p;
}

/**
 * Destroy a thread_params struct for thread parameters.
 */
void ms_destroy_params(thread_params* p) {
  free(p);
}
