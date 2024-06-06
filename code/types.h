#ifndef C9_TYPES

#include <inttypes.h> // uint8_t, int32_t, int64_t, uint32_t, uint64_t
#include <stdbool.h>  // bool
#include <stddef.h>   // size_t
#include <stdio.h>    // printf

#if 0

This header defines a set of standard types. The type names are similar to those of Rust and Zig.

#endif

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
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

#define C9_TYPES
#endif