#ifndef C9_ARRAY

#include <stdlib.h> // size_t
#include <string.h> // memcpy

#include "arena.h" // Arena, a_open, a_fill, a_close

// Simple dynamic array implementation that has the following functions:
// - array_create: initializes the array and returns a pointer to it
// - array_destroy: frees all memory in the array
// - array_push: adds an element to the array
// - array_pop: removes the last element from the array
// - array_get: returns the element at the given index
// - array_set: sets the element at the given index
// - array_length: returns the used size of the array

const size_t INDEX_WIDTH = 10;

typedef struct ArrayItem {
  void *data;
  struct ArrayItem *next;
} ArrayItem;

typedef struct IndexNode IndexNode;
struct IndexNode {
  IndexNode *children[INDEX_WIDTH];
  ArrayItem *item;
};

typedef struct {
  Arena *arena;
  ArrayItem *items; // Linked list of items
  IndexNode *index; // Index tree of items
  size_t length;
} Array;

// Create a new index node and return it
IndexNode *index_create(Arena *arena) {
  IndexNode *index = (IndexNode *)a_fill(arena, sizeof(IndexNode));
  for (size_t i = 0; i < INDEX_WIDTH; i++) {
    index->children[i] = 0;
  }
  index->item = 0;
  return index;
}

// Set item at the given index
void index_set(Arena *arena, IndexNode *indexNode, size_t index, void *item) {
  // If the indexNode is 0 there is nothing to add to
  if (indexNode == 0) {
    return;
  }
  // If the index is 0 we are at the leaf and should set the data
  else if (index == 0) {
    indexNode->item = item;
    return;
  }
  // Otherwise we need to go deeper into the tree
  else {
    size_t digit = index % INDEX_WIDTH;
    size_t next_index = index / INDEX_WIDTH;
    // If the child node does not exist, create it
    if (indexNode->children[digit] == 0) {
      indexNode->children[digit] = index_create(arena);
    }
    index_set(arena, indexNode->children[digit], next_index, item);
  }
}

// Get the item at the given index
void *index_get(IndexNode *indexNode, size_t index) {
  // If the indexNode is 0 there is nothing to get from
  if (indexNode == 0) {
    return 0;
  }
  // If the index is 0 we are at the leaf and should return the data
  else if (index == 0) {
    return indexNode->item;
  }
  // Otherwise we need to go deeper
  else {
    size_t digit = index % INDEX_WIDTH;
    size_t next_index = index / INDEX_WIDTH;
    return index_get(indexNode->children[digit], next_index);
  }
}

// Refill the entire index based on the items in the linked list
void index_recreate(Array *array) {
  ArrayItem *item = array->items;
  size_t index = 0;
  while (item != 0) {
    index_set(array->arena, array->index, index, item);
    item = item->next;
    index++;
  }
}

// Create a new array and return it
Array array_create(Arena *arena) {
  Array array;
  array.arena = arena;
  array.items = 0;
  array.index = index_create(arena);
  array.length = 0;
  return array;
}

// Copy data onto the array and add it to the last position
void array_push(Array *array, void *data, size_t data_size) {
  void *data_copy = a_fill(array->arena, data_size);
  memcpy(data_copy, data, data_size);
  ArrayItem *item = (ArrayItem *)a_fill(array->arena, sizeof(ArrayItem));
  item->data = data_copy;
  item->next = 0;

  if (array->items == 0) {
    array->items = item;
  } else {
    // get last item from index
    ArrayItem *lastItem = index_get(array->index, array->length - 1);
    if (lastItem != 0) {
      lastItem->next = item;
    }
  }
  // Add the item to the index
  index_set(array->arena, array->index, array->length, item);
  array->length += 1;
}

// Get the data from the last item and remove it from the array
void *array_pop(Array *array) {
  if (array->length == 0) {
    return 0;
  }
  void *data = 0;
  // If there is only one item
  if (array->length == 1) {
    data = array->items->data;
    array->items = 0;
  } else {
    // Get the last item and the second to last item
    ArrayItem *lastItem = index_get(array->index, array->length - 1);
    data = lastItem->data;
    ArrayItem *newLastItem = index_get(array->index, array->length - 2);
    newLastItem->next = 0;
  }
  // Remove the popped item from the index
  array->length -= 1;
  index_set(array->arena, array->index, array->length, 0);
  return data;
}

// Get the data from the first item and remove it from the array
void *array_shift(Array *array) {
  if (array->length == 0) {
    return 0;
  }
  ArrayItem *item = array->items;
  void *data = item->data;
  array->items = item->next;
  array->length -= 1;
  // Shifts the entire index tree by one
  index_recreate(array);
  return data;
}

// Get the data from the item at the given index starting from 0
void *array_get(Array *array, size_t index) {
  if (index >= array->length) {
    return 0;
  }
  ArrayItem *item = index_get(array->index, index);
  return item->data;
}

// Set the data at the item at the given index starting from 0
void array_set(Array *array, size_t index, void *data) {
  if (index >= array->length) {
    return;
  }
  ArrayItem *item = index_get(array->index, index);
  item->data = data;
}

// Return the used size of the array
size_t array_length(Array *array) {
  return array->length;
}

#define C9_ARRAY
#endif