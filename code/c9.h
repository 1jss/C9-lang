#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

typedef struct {
  uint8_t *data;
  size_t len;
} t8;

t8 text(char *s) {
  uint32_t len = 0;
  while (s[len] != '\0') {
    len++;
  }
  return (t8){(uint8_t *)s, len};
}

void print_i32(i32 value) { printf("%" PRId32, value); }
void print_i64(i64 value) { printf("%" PRId64, value); }
void print_u32(u32 value) { printf("%" PRIu32, value); }
void print_u64(u64 value) { printf("%" PRIu64, value); }
void print_f32(f32 value) { printf("%f", value); }
void print_f64(f64 value) { printf("%f", value); }
void print_bool(bool value) { printf("%d", value); }
void print_string(char *value) { printf("%s", value); }
void print_text(t8 value) {
  for (size_t i = 0; i < value.len; i++) {
    printf("%c", value.data[i]);
  }
}

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