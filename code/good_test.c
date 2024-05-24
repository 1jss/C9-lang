// This is a test file for the C9 linter to make sure it doesn't throw any errors on valid C9 code

#include <stdio.h> // printf
#include <stdlib.h>

// Typedefs and print functions
#include "c9.h" // i32

#if 0
This is a comment
#endif

// Only use const in global scope
const i32 MAX = 100;

// All structs are typedefs
typedef struct {
  i32 width;
  i32 height;
} RectangleType;

// All structs are typedefs
typedef struct {
  i32 a;
  i32 b;
} TestStruct;

// Functions (except main) are static
static i32 add(TestStruct props) {
  return props.a + props.b;
}

static i32 subtract(TestStruct props) {
  return props.a - props.b;
}

static TestStruct swap(TestStruct props) {
  i32 temp = props.a;
  props.a = props.b;
  props.b = temp;
  return props;
}

static RectangleType createRectangle(TestStruct props) {
  RectangleType rectangle = {.width = props.a, .height = props.b};
  return rectangle;
}

i32 main(void) {
  TestStruct test_struct = {
    .a = 10,
    .b = 20
  };

  i32 sum = add(test_struct);
  TestStruct swapped = swap(test_struct);
  i32 difference = subtract(swapped);

  // Using compound literal as a cast
  i32 sum_again = add((TestStruct){.a = 10, .b = 20});

  RectangleType rectangle = createRectangle(test_struct);

  printf("Sum: %d\n", sum);
  printf("Sum again: %d\n", sum_again);
  printf("Difference: %d\n", difference);
  printf("Rectangle width: %d, height: %d\n", rectangle.width, rectangle.height);

  return 0;
}