#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/**
 * Push an element to a queue.
 */
void queue_push(linked_queue q, void* data) {
  linked_node* node = (linked_node*) malloc(sizeof(linked_node));
  node->data = data;
  node->next = NULL;
  if (queue_is_empty(q)) {
    (*q) = node;
  } else {
    linked_node* qp = (*q);
    while (qp->next != NULL) {
      qp = qp->next;
    }
    qp->next = node;
  }
}

/**
 * Push an element to a queue with just an integer as data.
 */
void queue_push_int(linked_queue q, int data) {
  void* ptr = malloc(sizeof(int));
  *((int*) ptr) = data;
  queue_push(q, ptr);
}

/**
 * Pop an element from a queue.
 */
void* queue_pop(linked_queue q) {
  if (!queue_is_empty(q)) {
    void* data = (*q)->data;
    linked_node* tmp = (*q);
    (*q) = (*q)->next;
    free(tmp);
    return data;
  }
  return NULL;
}

/**
 * Pop an element from a queue whose data is to be treated as an integer.
 */
int queue_pop_int(linked_queue q) {
  void* data = queue_pop(q);
  if (data != NULL) {
    int i = *((int*) data);
    free(data);
    return i;
  }
  return -1;
}

/**
 * Check whether a queue contains an integer.
 */
int queue_contains_int(linked_queue q, int searched) {
  linked_node* node = (*q);
  while (node != NULL) {
    int found = *((int*) node->data);
    if (searched == found) {
      return 1;
    }
    node = node->next;
  }
  return 0;
}

/**
 * Get the last integer in the queue.
 */
int queue_get_last_int(linked_queue q) {
  if (queue_is_empty(q)) return 0;
  linked_node* node = (*q);
  while (node->next != NULL) {
    node = node->next;
  }
  return *((int*) node->data);
}

/**
 * Pop all elements from a queue.
 */
void queue_clear(linked_queue q) {
  while (!queue_is_empty(q)) {
    queue_pop(q);
  }
}

/**
 * Pop all int elements from a queue.
 */
void queue_clear_int(linked_queue q) {
  while (!queue_is_empty(q)) {
    queue_pop_int(q);
  }
}

/**
 * Test whether a given queue is empty.
 */
int queue_is_empty(linked_queue q) {
  return ((*q) == NULL);
}

/**
 * Compute queue size.
 */
int queue_size(linked_queue q) {
  linked_node* node = (*q);
  int size = 0;
  while (node != NULL) {
    size++;
    node = node->next;
  }
  return size;
}

/**
 * Create a queue datastructure.
 */
linked_queue queue_create() {
  linked_queue head = (linked_queue) malloc(sizeof(linked_node*));
  (*head) = NULL;
  return head;
}

/**
 * Copy a queue.
 */
linked_queue queue_copy(linked_queue q) {
  linked_queue copy = queue_create();
  linked_node* node = (*q);
  while (node != NULL) {
    queue_push(copy, node->data);
    node = node->next;
  }
  return copy;
}

/**
 * Copy a queue including the integers it is holding.
 */
linked_queue queue_copy_int(linked_queue q) {
  linked_queue copy = queue_create();
  linked_node* node = (*q);
  while (node != NULL) {
    int* new_int = (int*) malloc(sizeof(int));
    *new_int = *((int*) node->data);
    queue_push(copy, new_int);
    node = node->next;
  }
  return copy;
}

/**
 * Destroy a queue datastructure.
 */
void queue_destroy(linked_queue q) {
  queue_clear(q);
  free(q);
}

/**
 * Destroy an int queue datastructure.
 */
void queue_destroy_int(linked_queue q) {
  queue_clear_int(q);
  free(q);
}
