#ifndef C9_STRING

#include <stdbool.h> // bool
#include <string.h> // memcpy
#include "arena.h" // Arena, arena_fill
#include "status.h" // status
#include "types.h" // u8, u32, i32

#if 0

This header defines a string type and helper functions for it. The string type is implemented like a simple struct, ie no pointer tricks. The creative helper functions, like copy and replace are returning new strings, not modifying the input strings. These functions also take an arena as an argument, where it allocates memory for the new string. The same arena can preferably be used for many strings and be freed all at once.
- s8: a struct that represents a string
- to_s8: a function that converts a string literal to an s8
- new_string: a function that returns a growing string with initial space for 32 characters
- string_from_substring: a function that returns a growing string from a u8 pointer, index and length
- insert_into_string: a function that inserts a string into another string at a given index
- delete_from_string: a function that deletes a range from a string at a given index
- to_char: a function that converts an s8 to a char pointer
- equal_s8: a function that compares two s8s
- indexof_s8: a function that finds the index of a substring in an s8
- includes_s8: a function that finds a substring in an s8
- concat_s8: a function that concatenates two s8 strings
- concat3_s8: a function that concatenates three s8 strings
- replace_s8: a function that replaces all occurrences of a substring in an s8 string

#endif

const i32 INVALID_STRING_INDEX = -1;

typedef struct {
  u8 *data;
  i32 length;
  i32 capacity; // Current capacity of the string
} s8;

// Converts string literal to s8
s8 to_s8(char *string) {
  i32 length = 0;
  while (string[length] != '\0') {
    length++;
  }
  // Set capacity to 0 to ensure that the string literal is never written to
  return (s8){
    .data = (u8 *)string,
    .length = length,
    .capacity = 0
  };
}

// Get a growing string with initial space for 32 characters
s8 new_string(Arena *arena) {
  u8 *data = arena_fill(arena, sizeof(u8) * 32);
  data[0] = '\0';
  s8 string = {
    .data = data,
    .length = 0,
    .capacity = 32,
  };
  return string;
}

// Returns a growing string from a u8 pointer, index and length
s8 string_from_substring(Arena *arena, u8 *string, i32 index, i32 length) {
  u8 *data = arena_fill(arena, sizeof(u8) * length + 1);
  // Copy the replaced text
  memcpy(data, string + index, length);
  data[length] = '\0';
  s8 result = {
    .data = data,
    .length = length,
    .capacity = length + 1,
  };
  return result;
}

// Inserts a string into another string at a given index
i32 insert_into_string(Arena *arena, s8 *target, s8 substring, i32 index) {
  if (index < 0 || index > target->length) return status.ERROR;
  i32 new_length = target->length + substring.length + 1; // +1 for null terminator
  // Grow the string if needed
  if (new_length > target->capacity) {
    // Ensure the capacity is not zero
    if (target->capacity == 0) {
      target->capacity = new_length;
    }
    // Double the capacity until the new data fits
    while (new_length > target->capacity) {
      target->capacity *= 2;
    }
    // New allocation for the data
    u8 *new_data = arena_fill(arena, sizeof(u8) * target->capacity);
    memcpy(new_data, target->data, target->length);
    new_data[target->length] = '\0';
    target->data = new_data;
  }
  // Shift the characters to the right of the insertion point to the right, starting from the end
  for (i32 i = target->length - 1; i >= index; --i) {
    target->data[i + substring.length] = target->data[i];
  }
  // Copy the substring onto the now free space
  memcpy(target->data + index, substring.data, substring.length);
  // Update the length
  target->length += substring.length;
  // Add null terminator
  target->data[target->length] = '\0';
  return status.OK;
}

// Deletes a string from another string at a given index
void delete_from_string(s8 *target, i32 index, i32 length) {
  if(target->capacity == 0) return;
  // Shift the characters to the right of the deleted text to the left
  for (i32 i = index + length; i < target->length; i++) {
    target->data[i - length] = target->data[i];
  }
  target->length = target->length - length;
  // Add null terminator
  target->data[target->length] = '\0';
}

char *to_char(s8 string) {
  return (char *)(string.data);
}

// Compare two strings
bool equal_s8(s8 a, s8 b) {
  if (a.length != b.length) return false;
  for (i32 i = 0; i < a.length; i++) {
    if (a.data[i] != b.data[i]) {
      return false;
    }
  }
  return true;
}

// Find the start index of a substring in a string
i32 indexof_s8(s8 target, s8 substring) {
  if (target.length < substring.length) return INVALID_STRING_INDEX;
  for (i32 i = 0; i < target.length - substring.length; i++) {
    bool found = true;
    for (i32 j = 0; j < substring.length; j++) {
      if (target.data[i + j] != substring.data[j]) {
        found = false;
        break;
      }
    }
    if (found) return i;
  }
  return INVALID_STRING_INDEX;
}

// Check if a string includes a substring
bool includes_s8(s8 target, s8 substring) {
  return indexof_s8(target, substring) != INVALID_STRING_INDEX;
}

// Concatenate two strings
s8 concat_s8(Arena *arena, s8 a, s8 b) {
  i32 total_length = a.length + b.length;
  u8 *data = (u8 *)arena_fill(arena, sizeof(u8) * (total_length + 1));
  memcpy(data, a.data, a.length);
  memcpy(data + a.length, b.data, b.length);
  data[total_length] = '\0';
  return (s8){
    .data = data,
    .length = total_length,
    .capacity = total_length + 1
  };
}

// Concatenate three strings (for internal use only)
static s8 concat3_s8(Arena *arena, s8 a, s8 b, s8 c) {
  i32 total_length = a.length + b.length + c.length;
  u8 *data = (u8 *)arena_fill(arena, sizeof(u8) * (total_length + 1));
  memcpy(data, a.data, a.length);
  memcpy(data + a.length, b.data, b.length);
  memcpy(data + a.length + b.length, c.data, c.length);
  data[total_length] = '\0';
  return (s8){
    .data = data,
    .length = total_length,
    .capacity = total_length + 1
  };
}

s8 replace_s8(Arena *arena, s8 target, s8 substring, s8 replacement) {
  // Find the first occurrence of the substring
  i32 index = indexof_s8(target, substring);
  if (index == INVALID_STRING_INDEX) return target;
  // Text before the substring
  s8 prefix = {
    .data = target.data,
    .length = index,
    .capacity = 0
  };
  i32 rest_length = target.length - index - substring.length;
  // Text after the substring
  s8 rest = {
    .data = target.data + index + substring.length,
    .length = rest_length,
    .capacity = 0
  };
  // Recursively replace the substring in the rest of the string
  s8 suffix = replace_s8(arena, rest, substring, replacement);
  // Concatenate the prefix, replacement and suffix
  return concat3_s8(arena, prefix, replacement, suffix);
}

#define C9_STRING
#endif