#include <inttypes.h> // uint8_t, int32_t, int64_t, uint32_t, uint64_t
#include <stdlib.h>   // malloc, free, size_t

// Simple arena allocator that has the following functions:
// - a_open: initializes the arena and returns a pointer to it
// - a_fill: allocates memory in the arena and returns a pointer to it
// - a_close: frees all memory in the arena and all sub-arenas
// - a_reset: resets all heads in arena and sub-arenas to 0 without freeing memory
// - a_size: returns the used size of the arena and all sub-arenas
// - a_capacity: returns the total capacity of the arena and all sub-arenas

// If an arena is full it will create a new arena and link to it as a sub-arena
// When freeing an arena it will also free all sub-arenas
// Maximum arena size is 1GB

// Set MAX_ARENA_SIZE to the smallest of maximum size_t or 1GB
const size_t MAX_ARENA_SIZE = (size_t)-1 < 1024 * 1024 * 1024 ? (size_t)-1 : 1024 * 1024 * 1024;

typedef struct Arena {
  uint8_t *data;
  size_t head;
  size_t capacity;
  struct Arena *next;
} Arena;

// a_open creates a new arena with a size and returns a pointer to it
Arena *a_open(size_t size) {
  Arena *arena = (Arena *)malloc(sizeof(Arena));
  arena->data = (uint8_t *)malloc(size);
  arena->head = 0;
  arena->capacity = size;
  arena->next = 0;
  return arena;
}

// a_fill: allocates memory in the arena and returns a pointer to it
void *a_fill(Arena *arena, size_t size) {
  // If size is larger than 4, align to 8 bytes
  int8_t align_to = size > 4 ? 8 : 4;
  size_t aligned_head = arena->head;
  if (aligned_head % align_to != 0) {
    aligned_head = aligned_head + align_to - (aligned_head % align_to);
  }

  // If the size is 0 or bigger than the maximum arena size, return 0
  if (size == 0 || size > MAX_ARENA_SIZE) {
    return 0;
  }
  // If the current arena is full, use the next or create a new one
  if (aligned_head + size > arena->capacity) {
    if (arena->next == 0) {
      // Cap the arena size at MAX_ARENA_SIZE
      if (arena->capacity * 2 > MAX_ARENA_SIZE) {
        arena->next = a_open(MAX_ARENA_SIZE);
      } else {
        arena->next = a_open(arena->capacity * 2);
      }
    }
    return a_fill(arena->next, size);
  }
  // Point to the start of the aligned memory block and move the head
  void *ptr = arena->data + aligned_head;
  arena->head = aligned_head + size;
  return ptr;
}

// a_close: frees all memory in the arena and all sub-arenas
void a_close(Arena *arena) {
  if (arena->next != 0) {
    a_close(arena->next);
  }
  free(arena->data);
  free(arena);
}

// a_reset: resets all heads in arena and sub-arenas to 0 without freeing memory
void a_reset(Arena *arena) {
  arena->head = 0;
  if (arena->next != 0) {
    a_reset(arena->next);
  }
}

// a_size: returns the used size of the arena and all sub-arenas
size_t a_size(Arena *arena) {
  if (arena->next == 0) {
    return arena->head;
  }
  return arena->head + a_size(arena->next);
}

// a_capacity: returns the total capacity of the arena and all sub-arenas
size_t a_capacity(Arena *arena) {
  if (arena->next == 0) {
    return arena->capacity;
  }
  return arena->capacity + a_capacity(arena->next);
}