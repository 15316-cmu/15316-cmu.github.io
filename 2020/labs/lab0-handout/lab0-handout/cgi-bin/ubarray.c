#include <stdlib.h>

#include "ubarray.h"

inline void ubarray_check(const ubarray *array) {
  NULL_CHECK(array, "ubarray is null");
}

ubarray *ubarray_new(size_t size) {
  ubarray *result = malloc(sizeof(ubarray));
  NULL_CHECK(result, "out of memory");
  result->size = 0;
  result->limit = size;
  result->data = calloc(size, sizeof(void *));
  return result;
}

elem_t *ubarray_elem(ubarray *array, size_t index) {
  ubarray_check(array);
  CHECK(index >= 0 && index < array->size,
          "index access out of bounds");
  return array->data + index;
}

size_t ubarray_size(const ubarray *array) {
  ubarray_check(array);
  return array->size;
}

ubarray *ubarray_push_back(ubarray *array, elem_t elem) {
  ubarray_check(array);

  array->data[array->size] = elem;
  if (array->size == array->limit) {
    array->limit <<= 1;
    void **new_data = calloc(array->limit, sizeof(void *));
    NULL_CHECK(new_data, "out of memory");
    for (size_t i = 0; i < array->size; i++)
      new_data[i] = array->data[i];
    free(array->data);
    array->data = new_data;
  }
  return array;
}

elem_t ubarray_remove(ubarray *array, int index) {
  ubarray_check(array);
  elem_t result = array->data[index];
  array->size--;
  for (size_t i = index; i < array->size; i++)
    array->data[i] = array->data[i + 1];
  return result;
}

void ubarray_free(ubarray *array, destructor_t destructor) {
  ubarray_check(array);

  for (size_t i = 0; i < array->size; i++)
    destructor(array->data[i]);
  free(array->data);
  free(array);
}

