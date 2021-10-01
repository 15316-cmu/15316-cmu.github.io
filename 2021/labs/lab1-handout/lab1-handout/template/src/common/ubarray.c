#include <stdlib.h>

#include "common/safemem.h"
#include "common/ubarray.h"

#define malloc(n) safe_malloc(n)
#define calloc(n, s) safe_calloc(n, s)
#define free(p) safe_free(p)

inline void ubarray_check(const ubarray *array) {
  NULL_CHECK(array, "ubarray is null");
  NULL_CHECK(array->data, "ubarray data field is corrupted");
  CHECK(array->size < array->limit, "ubarray size is corrupted");
}

ubarray *ubarray_new(size_t size) {
  ubarray *result = malloc(sizeof(ubarray));
  NULL_CHECK(result, "[ubarray_new]: out of memory");
  result->size = 0;
  result->limit = size ? size : 1;
  result->data = calloc(result->limit, sizeof(void *));
  NULL_CHECK(result->data, "[ubarray_new]: out of memory");
  return result;
}

elem_t *ubarray_elem(ubarray *array, size_t index) {
  ubarray_check(array);
  CHECK(index >= 0 && index < array->size,
          "[ubarray_elem]: index access out of bounds");
  return array->data + index;
}

size_t ubarray_size(const ubarray *array) {
  ubarray_check(array);
  return array->size;
}

ubarray *ubarray_push_back(ubarray *array, elem_t elem) {
  ubarray_check(array);

  array->data[array->size] = elem;
  array->size++;
  if (array->size == array->limit) {
    array->limit <<= 1;
    void **new_data = calloc(array->limit, sizeof(void *));
    NULL_CHECK(new_data, "[ubarray_push_back]: out of memory");
    for (size_t i = 0; i < array->size; i++)
      new_data[i] = array->data[i];
    free(array->data);
    array->data = new_data;
  }
  return array;
}

elem_t ubarray_remove(ubarray *array, int index) {
  ubarray_check(array);
  CHECK(index >= 0 && index < array->size,
        "[ubarray_remove]: index access out of bounds");

  elem_t result = array->data[index];
  array->size--;
  for (size_t i = index; i <= array->size; i++)
    array->data[i] = array->data[i + 1];
  return result;
}

void ubarray_free(ubarray *array, destructor_t destructor) {
  ubarray_check(array);
  NULL_CHECK(destructor, "[ubarray_free]: destructor is null");

  for (size_t i = 0; i < array->size; i++)
    destructor(array->data[i]);
  free(array->data);
  free(array);
}


