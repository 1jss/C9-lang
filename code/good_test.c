// This is a test file for the C9 linter to make sure it doesn't throw any errors on valid C9 code

#include <stdio.h> // printf, size_t

#include "arena.h" // Arena, arena_open, arena_fill, arena_size, arena_capacity, arena_reset, arena_close
#include "array.h" // Array, array_create, array_push, array_shift, array_pop, array_get, array_set, array_length, array_last
#include "types.h" // i32, f32, print_f32, print_i32, print_s8, s8, str8, bool

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

typedef enum { DATA,
               ERROR } ReturnType;

typedef struct {
  i32 code;
  s8 message;
} ErrorType;

typedef struct {
  ReturnType type;
  union {
    f32 result;
    ErrorType error;
  };
} ReturnDataType;

static ReturnDataType divide(TestStruct props) {
  if (props.b == 0) {
    ErrorType error = {.code = 1, .message = str8("Division by zero")};
    ReturnDataType data = {.type = ERROR, .error = error};
    return data;
  }
  f32 value = (f32)props.a / (f32)props.b;
  ReturnDataType data = {.type = DATA, .result = value};
  return data;
}

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

  s8 text_data = str8("Hello, world!");
  print_s8(text_data);
  printf(" : ");
  print_i32(text_data.len);
  printf("\n");
  RectangleType rectangle = createRectangle(test_struct);

  ReturnDataType good_result = divide(test_struct);
  if (good_result.type == DATA) {
    print_f32(good_result.result);
    printf("\n");
  } else {
    printf("Error: ");
    print_s8(good_result.error.message);
    printf("\n");
  }

  ReturnDataType bad_result = divide((TestStruct){.a = 10, .b = 0});
  if (bad_result.type == DATA) {
    print_f32(bad_result.result);
    printf("\n");
  } else {
    printf("Error: ");
    print_s8(bad_result.error.message);
    printf("\n");
  }

  printf("Sum: %d\n", sum);
  printf("Sum again: %d\n", sum_again);
  printf("Difference: %d\n", difference);
  printf("Rectangle width: %d, height: %d\n", rectangle.width, rectangle.height);

  Arena *arena = arena_open(12); // Intentionally small arena for testing growth
  printf("Arena size: %zu\n", arena_size(arena));
  printf("Arena capacity: %zu\n", arena_capacity(arena));

  TestStruct *arena_data = arena_fill(arena, sizeof(TestStruct));
  if (arena_data == 0) {
    printf("Failed to allocate memory in arena\n");
    return 1;
  }

  i32 *arena_int = arena_fill(arena, sizeof(i32));
  if (arena_int == 0) {
    printf("Failed to allocate memory in arena\n");
    return 1;
  }

  *arena_data = test_struct;
  printf("Arena size after fill: %zu\n", arena_size(arena));
  printf("Arena capacity after fill: %zu\n", arena_capacity(arena));

  arena_reset(arena);
  printf("Arena size after reset: %zu\n", arena_size(arena));
  printf("Arena capacity after reset: %zu\n\n", arena_capacity(arena));

  arena_close(arena);

  // test the array functions

  Arena *array_arena = arena_open(1024);
  Array *array = array_create(array_arena);

  i32 a = 10;
  i32 b = 20;
  i32 c = 30;

  for (i32 i = 0; i < 5; i++) {
    array_push(array, &a, sizeof(i32));
    array_push(array, &b, sizeof(i32));
    array_push(array, &c, sizeof(i32));
  }

  printf("Array length: %zu\n", array_length(array));

  // set value at last index
  i32 d = 40;
  array_set(array, array_last(array), &d, sizeof(i32));

  // find item with value 40
  size_t index = 0;
  bool found = false;
  while (index <= array_last(array) && !found) {
    i32 *value = (i32 *)array_get(array, index);
    if (value != 0 && *value == 40) {
      printf("Found value 40 at index: %zu\n", index);
      found = true;
    }
    index++;
  }

  for (i32 i = 0; i < 10; i++) {
    i32 *value = (i32 *)array_pop(array);
    printf("Poped value: %d\n", *value);
  }

  printf("Array length after pop: %zu\n", array_length(array));

  // Push some more values
  array_push(array, &a, sizeof(i32));
  array_push(array, &b, sizeof(i32));

  printf("Array length after push: %zu\n", array_length(array));

  printf("Array arena storage: %zu\n", arena_size(array_arena));
  printf("Array arena capacity: %zu\n", arena_capacity(array_arena));

  arena_close(array_arena);

  return 0;
}