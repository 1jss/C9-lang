#ifndef C9_STRING

#include <inttypes.h> // uint8_t
#include <stddef.h>   // size_t
#include <stdio.h>    // printf
#include <stdbool.h>  // bool

#if 0

This header defines a string type and helper functions for it.
- s8: a struct that represents a string
- to_s8: a function that converts a char array to an s8
- print_s8: a function that prints an s8
- equal_s8: a function that compares two s8s
- find_s8: a function that finds a substring in an s8

#endif

typedef struct {
  uint8_t *data;
  size_t length;
} s8;

s8 to_s8(char *s) {
  uint32_t length = 0;
  while (s[length] != '\0') {
    length++;
  }
  return (s8){(uint8_t *)s, length};
}

void print_s8(s8 value) {
  for (size_t i = 0; i < value.length; i++) {
    printf("%c", value.data[i]);
  }
}

bool equal_s8(s8 a, s8 b) {
  if (a.length != b.length) {
    return false;
  }
  for (size_t i = 0; i < a.length; i++) {
    if (a.data[i] != b.data[i]) {
      return false;
    }
  }
  return true;
}

bool find_s8(s8 a, s8 b) {
  for (size_t i = 0; i < a.length - b.length; i++) {
    bool found = true;
    for (size_t j = 0; j < b.length; j++) {
      if (a.data[i + j] != b.data[j]) {
        found = false;
        break;
      }
    }
    if (found) {
      return true;
    }
  }
  return false;
}

#define C9_STRING
#endif