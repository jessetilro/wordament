#include <stdio.h>
#include <stdlib.h>

#ifndef LINKEDLIST
#define LINKEDLIST

// Datastructure for a node in a linkedlist.
typedef struct LinkedNode LinkedNode;
struct LinkedNode {
  void* data;
  LinkedNode* next;
};

// Datastructure for a linked list.
typedef LinkedNode** LinkedList;

// Callback function types.
typedef int (*LLCompareDataCallback)(void* data1, void* data2);
typedef void (*LLDestroyDataCallback)(void* data);
typedef void* (*LLCopyDataCallback)(void* data);
typedef void (*LLIterateCallback)(int idx, void* data, void* params);

// Functions.
void ll_enqueue(LinkedList q, void* data);
void ll_enqueue_all(LinkedList q, LinkedList data);
void ll_push(LinkedList q, void* data);
void* ll_dequeue(LinkedList q, LLDestroyDataCallback destroy);
int ll_contains(LinkedList q, void* pointer);
int ll_contains_data(LinkedList q, void* data, LLCompareDataCallback cmp);
void ll_iterate(LinkedList ll, LLIterateCallback iterate, void* params);
void* ll_get_last(LinkedList q);
void* ll_get_first(LinkedList q);
void ll_clear(LinkedList q, LLDestroyDataCallback destroy);
int ll_is_empty(LinkedList q);
int ll_size(LinkedList q);
LinkedList ll_create();
LinkedList ll_copy(LinkedList q);
LinkedList ll_copy_data(LinkedList q, LLCopyDataCallback copy_data);
void ll_clear(LinkedList q, LLDestroyDataCallback destroy);
void ll_destroy(LinkedList q, LLDestroyDataCallback destroy);
void ll_iterate(LinkedList ll, LLIterateCallback iterate, void* params);
void** ll_to_array(LinkedList ll);

#endif
