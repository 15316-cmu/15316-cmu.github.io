#include <string.h>

#include "common/safemem.h"
#include "common/extendible_hash.h"

#define malloc(n) safe_malloc(n)
#define calloc(n, s) safe_calloc(n, s)
#define free(p) safe_free(p)

entry_t *entry_new(const char *key, int value)
{
  entry_t *result = malloc(sizeof(entry_t));
  NULL_CHECK(result, "[entry_new]: out of memory");
  result->key = malloc(strnlen(key, MAXLINE) + 1);
  NULL_CHECK(result->key, "[entry_new]: out of memory");
  strncpy(result->key, key, strnlen(key, MAXLINE) + 1);
  result->value = value;
  return result;
}

void entry_free(entry_t *entry)
{
  NULL_CHECK(entry, "[entry_free]: entry is null");
  free(entry->key);
  free(entry);
}

bucket_t *bucket_new(int local_depth, int common_hash)
{
  bucket_t *result = malloc(sizeof(bucket_t));
  NULL_CHECK(result, "[bucket_new]: out of memory");
  result->local_depth = local_depth;
  result->common_hash = common_hash;
  result->entries = ubarray_new(BUCKET_SIZE);
  return result;
}

void bucket_free(bucket_t *bucket)
{
  NULL_CHECK(bucket, "[bucket_free]: bucket is null");
  ubarray_free(bucket->entries, (destructor_t)entry_free);
  free(bucket);
}

inline void hash_table_check(hash_table_t *table)
{
  NULL_CHECK(table, "[hash_table_check]: table is null");
}

hash_table_t *hash_table_new()
{
  hash_table_t *result = malloc(sizeof(hash_table_t));
  NULL_CHECK(result, "[hash_table_new]: out of memory");
  result->global_depth = 0;
  result->buckets = ubarray_new(1);
  ubarray_push_back(result->buckets, bucket_new(0, 0));
  return result;
}

void hash_table_free(hash_table_t *table)
{
  hash_table_check(table);
  ubarray_free(table->buckets, (destructor_t)bucket_free);
  free(table);
}

unsigned long hash(const char *key)
{
  unsigned long result = 5381;
  int c;
  while ((c = *key++))
    result = (result << 5) + result + c;
  return result;
}

int effective_hash(size_t hash, size_t depth)
{
  size_t bit_mask = depth == (sizeof(size_t) << 3)
                        ? 0xFFFFFFFF
                        : (size_t)(1 << depth) - 1;
  return hash & bit_mask;
}

bucket_t *find_bucket(hash_table_t *table, const char *key)
{
  int bucket_num = effective_hash(hash(key), table->global_depth);
  return *(UBARRAY_ELEM(bucket_t *)(table->buckets, bucket_num));
}

bool hash_table_find(hash_table_t *table,
                     const char *key,
                     int *value)
{
  hash_table_check(table);
  bucket_t *bucket = find_bucket(table, key);
  for (size_t i = 0; i < ubarray_size(bucket->entries); i++)
  {
    entry_t *entry = *(UBARRAY_ELEM(entry_t *)(bucket->entries, i));
    if (strcmp(entry->key, key) == 0)
    {
      *value = entry->value;
      return true;
    }
  }
  return false;
}

bool hash_table_remove(hash_table_t *table, const char *key)
{
  hash_table_check(table);
  bucket_t *bucket = find_bucket(table, key);
  for (size_t i = 0; i < ubarray_size(bucket->entries); i++)
  {
    entry_t *entry = *(UBARRAY_ELEM(entry_t *)(bucket->entries, i));
    if (strcmp(entry->key, key) == 0)
    {
      free(ubarray_remove(bucket->entries, i));
      return true;
    }
  }
  return false;
}

void grow_table(hash_table_t *table)
{
  table->global_depth++;
  size_t curr_size = ubarray_size(table->buckets);
  for (size_t i = 0; i < curr_size; i++)
  {
    ubarray_push_back(table->buckets,
                      *ubarray_elem(table->buckets, i));
  }
}

