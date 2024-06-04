#ifndef C9_ARRAY

#include <stdlib.h> // size_t
#include <string.h> // memcpy

#include "arena.h" // Arena, arena_open, arena_fill, arena_close

// Dynamic array implementation that has the following functions:
// - array_create: initializes the array and returns a pointer to it
// - array_destroy: frees all memory in the array
// - array_push: adds an element to the array
// - array_pop: removes the last element from the array
// - array_get: returns the element at the given index
// - array_set: sets the element at the given index
// - array_length: returns the used size of the array
// - array_last: returns the last index of the array

const size_t INDEX_WIDTH = 16;

typedef struct IndexNode IndexNode;
struct IndexNode {
  IndexNode **children; // Points to an array of children
  void *item;
};

typedef struct {
  Arena *arena;
  IndexNode *index; // Index tree of all items
  size_t length;
} Array;

// Create a new index node and return a pointer to it
IndexNode *index_create(Arena *arena) {
  // printf("- array.h | Adding IndexNode: %zu\n", sizeof(IndexNode));
  IndexNode *index = (IndexNode *)arena_fill(arena, sizeof(IndexNode));
  index->children = 0;
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
    if (indexNode->children == 0) {
      // printf("- array.h | Adding IndexNode[]: %zu\n", INDEX_WIDTH * sizeof(IndexNode));
      indexNode->children = (IndexNode **)arena_fill(arena, INDEX_WIDTH * sizeof(IndexNode *));
      for (size_t i = 0; i < INDEX_WIDTH; i++) {
        indexNode->children[i] = 0;
      }
    }
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

// Create a new array and return a pointer to it
Array *array_create(Arena *arena) {
  // printf("- array.h | Adding Array: %zu\n", sizeof(Array));
  Array *array = (Array *)arena_fill(arena, sizeof(Array));
  array->arena = arena;
  array->index = index_create(arena);
  array->length = 0;
  return array;
}

// Copy data onto the array and add it to the last position
void array_push(Array *array, void *data, size_t data_size) {
  // printf("- array.h | Adding data: %zu\n", data_size);
  void *item = arena_fill(array->arena, data_size);
  memcpy(item, data, data_size);

  // Add the item to the index
  index_set(array->arena, array->index, array->length, item);
  array->length += 1;
}

// Get the data from the last item and remove it from the array
void *array_pop(Array *array) {
  if (array->length == 0) {
    return 0;
  }
  array->length -= 1;
  // Get last item and remove it from the index
  void *data = index_get(array->index, array->length);
  index_set(array->arena, array->index, array->length, 0);
  return data;
}

// Get the data at the given index starting from 0
void *array_get(Array *array, size_t index) {
  if (index >= array->length) {
    return 0;
  }
  return index_get(array->index, index);
}

// Set the data at the given index starting from 0
void array_set(Array *array, size_t index, void *data, size_t data_size) {
  if (index >= array->length) {
    return;
  }
  void *item = arena_fill(array->arena, data_size);
  memcpy(item, data, data_size);
  index_set(array->arena, array->index, index, item);
}

// Return the used size of the array
size_t array_length(Array *array) {
  return array->length;
}

// Get last index of the array
size_t array_last(Array *array) {
  // If the array is empty return -1
  if (array->length == 0) {
    return -1;
  }
  return array->length - 1;
}

#define C9_ARRAY
#endif