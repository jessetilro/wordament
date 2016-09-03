#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define color codes for printing.
#define C_NRM  "\x1B[0m"
#define C_CYN  "\x1B[36m"

// Compare Function
typedef int (*MSCompareFunction)(void* data1, void* data2);

// Typedef and functions for the struct for an input sequence of numbers.
typedef struct MergeSortList MergeSortList;
struct MergeSortList {
  int size;
  void** array;
};
MergeSortList* ms_build_list(int size, void** array);
void ms_destroy_list(MergeSortList* list);

// Typedef and functions for the thread function and parameters struct.
typedef void* (*thread_func_ptr)(void*);
typedef struct {
  int idx1;
  int idx2;
  MergeSortList* list;
  MSCompareFunction compare;
} thread_params;
thread_params* ms_build_params(int idx1, int idx2, MergeSortList* list, MSCompareFunction compare);
void ms_destroy_params(thread_params* p);

int ms_compare_wrapper(void* data1, void* data2, MSCompareFunction func);

// Main function declarations.
void mergesort(void** array, int size, MSCompareFunction compare);
void* thread_sort(void* params_void);
void* thread_merge(void* params_void);
// Sorting algorithm function declarations.
void merge_sort(void* base, size_t nitems, size_t size, MSCompareFunction compar);
void merge_sort_combine(void* base, size_t nitems, size_t size, MSCompareFunction compar);
int compare_floats(void* float1, void* float2);
int ms_compare_ints(void* int1, void* int2);
