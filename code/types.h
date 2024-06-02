#ifndef C9_TYPES

#include <inttypes.h> // uint8_t, int32_t, int64_t, uint32_t, uint64_t
#include <stdbool.h>  // bool
#include <stddef.h>   // size_t
#include <stdio.h>    // printf

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

typedef struct {
  uint8_t *data;
  size_t len;
} s8;

s8 str8(char *s) {
  uint32_t len = 0;
  while (s[len] != '\0') {
    len++;
  }
  return (s8){(uint8_t *)s, len};
}

void print_i32(i32 value) { printf("%" PRId32, value); }
void print_i64(i64 value) { printf("%" PRId64, value); }
void print_u32(u32 value) { printf("%" PRIu32, value); }
void print_u64(u64 value) { printf("%" PRIu64, value); }
void print_f32(f32 value) { printf("%f", value); }
void print_f64(f64 value) { printf("%f", value); }
void print_bool(bool value) { printf("%d", value); }
void print_string(char *value) { printf("%s", value); }
void print_s8(s8 value) {
  for (size_t i = 0; i < value.len; i++) {
    printf("%c", value.data[i]);
  }
}

#define C9_TYPES
#endif