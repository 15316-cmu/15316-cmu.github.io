#include <stdlib.h>

#include "csapp.h"

typedef void *elem_t;
typedef void(*destructor_t)(void *);
typedef void(*serializer_t)(int, void *);
typedef void *(*deserializer_t)(int);

/*
 * Syntactic sugar for accessing ubarray and then type cast,
 * similar to c++ template. E.g.
 *
 * int **elem = UBARRAY_ELEM(int *)(array, 0);
 */
#define UBARRAY_ELEM(type) (type *) ubarray_elem

typedef struct {
  size_t limit;
  size_t size;
  elem_t *data;
} ubarray;

void ubarray_check(const ubarray *array);
/*
 * Returns a pointer to a new ubarray, with initial size
 * set to size.
 */
ubarray *ubarray_new(size_t size);
/*
 * Returns a pointer to the element of array at index.
 *
 * Index must be smaller than the size of the array.
 */
elem_t *ubarray_elem(ubarray *array, size_t index);
/*
 * Returns the number of elements in the array currently.
 */
size_t ubarray_size(const ubarray *array);
/*
 * Adds element elem to the back of the array, increasing
 * its size by 1.
 */
ubarray *ubarray_push_back(ubarray *array, elem_t elem);
/*
 * Removes the element at index in array. All subsequent
 * elements are copied forward by 1 so that the array is
 * contiguous and the total size is decreased by 1.
 *
 * Index must be smaller than the size of the array.
 */
elem_t ubarray_remove(ubarray *array, int index);
/*
 * Frees the array data structure at array, using
 * the provided destructor to free remaining contents
 * in the array.
 */
void ubarray_free(ubarray *arrray, destructor_t destructor);

