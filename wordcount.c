#include "wordcount.h"

const unsigned int polynomial_base = 31;
const size_t hashmap_size = 1024;

void count_words(FILE* stream) {
  // Convert inputstream to char array.
  fseek(stream, 0, SEEK_END);
  long length = ftell(stream);
  char *characters = malloc(length);
  fseek(stream, 0, SEEK_SET);
  fread(characters, 1, length, stream);
  int reading = 0;
  int start = 0;

  // Use hashmap to store wordcounts.
  HashMap* hm = create_hashmap(hashmap_size);
  set_hash_function(hm, hash_polynomial);

  for (int i = 0; i < length; i++) {
    char character = characters[i];
    if (!reading && isalpha(character)) {
      reading = 1;
      start = i;
      if (i != (length - 1)) continue;
    }
    if (reading && (!isalpha(character) || i == length - 1)) {
      reading = 0;
      size_t wordsize = i - start + 1;
      if (isalpha(character)) {
        wordsize += 1;
      }
      char* word = calloc(wordsize, sizeof(char));
      strncpy(word, (char*) &characters[start], wordsize - 1);
      unsigned int null_idx = wordsize - 1;
      word[null_idx] = '\0';

      unsigned int* data = (unsigned int*) get_data(hm, word);
      if (data == NULL) {
        data = malloc(sizeof(unsigned int));
        *data = 1;
      }
      insert_data(hm, word, data, resolve_collision_increment);
      free(word);
    }
  }

  iterate(hm, print_wordcount);
  delete_hashmap(hm, destroy_wordcount_data);

  free(characters);
}

void print_wordcount(const char* key, void* data) {
  printf("%s: %i\n", key, *((int*)data));
}

void destroy_wordcount_data(void* data) {
  free(data);
}

void* resolve_collision_increment(void* old_value, void* new_value) {
  unsigned int* data = (unsigned int*) old_value;
  *data += 1;
	return data;
}

unsigned int hash_polynomial(const char* key) {
  int n = strlen(key);
  uintmax_t hash_code = polynomial(n-1, 31, key, n);
  return (unsigned int) hash_code;
}

// Horner's rule for polynomial evaluation
uintmax_t polynomial(unsigned int p, const unsigned int z, const char* av, unsigned int n) {
  int idx = n - 1;
  if (p == 0) {
    return (uintmax_t) av[idx];
  }
  idx -= p;
  uintmax_t a = (uintmax_t) av[idx];
  return a + (z * polynomial(p - 1, z, av, n));
}
