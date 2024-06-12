#ifndef C9_TYPES_PRINT

#include <inttypes.h> // PRId8, PRId16, PRId32, PRId64, PRIu8, PRIu16, PRIu32, PRIu64
#include <stdbool.h> // bool
#include <stdio.h> // printf
#include "types.h" // i8, i16, i32, i64, u8, u16, u32, u64, f32, f64

#if 0

This header defines print functions for the types in types.h

#endif

void print_i8(i8 value) { printf("%" PRId8, value); }
void print_i16(i16 value) { printf("%" PRId16, value); }
void print_i32(i32 value) { printf("%" PRId32, value); }
void print_i64(i64 value) { printf("%" PRId64, value); }
void print_u8(u8 value) { printf("%" PRIu8, value); }
void print_u16(u16 value) { printf("%" PRIu16, value); }
void print_u32(u32 value) { printf("%" PRIu32, value); }
void print_u64(u64 value) { printf("%" PRIu64, value); }
void print_f32(f32 value) { printf("%f", value); }
void print_f64(f64 value) { printf("%f", value); }
void print_bool(bool value) { printf("%d", value); }
void print_string(char *value) { printf("%s", value); }

#define C9_TYPES_PRINT
#endif