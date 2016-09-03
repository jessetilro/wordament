#include "linkedlist.h"

/**
 * Enqueue an element to the list.
 */
void ll_enqueue(LinkedList q, void* data) {
  LinkedNode* node = (LinkedNode*) malloc(sizeof(LinkedNode));
  node->data = data;
  node->next = NULL;
  if (ll_is_empty(q)) {
    (*q) = node;
  } else {
    LinkedNode* qp = (*q);
    while (qp->next != NULL) {
      qp = qp->next;
    }
    qp->next = node;
  }
}

/**
 * Enqueue all items from a given other linked list.
 */
void ll_enqueue_all(LinkedList q, LinkedList data) {
  LinkedNode* node = (*data);
  while (node != NULL) {
    ll_enqueue(q, node->data);
    node = node->next;
  }
}

/**
 * Push an element to the stack.
 */
void ll_push(LinkedList ll, void* data) {
  ll_enqueue(ll, data);
}

/**
 * Pop an element from a queue.
 */
void* ll_dequeue(LinkedList q, LLDestroyDataCallback destroy) {
  if (!ll_is_empty(q)) {
    void* data = (*q)->data;
    LinkedNode* tmp = (*q);
    (*q) = (*q)->next;
    free(tmp);
    if (destroy == NULL) {
      return data;
    } else {
      destroy(data);
    }
  }
  return NULL;
}

/**
 * Pop an element from the stack.
 */
void* ll_pop(LinkedList q, LLDestroyDataCallback destroy) {
  if (!ll_is_empty(q)) {
    LinkedNode* prev = NULL;
    LinkedNode* node = (*q);
    while (node->next != NULL) {
      prev = node;
      node = node->next;
    }
    if (prev == NULL) {
      (*q) = NULL;
    } else {
      prev->next = NULL;
    }
    void* data = node->data;
    free(node);
    if (destroy == NULL) {
      return data;
    } else {
      destroy(data);
    }
  }
  return NULL;
}

/**
 * Check whether a queue contains a data pointer.
 */
int ll_contains(LinkedList q, void* pointer) {
  LinkedNode* node = (*q);
  while (node != NULL) {
    if (pointer == node->data) {
      return 1;
    }
    node = node->next;
  }
  return 0;
}

/**
 * Check whether a queue contains a data pointer.
 */
int ll_contains_data(LinkedList q, void* data, LLCompareDataCallback cmp) {
  LinkedNode* node = (*q);
  while (node != NULL) {
    if (cmp(data, node->data) == 0) {
      return 1;
    }
    node = node->next;
  }
  return 0;
}

/**
 * Iterate over data in linked list in fifo order.
 */
void ll_iterate(LinkedList ll, LLIterateCallback iterate, void* params) {
  LinkedNode* node = (*ll);
  int idx = 0;
  while (node != NULL) {
    iterate(idx, node->data, params);
    node = node->next;
    idx++;
  }
}

/**
 * Get the last data in the queue.
 */
void* ll_get_last(LinkedList q) {
  if (ll_is_empty(q)) return NULL;
  LinkedNode* node = (*q);
  while (node->next != NULL) {
    node = node->next;
  }
  return node->data;
}

/**
 * Get the first data in the queue.
 */
void* ll_get_first(LinkedList q) {
  if (ll_is_empty(q)) return NULL;
  return (*q)->data;
}

/**
 * Test whether a given queue is empty.
 */
int ll_is_empty(LinkedList q) {
  return ((*q) == NULL);
}

/**
 * Compute queue size.
 */
int ll_size(LinkedList q) {
  LinkedNode* node = (*q);
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
LinkedList ll_create() {
  LinkedList head = (LinkedList) malloc(sizeof(LinkedNode*));
  (*head) = NULL;
  return head;
}

/**
 * Copy a queue.
 */
LinkedList ll_copy(LinkedList q) {
  LinkedList copy = ll_create();
  LinkedNode* node = (*q);
  while (node != NULL) {
    ll_enqueue(copy, node->data);
    node = node->next;
  }
  return copy;
}

/**
 * Copy a queue and its data.
 */
LinkedList ll_copy_data(LinkedList q, LLCopyDataCallback copy_data) {
  LinkedList copy = ll_create();
  LinkedNode* node = (*q);
  while (node != NULL) {
    ll_push(copy, copy_data(node->data));
    node = node->next;
  }
  return copy;
}

/**
 * Remove all nodes from a queue and optionally destroy their contents.
 */
void ll_clear(LinkedList q, LLDestroyDataCallback destroy) {
  while (!ll_is_empty(q)) {
    ll_dequeue(q, destroy);
  }
}

/**
 * Destroy a queue datastructure.
 */
void ll_destroy(LinkedList q, LLDestroyDataCallback destroy) {
  ll_clear(q, destroy);
  free(q);
}

/**
 * Get an array of data pointers contained in a given linked list.
 */
void** ll_to_array(LinkedList ll) {
  int size = ll_size(ll);
  void** array = calloc(size, sizeof(void*));
  LinkedNode* node = (*ll);
  for (int i = 0; i < size; i++) {
    if (node == NULL) break;
    array[i] = node->data;
    node = node->next;
  }
  return array;
}