void split(hash_table_t *table, bucket_t *old, const char *key)
{
  if (old->local_depth == (sizeof(size_t) << 3))
    unix_error("[split]: out of memory");

  size_t new_depth = old->local_depth + 1;
  size_t new_hash = old->common_hash + (1 << old->local_depth);
  bucket_t *zeroes = bucket_new(new_depth, old->common_hash);
  bucket_t *ones = bucket_new(new_depth, new_hash);

  for (size_t i = 0; i < ubarray_size(old->entries); i++)
  {
    entry_t *entry = *(UBARRAY_ELEM(entry_t *)(old->entries, i));
    if (effective_hash(hash(entry->key), new_depth) == new_hash)
      ubarray_push_back(ones->entries, entry);
    else
      ubarray_push_back(zeroes->entries, entry);
  }

  for (size_t i = 0; i < ubarray_size(table->buckets); i++)
  {
    size_t hash = effective_hash(i, new_depth);
    if (hash == new_hash)
    {
      *ubarray_elem(table->buckets, i) = ones;
    }
    else if (hash == old->common_hash)
    {
      *ubarray_elem(table->buckets, i) = zeroes;
    }
  }
}

void hash_table_insert(hash_table_t *table,
                       const char *key,
                       int value)
{
  hash_table_check(table);
  bucket_t *bucket = find_bucket(table, key);
  for (size_t i = 0; i < ubarray_size(bucket->entries); i++)
  {
    entry_t *entry = *(UBARRAY_ELEM(entry_t *)(bucket->entries, i));
    if (strcmp(entry->key, key) == 0)
    {
      entry->value = value;
      return;
    }
  }

  if (ubarray_size(bucket->entries) < BUCKET_SIZE)
  {
    ubarray_push_back(bucket->entries, entry_new(key, value));
    return;
  }

  if (bucket->local_depth == table->global_depth)
    grow_table(table);
  split(table, bucket, key);
  hash_table_insert(table, key, value);
}

hash_table_iterator *hash_table_iterate(hash_table_t *table)
{
  hash_table_check(table);

  hash_table_iterator *result = malloc(sizeof(hash_table_iterator));
  NULL_CHECK(result, "[hash_table_iterate]: out of memory");
  result->table = table;
  result->bucket_index = 0;
  result->entry_index = 0;
  return result;
}

void iterator_check(hash_table_iterator *it)
{
  NULL_CHECK(it, "[extendible_hash iterator check]: iterator is null");
  hash_table_check(it->table);
}

entry_t *iterator_elem(hash_table_iterator *it)
{
  iterator_check(it);

  if (it->bucket_index >= ubarray_size(it->table->buckets))
    return NULL;
  bucket_t *bucket = *(UBARRAY_ELEM(bucket_t *)(it->table->buckets, it->bucket_index));
  if (it->entry_index >= ubarray_size(bucket->entries))
    return NULL;
  return *(UBARRAY_ELEM(entry_t *)(bucket->entries, it->entry_index));
}

void iterator_next(hash_table_iterator *it)
{
  iterator_check(it);
  size_t num_buckets = 1 << it->table->global_depth;
  while (it->bucket_index < num_buckets)
  {
    bucket_t *bucket = *(UBARRAY_ELEM(bucket_t *)(it->table->buckets, it->bucket_index));
    if (it->entry_index >= ubarray_size(bucket->entries) - 1)
    {
      it->bucket_index++;
      it->entry_index = 0;
      break;
    }
    else
    {
      it->entry_index++;
      break;
    }
  }
}

void iterator_free(hash_table_iterator *it)
{
  iterator_check(it);
  free(it);
}

void hash_table_serialize(hash_table_t *table, int out)
{
  hash_table_check(table);
  hash_table_iterator *it = hash_table_iterate(table);
  entry_t *entry = iterator_elem(it);
  while (entry != NULL)
  {
    dprintf(out, "%s %d\n", entry->key, entry->value);
    iterator_next(it);
    entry = iterator_elem(it);
  }
}

hash_table_t *hash_table_deserialize(int in)
{
  char read_buf[MAXLINE];
  char str_buf[MAXLINE];
  int value;
  rio_t rp;
  Rio_readinitb(&rp, in);
  hash_table_t *result = hash_table_new();

  while (Rio_readlineb(&rp, read_buf, MAXLINE) != 0)
  {
    if (sscanf(read_buf, "%s %d\n", str_buf, &value) < 2)
      unix_error("[hash_table_deserialize]: malformed file");
    hash_table_insert(result, str_buf, value);
  }
  return result;
}
