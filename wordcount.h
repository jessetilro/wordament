#include <inttypes.h>
#include <stdio.h>
#include <ctype.h>

#include "hashmap.h"

void count_words(FILE* stream);
void print_wordcount(const char* key, void* data);

void destroy_wordcount_data(void* data);
void* resolve_collision_increment(void* old_value, void* new_value);
unsigned int hash_polynomial(const char* key);
uintmax_t polynomial(unsigned int p, unsigned int z, const char* av, unsigned int n);
