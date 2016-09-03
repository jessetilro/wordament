#include "hashmap.h"

struct HashMap {
  size_t size;
  HashFunction hash_function;
  Bucket** buckets;
};

struct Bucket {
  char* key;
  void* data;
  Bucket* next;
};

HashMap* create_hashmap(size_t key_space) {
  HashMap* hm = malloc(sizeof(HashMap));
  hm->size = key_space;
  hm->hash_function = hash;
  hm->buckets = calloc(key_space, sizeof(Bucket*));
  return hm;
}

Bucket* create_bucket(const char* key, void* data, Bucket* next) {
  Bucket* bucket = malloc(sizeof(Bucket));
  bucket->key = strdup(key);
  bucket->data = data;
  bucket->next = next;
  return bucket;
}

unsigned int hash(const char* key) {
  int length = strlen(key);
  int sum = 0;
  for (int i = 0; i < length; i++) {
    sum += (int) key[i];
  }
  return sum;
}

void insert_data(HashMap* hm, const char* key, void* data, ResolveCollisionCallback resolve_collision) {
  Bucket* bucket = get_bucket(hm, key);
  if (bucket == NULL) {
    Bucket* new_bucket = create_bucket(key, data, NULL);
    add_bucket(hm, new_bucket);
  } else if (resolve_collision != NULL) {
    bucket->data = resolve_collision(bucket->data, data);
  }
}

void* get_data(HashMap* hm, const char* key) {
  Bucket* bucket = get_bucket(hm, key);
  if (bucket != NULL) {
    return bucket->data;
  }
  return NULL;
}

void iterate(HashMap* hm, void (*callback)(const char* key, void* data)) {
  for (unsigned int i = 0; i < hm->size; i++) {
    Bucket* bucket = hm->buckets[i];

    while (bucket != NULL) {
      callback(bucket->key, bucket->data);
      bucket = bucket->next;
    }
  }
}

void remove_data(HashMap* hm, const char* key, DestroyDataCallback destroy_data) {
  Bucket* bucket = get_bucket(hm, key);
  if (bucket != NULL) {
    Bucket* prev = get_bucket_prev(hm, bucket);
    Bucket* next = bucket->next;
    remove_bucket(hm, bucket, destroy_data);
    if (prev == NULL) {
      int index = get_bucket_index(hm, key);
      hm->buckets[index] = next;
    }
  }
}

void delete_hashmap(HashMap* hm, DestroyDataCallback destroy_data) {
  for (unsigned int i = 0; i < hm->size; i++) {
    Bucket* list = hm->buckets[i];
    remove_buckets(hm, list, destroy_data);
  }
  free(hm->buckets);
  free(hm);
}

int get_bucket_index(HashMap* hm, const char* key) {
  return hm->hash_function(key) % hm->size;
}

Bucket* get_bucket_list(HashMap* hm, const char* key) {
  int index = get_bucket_index(hm, key);
  Bucket* bucket = hm->buckets[index];
  return bucket;
}

Bucket* get_bucket_prev(HashMap* hm, Bucket* bucket) {
  Bucket* prev = NULL;
  Bucket* search = get_bucket_list(hm, bucket->key);
  while (search != bucket && search != NULL) {
    prev = search;
    search = search->next;
  }
  return prev;
}

Bucket* get_bucket(HashMap* hm, const char* key) {
  Bucket* bucket = get_bucket_list(hm, key);
  while (bucket != NULL) {
    if (strcmp(bucket->key, key) == 0) {
      return bucket;
    }
    bucket = bucket->next;
  }
  return NULL;
}

void add_bucket(HashMap* hm, Bucket* bucket) {
  Bucket* list = get_bucket_list(hm, bucket->key);
  if (list == NULL) {
    int index = get_bucket_index(hm, bucket->key);
    hm->buckets[index] = bucket;
  } else {
    while (list->next != NULL) {
      list = list->next;
    }
    list->next = bucket;
  }
}

void remove_bucket(HashMap* hm, Bucket* bucket, DestroyDataCallback destroy_data) {
  if (bucket != NULL) {
    Bucket* next = bucket->next;
    Bucket* prev = get_bucket_prev(hm, bucket);
    if (prev != NULL) {
      prev->next = next;
    }
    destroy_bucket(bucket, destroy_data);
  }
}

void remove_buckets(HashMap* hm, Bucket* bucket, DestroyDataCallback destroy_data) {
  if (bucket != NULL) {
    remove_buckets(hm, bucket->next, destroy_data);
    remove_bucket(hm, bucket, destroy_data);
  }
}

void destroy_bucket(Bucket* bucket, DestroyDataCallback destroy_data) {
  free(bucket->key);
  if (destroy_data != NULL) {
    destroy_data(bucket->data);
  }
  free(bucket);
}

void set_hash_function(HashMap* hm, unsigned int (*hash_function)(const char* key)) {
  hm->hash_function = hash_function;

  // Rehashing: use a temporary hashmap to avoid loss of data.
  HashMap* thm = create_hashmap(hm->size);
  thm->hash_function = hash_function;
  for (unsigned int i = 0; i < hm->size; i++) {
    Bucket* bucket = hm->buckets[i];
    while (bucket != NULL) {
      insert_data(thm, bucket->key, bucket->data, NULL);
      bucket = bucket->next;
    }
  }

  // Now remove old buckets, copy over the bucket pointers, clear the temporary hashmap and then destroy it.
  for (unsigned int i = 0; i < hm->size; i++) {
    remove_buckets(hm, hm->buckets[i], NULL);
    hm->buckets[i] = thm->buckets[i];
    thm->buckets[i] = NULL;
  }
  delete_hashmap(thm, NULL);
}

char* strdup(const char* s) {
  char* d = malloc(strlen(s) + 1);
  if (d != NULL) strcpy(d,s);
  return d;
}
