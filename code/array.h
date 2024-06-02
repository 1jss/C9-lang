#ifndef C9_ARRAY

#include <stdlib.h> // size_t
#include <string.h> // memcpy

#include "arena.h" // Arena, a_open, a_fill, a_reset, a_close

// Simple dynamic array implementation that has the following functions:
// - array_create: initializes the array and returns a pointer to it
// - array_destroy: frees all memory in the array
// - array_push: adds an element to the array
// - array_pop: removes the last element from the array
// - array_get: returns the element at the given index
// - array_set: sets the element at the given index
// - array_length: returns the used size of the array

typedef struct ArrayItem {
  void *data;
  struct ArrayItem *next;
} ArrayItem;

typedef struct {
  ArrayItem *items;
  Arena *arena;
  size_t length;
} Array;

// Create a new array and return it
Array array_create() {
  Array array;
  array.arena = a_open(1024);
  array.items = 0;
  array.length = 0;
  return array;
}

// Free all memory in the array
void array_destroy(Array *array) {
  a_close(array->arena);
  array->items = 0;
  array->length = 0;
}

// Copy data onto the arena and add it to the array
void array_push(Array *array, void *data, size_t data_size) {
  void *data_copy = a_fill(array->arena, data_size);
  memcpy(data_copy, data, data_size);
  ArrayItem *item = (ArrayItem *)a_fill(array->arena, sizeof(ArrayItem));
  item->data = data_copy;
  item->next = array->items; // Put the current items at the next pointer
  array->items = item;
  array->length++;
}

// Get the data from the first item and remove it from the list
void *array_pop(Array *array) {
  if (array->length == 0) {
    return 0;
  }
  ArrayItem *item = array->items;
  void *data = item->data;
  array->items = item->next;
  array->length--;
  return data;
}

// Get the data from the item at the given index starting from 0
void *array_get(Array *array, size_t index) {
  if (index >= array->length) {
    return 0;
  }
  ArrayItem *item = array->items;
  for (size_t i = 0; i < index; i++) {
    item = item->next;
  }
  return item->data;
}

// Set the data at the item at the given index starting from 0
void array_set(Array *array, size_t index, void *data) {
  if (index >= array->length) {
    return;
  }
  ArrayItem *item = array->items;
  for (size_t i = 0; i < index; i++) {
    item = item->next;
  }
  item->data = data;
}

// Return the used size of the array
size_t array_length(Array *array) {
  return array->length;
}

#define C9_ARRAY
#endif