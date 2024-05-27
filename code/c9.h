#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

typedef struct {
  uint8_t *data;
  size_t len;
} u8;

u8 text(char *s) {
  uint32_t len = 0;
  while (s[len] != '\0') {
    len++;
  }
  return (u8){(uint8_t *)s, len};
}

void print_i32(i32 value) { printf("%" PRId32, value); }
void print_i64(i64 value) { printf("%" PRId64, value); }
void print_u32(u32 value) { printf("%" PRIu32, value); }
void print_u64(u64 value) { printf("%" PRIu64, value); }
void print_f32(f32 value) { printf("%f", value); }
void print_f64(f64 value) { printf("%f", value); }
void print_bool(bool value) { printf("%d", value); }
void print_string(char *value) { printf("%s", value); }
void print_text(u8 value) {
  for (size_t i = 0; i < value.len; i++) {
    printf("%c", value.data[i]);
  }
}