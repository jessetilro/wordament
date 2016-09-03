#include <stdlib.h>
#include <string.h>

#ifndef HASHMAP
#define HASHMAP

// Hash Map Callbacks
typedef void* (*ResolveCollisionCallback)(void* old_data, void* new_data);
typedef void (*DestroyDataCallback)(void* data);

// Hash Map
typedef struct HashMap HashMap;

HashMap* create_hashmap(size_t key_space);
void insert_data(HashMap* hm, const char* key, void* data, ResolveCollisionCallback resolve_collision);
void* get_data(HashMap* hm, const char* key);
void iterate(HashMap* hm, void (*callback)(const char* key, void* data));
void remove_data(HashMap* hm, const char* key, DestroyDataCallback destroy_data);
void delete_hashmap(HashMap* hm, DestroyDataCallback destroy_data);

// Conversion
//LinkedList hm_to_linkedlist();
//void** hm_to_array(HashMap* hm);

// Bucket
typedef struct Bucket Bucket;

int get_bucket_index(HashMap* hm, const char* key);
Bucket* get_bucket_list(HashMap* hm, const char* key);
Bucket* get_bucket_prev(HashMap* hm, Bucket* bucket);
Bucket* get_bucket(HashMap* hm, const char* key);
void add_bucket(HashMap* hm, Bucket* bucket);
void remove_bucket(HashMap* hm, Bucket* bucket, DestroyDataCallback destroy_data);
void remove_buckets(HashMap* hm, Bucket* bucket, DestroyDataCallback destroy_data);
void destroy_bucket(Bucket*, DestroyDataCallback destroy_data);

// Hash Function
typedef unsigned int (*HashFunction)(const char* key);
void set_hash_function(HashMap* hm, HashFunction hash_function);
unsigned int hash(const char* key);

// Since the library function is not included by the compiler
char *strdup (const char *s);

#endif
