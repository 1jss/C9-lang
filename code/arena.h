#ifndef C9_ARENA

#include <stdlib.h> // malloc, free
#include "types.h" // u8, i32

#if 0

Simple arena allocator that has the following functions:
- arena_open: initializes the arena and returns a pointer to it
- arena_fill: allocates memory in the arena and returns a pointer to it
- arena_close: frees all memory in the arena and all sub-arenas
- arena_reset: resets all heads in arena and sub-arenas to 0 without freeing memory
- arena_size: returns the used size of the arena and all sub-arenas
- arena_capacity: returns the total capacity of the arena and all sub-arenas

The arena is implemented as a linked list of arenas, where each arena has a pointer to the next arena. This lets the arena size grow dynamically. The size of the first arena is set at creation. When the first arena is full it will create a new arena of double the size and link to it. The size of an individual subsequent arena is capped at 1GB, which means that the largest object that can be stored in the arena is 1GB.

When freeing an arena it will also free all sub-arenas. This encourages the use of smaller arenas for temporary allocations and larger arenas for more permanent allocations.

#endif

// Set MAX_ARENA_SIZE to 1GB
const i32 MAX_ARENA_SIZE = 1024 * 1024 * 1024;

typedef struct Arena {
  u8 *data;
  i32 head;
  i32 capacity;
  struct Arena *next;
} Arena;

// arena_open creates a new arena with a size and returns a pointer to it
Arena *arena_open(i32 size) {
  Arena *arena = (Arena *)malloc(sizeof(Arena));
  arena->data = (u8 *)malloc(size);
  arena->head = 0;
  arena->capacity = size;
  arena->next = 0;
  return arena;
}

// arena_fill: allocates memory in the arena and returns a pointer to it
void *arena_fill(Arena *arena, i32 size) {
  // If size is larger than 4, align to 8 bytes
  u8 align_to = size > 4 ? 8 : 4;
  i32 aligned_head = arena->head;
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
        arena->next = arena_open(MAX_ARENA_SIZE);
      } else {
        arena->next = arena_open(arena->capacity * 2);
      }
    }
    return arena_fill(arena->next, size);
  }
  // Point to the start of the aligned memory block and move the head
  void *ptr = arena->data + aligned_head;
  arena->head = aligned_head + size;
  return ptr;
}

// arena_close: frees all memory in the arena and all sub-arenas
void arena_close(Arena *arena) {
  if (arena->next != 0) {
    arena_close(arena->next);
  }
  free(arena->data);
  free(arena);
}

// arena_reset: resets all heads in arena and sub-arenas to 0 without freeing memory
void arena_reset(Arena *arena) {
  arena->head = 0;
  if (arena->next != 0) {
    arena_reset(arena->next);
  }
}

// arena_size: returns the used size of the arena and all sub-arenas
i32 arena_size(Arena *arena) {
  if (arena->next == 0) {
    return arena->head;
  }
  return arena->head + arena_size(arena->next);
}

// arena_capacity: returns the total capacity of the arena and all sub-arenas
i32 arena_capacity(Arena *arena) {
  if (arena->next == 0) {
    return arena->capacity;
  }
  return arena->capacity + arena_capacity(arena->next);
}

#define C9_ARENA
#endif