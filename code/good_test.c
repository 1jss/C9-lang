// This is a test file for the C9 linter to make sure it doesn't throw any errors on valid C9 code

#include <stdio.h> // printf
#include "arena.h" // Arena, arena_open, arena_fill, arena_size, arena_capacity, arena_reset, arena_close
#include "array.h" // Array, array_create, array_push, array_pop, array_get, array_set, array_length, array_last
#include "status.h" // status
#include "string.h" // s8, to_s8, print_s8, concat_s8, includes_s8, replace_s8, replaceall_s8
#include "types.h" // i32, f32, bool
#include "types_print.h" // print_f32, print_i32

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

typedef struct {
  i32 status;
  union {
    f32 result;
    s8 error_message;
  };
} ReturnDataType;

static ReturnDataType divide(TestStruct props) {
  if (props.b == 0) {
    ReturnDataType data = {
      .status = status.ERROR,
      .error_message = to_s8("Division by zero")
    };
    return data;
  }
  f32 value = (f32)props.a / (f32)props.b;
  ReturnDataType data = {
    .status = status.OK,
    .result = value
  };
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
  RectangleType rectangle = {
    .width = props.a,
    .height = props.b
  };
  return rectangle;
}

void print_s8(s8 string) {
  for (i32 i = 0; i < string.length; i++) {
    printf("%c", string.data[i]);
  }
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

  s8 text_data = to_s8("Hello, world!");
  print_s8(text_data);
  printf(" : ");
  print_i32(text_data.length);
  printf("\n");
  RectangleType rectangle = createRectangle(test_struct);

  ReturnDataType good_result = divide(test_struct);
  if (good_result.status == status.OK) {
    print_f32(good_result.result);
    printf("\n");
  } else {
    printf("Error: ");
    print_s8(good_result.error_message);
    printf("\n");
  }

  ReturnDataType bad_result = divide((TestStruct){.a = 10, .b = 0});
  if (bad_result.status == status.OK) {
    print_f32(bad_result.result);
    printf("\n");
  } else {
    printf("Error: ");
    print_s8(bad_result.error_message);
    printf("\n");
  }

  printf("Sum: %d\n", sum);
  printf("Sum again: %d\n", sum_again);
  printf("Difference: %d\n", difference);
  printf("Rectangle width: %d, height: %d\n", rectangle.width, rectangle.height);

  Arena *arena = arena_open(12); // Intentionally small arena for testing growth
  printf("Arena size: %d\n", arena_size(arena));
  printf("Arena capacity: %d\n", arena_capacity(arena));

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
  printf("Arena size after fill: %d\n", arena_size(arena));
  printf("Arena capacity after fill: %d\n", arena_capacity(arena));

  arena_reset(arena);
  printf("Arena size after reset: %d\n", arena_size(arena));
  printf("Arena capacity after reset: %d\n\n", arena_capacity(arena));

  arena_close(arena);

  // test the array functions

  Arena *array_arena = arena_open(1024);
  Array *array = array_create(array_arena, sizeof(i32));

  i32 a = 10;
  i32 b = 20;
  i32 c = 30;

  for (i32 i = 0; i < 5; i++) {
    array_push(array, &a);
    array_push(array, &b);
    array_push(array, &c);
  }

  printf("Array length: %d\n", array_length(array));

  // set value at last index
  i32 d = 40;
  array_set(array, array_last(array), &d);

  // find item with value 40
  i32 index = 0;
  bool found = false;
  while (index <= array_last(array) && !found) {
    i32 *value = (i32 *)array_get(array, index);
    if (value != 0 && *value == 40) {
      printf("Found value 40 at index: %d\n", index);
      found = true;
    }
    index++;
  }

  for (i32 i = 0; i < 10; i++) {
    i32 *value = (i32 *)array_pop(array);
    printf("Poped value: %d\n", *value);
  }

  printf("Array length after pop: %d\n", array_length(array));

  // Push some more values
  array_push(array, &a);
  array_push(array, &b);

  printf("Array length after push: %d\n", array_length(array));

  printf("Array arena storage: %d\n", arena_size(array_arena));
  printf("Array arena capacity: %d\n", arena_capacity(array_arena));

  arena_close(array_arena);

  Arena *string_arena = arena_open(512);
  s8 hello = to_s8("Hello");
  s8 world = to_s8("World");
  s8 hello_world = concat_s8(string_arena, hello, world);

  bool includes = includes_s8(hello_world, to_s8("loWo"));

  print_s8(hello_world);
  if (includes) {
    printf(" includes 'loWo'\n");
  } else {
    printf(" does not include 'loWo'\n");
  }

  s8 replaced_hello = replace_s8(string_arena, hello_world, to_s8("loWo"), to_s8("lo wo"));

  print_s8(replaced_hello);
  printf(" %d\n", replaced_hello.length);

  s8 all_replaced = replace_s8(string_arena, replaced_hello, to_s8("l"), to_s8("j"));
  print_s8(all_replaced);

  arena_close(string_arena);

  return 0;
}