#ifndef C9_TYPES

#include <inttypes.h> // int8_t, int16_t int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t

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

#define C9_TYPES
#endif