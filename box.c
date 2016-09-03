#include <stdio.h>
#include <stdlib.h>

#include "box.h"

void* box_int(int i) {
  int* d = malloc(sizeof(int));
  (*d) = i;
  return (void*) d;
}
int unbox_int(void* d) {
  return *((int*) d);
}
void destroy_int(void* d) {
  free((int*) d);
}
int remove_int(void* d) {
  int i = unbox_int(d);
  destroy_int(d);
  return i;
}
void* copy_int(void* d) {
  return box_int(unbox_int(d));
}

void* box_char(char c) {
  char* d = malloc(sizeof(char));
  (*d) = c;
  return (void*) d;
}
char unbox_char(void* d) {
  return *((char*) d);
}
void destroy_char(void* d) {
  free((char*) d);
}
char remove_char(void* d) {
  char c = unbox_char(d);
  destroy_char(d);
  return c;
}
void* copy_char(void* d) {
  return box_char(unbox_char(d));
}
