#ifndef C9_ARRAY

#include <stdlib.h> // size_t
#include <string.h> // memcpy
#include "arena.h" // Arena, arena_fill

#if 0

Dynamic array implementation that has the following functions:
 - array_create: initializes the array with an item size and returns a pointer to it
 - array_create_width: initializes the array with an item size and a given index width and returns a pointer to it
 - array_push: adds an element to the array
 - array_pop: removes the last element from the array
 - array_get: returns the element at the given index
 - array_set: sets the element at the given index
 - array_length: returns the used size of the array
 - array_last: returns the last index of the array

The array index is implemented around a tree structure to allow for fast access and insertion. Each layer of the tree has a width of index_width and the depth is determined by the number of items in the array.

Consider an example where 'index_width' is 10. The tree would look something like this:

Layer 1: 0 1 2 3 4 5 6 7 8 9
Each of these nodes can have up to 10 children:

Layer 2: 0 1 2 3 4 5 6 7 8 9 (for each of the nodes in layer 1)
And each of those nodes can have up to 10 children, and so on:

Layer 3: 0 1 2 3 4 5 6 7 8 9 (for each of the nodes in layer 2)

The indexes are stored in reverse order, meaning that the largest indexes are deepest into the tree.

For example:
- The index 198 would be stored at positions 8 -> 9- > 1 in layers 1, 2, and 3
- The index 42 would be stored at positions 2 -> 4 in layers 1 and 2
- The index 1024 would be stored at positions 4 -> 2 -> 0 -> 1 in layers 1, 2, 3, and 4

The tree structure grows as more items are added to the array, adding more layers as needed.

The array index is stored in an arena allocator to allow for fast allocation and growth without needing to free memory on every pop or set operation. All items added to the array are copied to the array's arena, so the original data can be safely disposed of after adding it to the array.

#endif

const size_t DEFAULT_INDEX_WIDTH = 16;
const size_t INVALID_ARRAY_INDEX = -1;

typedef struct IndexNode IndexNode;
struct IndexNode {
  IndexNode **children; // Points to an array of children
  void *item;
};

typedef struct {
  Arena *arena;
  IndexNode *index; // Index tree of all items
  size_t length;
  size_t item_size;
  size_t index_width;
} Array;

// Create a new index node and return a pointer to it
static IndexNode *index_create(Arena *arena) {
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
static void index_set(index_set_params params) {
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
    // If the children node does not exist, create it
    if (params.indexNode->children == 0) {
      params.indexNode->children = (IndexNode **)arena_fill(params.arena, params.index_width * sizeof(IndexNode *));
      for (size_t i = 0; i < params.index_width; i++) {
        params.indexNode->children[i] = 0;
      }
    }
    // If the child indexNode at position digit does not exist, create it
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
static void *index_get(index_get_params params) {
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

// Create a new array width a given item size and index width and return a pointer to it
// Item size is the size of each item in the array
// Index width is the number of children each node can have.
// The optimal value is determined by the number of items in the array.
Array *array_create_width(Arena *arena, size_t item_size, size_t index_width) {
  Array *new_array = (Array *)arena_fill(arena, sizeof(Array));
  new_array->arena = arena;
  new_array->index = index_create(arena);
  new_array->length = 0;
  new_array->item_size = item_size;
  new_array->index_width = index_width;
  return new_array;
}

// Create a new default array and return a pointer to it
Array *array_create(Arena *arena, size_t item_size) {
  return array_create_width(arena, item_size, DEFAULT_INDEX_WIDTH);
}

// Copy data onto the array and add it to the last position
// The data has to be pushed by reference as that's the only type agnostic way to pass values
void array_push(Array *array, void *data) {
  void *item = arena_fill(array->arena, array->item_size);
  if (item == 0) return; // Allocation failed
  memcpy(item, data, array->item_size);

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
// The data has to be returned by reference as that's the only type agnostic way to return a value
void *array_pop(Array *array) {
  if (array->length == 0) return 0; // No items to pop
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
  if (index >= array->length) return 0;
  index_get_params get_params = {
    .indexNode = array->index,
    .index = index,
    .index_width = array->index_width
  };
  return index_get(get_params);
}

// Set the data at the given index starting from 0
void array_set(Array *array, size_t index, void *data) {
  if (index >= array->length) return;
  void *item = arena_fill(array->arena, array->item_size);
  if (item == 0) return; // Allocation failed
  memcpy(item, data, array->item_size);
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
// Warning! This returns a huge number (INVALID_ARRAY_INDEX) if the array is empty, as size_t can't be negative.
size_t array_last(Array *array) {
  // If the array is empty
  if (array->length == 0) return INVALID_ARRAY_INDEX;
  return array->length - 1;
}

#define C9_ARRAY
#endif