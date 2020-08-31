#include <stdbool.h>

#include "ubarray.h"

#define BUCKET_SIZE 8

/*
 * For our purpose the only entry type we allow
 * is a string -> int pair.
 */
typedef struct
{
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

typedef struct
{
  size_t local_depth;
  size_t common_hash;
  ubarray *entries;
} bucket_t;

bucket_t *bucket_new(int local_depth, int common_hash);
void bucket_free(bucket_t *bucket);

typedef struct
{
  size_t global_depth;
  ubarray *buckets;
} hash_table_t;

typedef struct
{
  hash_table_t *table;
  size_t bucket_index;
  size_t entry_index;
} hash_table_iterator;

void hash_table_check(hash_table_t *table);
hash_table_t *hash_table_new();
unsigned long hash(const char *key);
bool hash_table_find(hash_table_t *table, const char *key, int *value);
bool hash_table_remove(hash_table_t *table, const char *key);
void hash_table_insert(hash_table_t *table, const char *key, int value);
void hash_table_free(hash_table_t *table);
hash_table_iterator *hash_table_iterate(hash_table_t *table);

entry_t *iterator_elem(hash_table_iterator *it);
void iterator_next(hash_table_iterator *it);
void iterator_free(hash_table_iterator *it);

void hash_table_serialize(hash_table_t *table, int out);
hash_table_t *hash_table_deserialize(int in);
