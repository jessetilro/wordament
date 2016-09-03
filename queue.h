// Struct for a node in a singly linked list,
// used to implement the FIFO queue.
typedef struct linked_node linked_node;
struct linked_node {
  void* data;
  linked_node* next;
};
typedef linked_node** linked_queue;

// Queue functions for FCFS scheduling.
linked_queue queue_create();
linked_queue queue_copy(linked_queue q);
linked_queue queue_copy_int(linked_queue q);
void queue_destroy(linked_queue q);
void queue_destroy_int(linked_queue q);
void queue_push(linked_queue q, void* data);
void queue_push_int(linked_queue q, int data);
void* queue_pop(linked_queue q);
int queue_pop_int(linked_queue q);
int queue_get_last_int(linked_queue q);
int queue_contains_int(linked_queue q, int i);
void queue_clear(linked_queue q);
void queue_clear_int(linked_queue q);
int queue_is_empty(linked_queue q);
int queue_size(linked_queue q);
