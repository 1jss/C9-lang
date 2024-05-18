#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

void print_i32(i32 value) { printf("%" PRId32, value); }
void print_i64(i64 value) { printf("%" PRId64, value); }
void print_u32(u32 value) { printf("%" PRIu32, value); }
void print_u64(u64 value) { printf("%" PRIu64, value); }
void print_f32(f32 value) { printf("%f", value); }
void print_f64(f64 value) { printf("%f", value); }
void print_bool(bool value) { printf("%d", value); }
void print_string(char *value) { printf("%s", value); }