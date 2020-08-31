#include <stdbool.h>

#include "ubarray.h"

#define BUCKET_SIZE 8

/*
 * For our purpose the only entry type we allow
 * is a string -> int pair.
 */
typedef struct {
  char *key;
  int value;
} entry_t;

/*
 * Returns a pointer to a new entry, copying the
 * string given.
 */
entry_t *entry_new(const char *key, int value);
/*
 * Frees the entry_t at entry. Up to caller to
 * ensure no dangling pointer to key is around.
 */
void entry_free(entry_t *entry);

typedef struct {
  size_t local_depth;
  size_t common_hash;
  ubarray *entries;
} bucket_t;

/*
 * Returns a pointer to a new bucket with the given local
 * depth, holding elements hashing to common_hash
 */
bucket_t *bucket_new(int local_depth, int common_hash);
/*
 * Frees the bucket_t at bucket.
 */
void bucket_free(bucket_t *bucket);

typedef struct {
  size_t global_depth;
  ubarray *buckets;
} hash_table_t;

typedef struct {
  hash_table_t *table;
  size_t bucket_index;
  size_t entry_index;
} hash_table_iterator;

/*
 * Check that the given hash table is valid.
 */
void hash_table_check(hash_table_t *table);
/*
 * Returns a pointer to a new hash table.
 */
hash_table_t *hash_table_new();
/*
 * Hashes a string into an integer
 */
int hash(const char *key);
/*
 * Finds if there is an element with key equal to the one given.
 *
 * Returns true and set value to mapped value if there is such an
 * element, or false if no such element exists.
 */
bool hash_table_find(hash_table_t *table, const char *key, int *value);
/*
 * Inserts the element key: value into the hashtable, overwriting the 
 * original value if a mapping already exists.
 */
void hash_table_insert(hash_table_t *table, const char *key, int value);
/*
 * Removes the element with key equal to the one given. Returns false if
 * no such element exists.
 */
bool hash_table_remove(hash_table_t *table, const char *key);
/*
 * Frees the hash table.
 */
void hash_table_free(hash_table_t *table);

/*
 * Returns a pointer to an iterator to the hash table.
 *
 * Behavior is undefined if the hash table is modified while
 * operations are being done to the iterator.
 */
hash_table_iterator *hash_table_iterate(hash_table_t *table);
/*
 * Returns a pointer to the entry the iterator currently points to,
 * or null if no more elements are available.
 */
entry_t *iterator_elem(hash_table_iterator *it);
/*
 * Advances the iterator by one.
 */
void iterator_next(hash_table_iterator *it);
/*
 * Fress the iterator
 */
void iterator_free(hash_table_iterator *it);

/* 
 * Serializes the hash_table out to the fd out.
 */
void hash_table_serialize(hash_table_t *table, int out);
/*
 * Reads in a hash table from fd in.
 *
 * This function should fail and exit if the file is malformed.
 */
hash_table_t *hash_table_deserialize(int in);
