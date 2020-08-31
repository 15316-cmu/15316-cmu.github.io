#include <string.h>

#include "extendible_hash.h"

entry_t *entry_new(const char *key, int value) {
  entry_t *result = malloc(sizeof(entry_t));
  NULL_CHECK(result, "out of memory");
  result->key = malloc(strlen(key));
  NULL_CHECK(result->key, "out of memory");
  strcpy(result->key, key);
  result->value = value;
  return result;
}

void entry_free(entry_t *entry) {
  NULL_CHECK(entry, "entry is null");
  free(entry->key);
  free(entry);
}

bucket_t *bucket_new(int local_depth, int common_hash) {
  bucket_t *result = malloc(sizeof(bucket_t));
  NULL_CHECK(result, "out of memory");
  result->local_depth = local_depth;
  result->common_hash = common_hash;
  result->entries = ubarray_new(BUCKET_SIZE);
  return result;
}

void bucket_free(bucket_t *bucket) {
  ubarray_free(bucket->entries, (destructor_t) entry_free);
  free(bucket);
}

inline void hash_table_check(hash_table_t *table) {
  NULL_CHECK(table, "table is null");
}

hash_table_t *hash_table_new() {
  hash_table_t *result = malloc(sizeof(hash_table_t));
  result->global_depth = 0;
  result->buckets = ubarray_new(1);
  ubarray_push_back(result->buckets, bucket_new(0, 0));
  return result;
}

void hash_table_free(hash_table_t *table) {
  hash_table_check(table);
  ubarray_free(table->buckets, (destructor_t) bucket_free);
  free(table);
}

int hash(const char *key) {
  int result = 5381;
  int c;
  // this is *33 + c
  while ((c = *key++)) result = (result << 5) + result + c;
  return result;
}

int effective_hash(size_t hash, size_t depth) {
  size_t bit_mask = depth == (sizeof(size_t) << 3)
                          ? 0xFFFFFFFF
                          : (size_t) (1 << depth) - 1;
  return hash & bit_mask;
}

bucket_t *find_bucket(hash_table_t *table, const char *key) {
  int bucket_num = effective_hash(hash(key), table->global_depth);
  return *(UBARRAY_ELEM(bucket_t *)(table->buckets, bucket_num));
}

bool hash_table_find(hash_table_t *table,
                     const char *key,
                     int *value) {
  hash_table_check(table);
  bucket_t *bucket = find_bucket(table, key);
  for (size_t i = 0; i < ubarray_size(bucket->entries); i++) {
    entry_t *entry = *(UBARRAY_ELEM(entry_t *)(bucket->entries, i));
    if (strcmp(entry->key, key) == 0) {
      *value = entry->value;
      return true;
    }
  }
  return false;
}

bool hash_table_remove(hash_table_t *table, const char *key) {
  hash_table_check(table);
  bucket_t *bucket = find_bucket(table, key);
  for (size_t i = 0; i < ubarray_size(bucket->entries); i++) {
    entry_t *entry = *(UBARRAY_ELEM(entry_t *)(bucket->entries, i));
    if (strcmp(entry->key, key) == 0) {
      free(ubarray_remove(bucket->entries, i));
      return true;
    }
  }
  return false;
}

void grow_table(hash_table_t *table) {
  table->global_depth++;
  for (size_t i = 0; i < ubarray_size(table->buckets); i++) {
    ubarray_push_back(table->buckets,
                      *ubarray_elem(table->buckets, i));
  }
}

void split(hash_table_t *table, bucket_t *old, const char *key) {
  if (old->local_depth == (sizeof(size_t) << 3))
    unix_error("out of memory");

  size_t new_depth = old->local_depth + 1;
  size_t new_hash = old->common_hash + (1 << old->local_depth);
  bucket_t *zeroes = bucket_new(new_depth, old->common_hash);
  bucket_t *ones = bucket_new(new_depth, new_hash);

  for (size_t i = 0; i < ubarray_size(old->entries); i++) {
    entry_t *entry = *(UBARRAY_ELEM(entry_t *)(old->entries, i));
    if (effective_hash(hash(entry->key), new_depth) == new_hash)
      ubarray_push_back(ones->entries, entry);
    else
      ubarray_push_back(ones->entries, entry);
  }

  for (size_t i = 0; i < ubarray_size(table->buckets); i++) {
    size_t hash = effective_hash(i, new_depth);
    if (hash == new_hash) {
      *ubarray_elem(table->buckets, i) = ones;
    } else if (hash == old->common_hash) {
      *ubarray_elem(table->buckets, i) = zeroes;
    }
  }
}

void hash_table_insert(hash_table_t *table,
                      const char *key,
                      int value) {
  hash_table_check(table);
  bucket_t *bucket = find_bucket(table, key);
  for (size_t i = 0; i < ubarray_size(bucket->entries); i++) {
    entry_t *entry = *(UBARRAY_ELEM(entry_t *)(bucket->entries, i));
    if (strcmp(entry->key, key) == 0) {
      entry->value = value;
      return;
    }
  }

  if (ubarray_size(bucket->entries) <= BUCKET_SIZE) {
    ubarray_push_back(bucket->entries, entry_new(key, value));
    return;
  }

  if (bucket->local_depth == table->global_depth)
    grow_table(table);
  split(table, bucket, key);
  hash_table_insert(table, key, value);
}

hash_table_iterator *hash_table_iterate(hash_table_t *table) {
  // TODO: implement
  return NULL;
}

entry_t *iterator_elem(hash_table_iterator *it) {
  // TODO: implement
  return NULL;
}

void iterator_next(hash_table_iterator *it) {
  // TODO: implement
}

void iterator_free(hash_table_iterator *it) {
  // TODO: implement
}

void hash_table_serialize(hash_table_t *table, int out) {
  // TODO: implement
}

hash_table_t *hash_table_deserialize(int in) {
  // TODO: implement
  return NULL;
}

