#ifndef C9_ARRAY

#include <math.h>   // log2, ceil, pow
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

const size_t DEFAULT_INDEX_WIDTH = 16;

typedef struct IndexNode IndexNode;
struct IndexNode {
  IndexNode **children; // Points to an array of children
  void *item;
};

typedef struct {
  Arena *arena;
  IndexNode *index; // Index tree of all items
  size_t length;
  size_t index_width;
} Array;

// Create a new index node and return a pointer to it
IndexNode *index_create(Arena *arena) {
  // printf("- array.h | Adding IndexNode: %zu\n", sizeof(IndexNode));
  IndexNode *index = (IndexNode *)arena_fill(arena, sizeof(IndexNode));
  index->children = 0;
  index->item = 0;
  return index;
}

typedef struct {
  Arena *arena;
  IndexNode *indexNode;
  size_t index;
  size_t index_width;
  void *item;
} index_set_params;

// Set item at the given index
void index_set(index_set_params params) {
  // If the indexNode is 0 there is nothing to add to
  if (params.indexNode == 0) {
    return;
  }
  // If the index is 0 we are at the leaf and should set the data
  else if (params.index == 0) {
    params.indexNode->item = params.item;
    return;
  }
  // Otherwise we need to go deeper into the tree
  else {
    size_t digit = params.index % params.index_width;
    size_t next_index = params.index / params.index_width;
    // If the child node does not exist, create it
    if (params.indexNode->children == 0) {
      params.indexNode->children = (IndexNode **)arena_fill(params.arena, params.index_width * sizeof(IndexNode *));
      for (size_t i = 0; i < params.index_width; i++) {
        params.indexNode->children[i] = 0;
      }
    }
    if (params.indexNode->children[digit] == 0) {
      params.indexNode->children[digit] = index_create(params.arena);
    }
    index_set_params next_params = {
      .arena = params.arena,
      .indexNode = params.indexNode->children[digit],
      .index = next_index,
      .index_width = params.index_width,
      .item = params.item
    };
    // Continue to the next node
    index_set(next_params);
  }
}

typedef struct {
  IndexNode *indexNode;
  size_t index;
  size_t index_width;
} index_get_params;

// Get the item at the given index
void *index_get(index_get_params params) {
  // If the indexNode is 0 there is nothing to get from
  if (params.indexNode == 0) {
    return 0;
  }
  // If the index is 0 we are at the leaf and should return the data
  else if (params.index == 0) {
    return params.indexNode->item;
  }
  // Otherwise we need to go deeper
  else {
    size_t digit = params.index % params.index_width;
    size_t next_index = params.index / params.index_width;
    index_get_params next_params = {
      .indexNode = params.indexNode->children[digit],
      .index = next_index,
      .index_width = params.index_width
    };
    return index_get(next_params);
  }
}

// Create a new array width a given index width and return a pointer to it
// Index width is the number of children each node can have.
// The optimal value is determined by the number of items in the array.
Array *array_create_width(Arena *arena, size_t index_width) {
  Array *array = (Array *)arena_fill(arena, sizeof(Array));
  array->arena = arena;
  array->index = index_create(arena);
  array->length = 0;
  array->index_width = index_width;
  return array;
}

// Create a new default array and return a pointer to it
Array *array_create(Arena *arena) {
  return array_create_width(arena, DEFAULT_INDEX_WIDTH);
}

// Copy data onto the array and add it to the last position
void array_push(Array *array, void *data, size_t data_size) {
  // printf("- array.h | Adding data: %zu\n", data_size);
  void *item = arena_fill(array->arena, data_size);
  memcpy(item, data, data_size);

  // Add the item to the index
  index_set_params set_params = {
    .arena = array->arena,
    .indexNode = array->index,
    .index = array->length,
    .index_width = array->index_width,
    .item = item
  };
  index_set(set_params);
  array->length += 1;
}

// Get the data from the last item and remove it from the array
void *array_pop(Array *array) {
  if (array->length == 0) {
    return 0;
  }
  array->length -= 1;
  // Get last item and remove it from the index
  index_get_params get_params = {
    .indexNode = array->index,
    .index = array->length,
    .index_width = array->index_width
  };
  void *data = index_get(get_params);
  index_set_params set_params = {
    .arena = array->arena,
    .indexNode = array->index,
    .index = array->length,
    .index_width = array->index_width,
    .item = 0
  };
  index_set(set_params);
  return data;
}

// Get the data at the given index starting from 0
void *array_get(Array *array, size_t index) {
  if (index >= array->length) {
    return 0;
  }
  index_get_params get_params = {
    .indexNode = array->index,
    .index = index,
    .index_width = array->index_width
  };
  return index_get(get_params);
}

// Set the data at the given index starting from 0
void array_set(Array *array, size_t index, void *data, size_t data_size) {
  if (index >= array->length) {
    return;
  }
  void *item = arena_fill(array->arena, data_size);
  memcpy(item, data, data_size);
  index_set_params set_params = {
    .arena = array->arena,
    .indexNode = array->index,
    .index = index,
    .index_width = array->index_width,
    .item = item
  };
  index_set(set_params);
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