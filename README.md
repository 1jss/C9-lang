# C9 lang
C9 is a beginner friendly subset of the C99 programing language with opinionated coding style.

## Purpose
The purpose of C9 is to create a learnable, portable and readable programming language that can be compiled on existing C99 compilers. It does this by limiting the number of keywords, types and features available in the language as well as promoting a set of opinionated coding styles. The intention is to make a small (*learnable*) language that can be compiled on many different (desktop) systems (*portable*) in a standardised style that makes the code easy to understand (*readable*).

> C9 is not intended to replace C99. If you already know C or come to a point where you need a keyword that is not available in C9, use C99 instead.

## Goals
- **Learnable**: Should be beginner friendly. Include only a very small amount of keywords.
- **Portable**: Should compile on any existing C99 compiler. No build system or external libraries required.
- **Readable**: The language should have a standardised coding style.

# Introduction
C9 is a compiled programming language based on C.

## Compiling
C9 can be compiled on any existing C99 compatible compiler such as **GCC** or **Clang**.

### GCC:
`gcc -std=c99 -Wall -Wextra -Werror -O2 -std=c99 -pedantic main.c -o main`

### Clang:
`clang -std=c99 -Wall -Wextra main.c -o main`

## Types
C9 is a typed language and uses the types defined in `types.h` which is included in the `code` folder. These are:

| Type    | Description             |
| ------- | ----------------------- |
| i8      | 8 bit signed integer    |
| i16     | 16 bit signed integer   |
| i32     | 32 bit signed integer   |
| i64     | 64 bit signed integer   |
| u8      | 8 bit unsigned integer  |
| u16     | 16 bit unsigned integer |
| u32     | 32 bit unsigned integer |
| u64     | 64 bit unsigned integer |
| f32     | 32 bit floating point   |
| f64     | 64 bit floating point   |


## Comments
C9 supports single line and multi line comments.

```C
// This is a single line comment

/*
This is a multi line comment
*/
```

## Flow control
C9 supports the following flow control statements:

- `if` statement
- `else` statement
- `while` loop
- `for` loop

```C
if (a == 1) {
  printf("a is 1\n");
} else if (a == 2) {
  printf("a is 2\n");
} else {
  printf("a is not 1 or 2\n");
}

while (a < 10) {
  printf("a is %d\n", a);
  a++;
}

for (i32 i = 0; i < 10; i++) {
  printf("i is %d\n", i);
}
```

## Functions
Functions are defined as follows:

```C
i32 add(i32 a, i32 b) {
  return a + b;
}
```

```
Return type of the function. 
|   Name of the function
|   |   Parameters with their types
|   |   |
v   v   v  
i32 add(i32 a, i32 b)
```

If a function does not return a value or takes no parameters use `void`, which is a type that represents nothing.

```C
void print_hello(void) {
  printf("Hello, World!\n");
}
```

## Variables
Variables are defined as follows:

```C
i32 a = 0;
```

```
The type of the variable.
|   The name of the variable
|   |   The value of the variable
|   |   |
v   v   v
i32 a = 0;
```

- Each variable must be initiated with a value.
- Each variable must be declared on a separate line.
- Prefer using long variable names and avoid abbreviations.

## Structs
Structs are a way to group variables together. They are defined as follows:

```C
typedef struct {
  i32 width;
  i32 height;
} RectangleType;
```

`RectangleType` is the name of the struct and `width` and `height` are the variables inside the struct. The RectangleType struct can now be used as a type in the program as follows:

```C
RectangleType my_rectangle = {
  .width = 10,
  .height = 20
  };

i32 area = my_rectangle.width * my_rectangle.height;
```

## Constants
Constants are available in global scope and are defined as follows:

```C
const i32 MAX_ITERATIONS = 100;
```

Constants are not allowed in local scope (inside functions).

## Arrays
Arrays are defined as follows:

```C
i32 array[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
```

The built in array type is fixed length. If you need a dynamic array, use the `Array` type from `array.h` in the C9-libs repo.

## Includes
A header file is a file that contains definitions of functions and types that can be included in other files. To include a header file from your project directory, use double quotes.

```C
#import "types.h"
```

To include a system header file, use angle brackets.
```C
#include <stdio.h>
```

When including a header file, add a comment with the functions or types that are included and used from the header.

```C
#import "types.h" // i8, i32, u8
#include <stdio.h> // printf
```

Note that C9 can use any C99 compatible libraries. Note, however that libraries that are not "header only" requires a different build command.

## Pointers (references)
A pointer is a reference to a variable's address in memory. 

- Pointers are defined with a `*` before the variable name.
- To get the address of a variable add `&` before the variable name.
- To get the content of a reference add `*` before the reference.
- To assign a value to a reference add `*` before the reference.

```C
i32 a = 10; // A normal variable
i32 *a_reference = &a; // A pointer to a's address
i32 b = *a_reference; // b is assigned a's content
*a_reference = 20; // Assign 20 to the content of the address in a_reference
```

## Memory management
Variables can be stored either on "the stack" or "the heap". The stack is a region of memory that is automatically used and released by the program. The heap is a region of memory that is manually used and released by the programmer.

To use the stack, define the variable as usual:

```C
i32 a = 10;
```

To use the heap, use the `malloc` and `free` functions:

```C
i32 *a = malloc(sizeof(i32));
*a = 10;
free(a);
```

Manually allocating memory is the source of many bugs, so it is recommended to use the stack whenever possible. If you need to allocate memory on the heap, consider using the arena allocator (`arena.h`) in the C9-libs repo. This file contains functions for allocating memory on the heap in a safer way.

# C9 and C99 comparison
As C9 is a subset of the C99 programing language the following part is intended for someone that already knows C99 and wants to know what is missing in C9 and why.

## Keywords
All C99 keywords are reserved in C9, but many are not allowed.

ANSI C keywords:

|              |              |              |            |
| ------------ | ------------ | ------------ | ---------- |
| ~~auto~~     | ~~break~~  | ~~case~~     | ~~char~~   |
| _const_      | ~~continue~~ | ~~default~~  | ~~do~~     |
| double       | else         | ~~enum~~     | ~~extern~~ |
| float        | for          | ~~goto~~     | if         |
| ~~int~~      | ~~long~~     | ~~register~~ | return     |
| ~~short~~    | ~~signed~~   | sizeof       | static     |
| struct       | ~~switch~~   | typedef      | union      |
| ~~unsigned~~ | void         | ~~volatile~~ | while      |

Some keywords are only allowed in specific contexts:
- `const` is not allowed in local scope (inside functions)

Reasons:
- Several keywords are removed as they are connected to discouraged or uncommon programming patterns (auto, continue, do, extern, goto, register, volatile). If you need any of these, you are not the target audience for C9. Use the full C99 instead.
- Keywords connected to integer types of different sizes (char, int, long, short, signed, unsigned) are removed as they can vary between systems. Use `inttypes.h` or C9's `types.h` instead. (Following the MISRA C recommendation)
- Switch case statements are removed as they are a common source of errors (fallthrough), require 4 keywords (switch, case, break, default) and can only handle matches of int and char. Use if else statements instead.
- Enums are removed as they are not typed and can cause namespace collisions. Enums are replaced with constant structs (see `status.h` for an example).

C99 keywords:

|            |              | 
| ---------- | ------------ | 
| ~~inline~~ | ~~restrict~~ |
| _Bool      |  _Complex    |
| _Imaginary |              |

C9 is guaranteed to be C99 compliant, but any keywords from newer C standards are also reserved and not allowed.

This leaves the following keywords available in C9:

|         |          |            |          |
| ------- | -------- | ---------- | -------- |
| _const_ | double   | else       | float    |
| for     | if       | return     | sizeof   |
| static  | struct   | typedef    | union    |
| void    | while    |            |          |

_Bool, _Complex, _Imaginary

## Types

- The built in integer types are not allowed as they can vary in size between systems. Use `inttypes.h` or C9's `types.h` instead.

```C
// Not allowed
char a = 0;
short b = 0;
int c = 0;
long d = 0;

// Allowed
#import <inttypes.h>
int8_t a = 0;
int16_t b = 0;
int32_t c = 0;
int64_t d = 0;

// Allowed
#import "types.h"
i8 a = 0;
i16 b = 0;
i32 c = 0;
i64 d = 0;
```

> A valid exception to this rule is when an external library requires a specific type, say a `char *` for passing text.

- Implicit ints are not used nor allowed.

```C
// Not allowed
a = 0;
function(a) {
  return a;
}

// Allowed
int32_t a = 0;
int32_t function(int32_t a) {
  return a;
}
```

- All variables and pointers must be initiated with a value.

```C
// Not allowed
int32_t a;
int32_t *pointer;

// Allowed
int32_t a = 0;
int32_t *pointer = 0;
```

- Type casting to and from pointers is not allowed

```C
// Not allowed
int32_t a = 12345;
int32_t b = &a;
int32_t *pointer = (int32_t *) b;

// Allowed
int32_t a = 12345;
int32_t b = a;
int32_t *pointer = &b;

```

## Variables

- Prefer using long variable names and avoid abbreviations

```C
// Discouraged
int32_t p;
int32_t pc;
int32_t procCnt;

// Encouraged
int32_t process_count;
int32_t processCount;
```

- Tentative definitions are not allowed

```C
int32_t n;
int32_t n; // Not allowed
```

- Use `const` for constants in global scope (instead of `#define` macros)

```C
// Not allowed
#define MAX 100

// Allowed
const int32_t MAX = 100;
```

- Never use `const` in local scope (inside functions)

```C
// Not allowed
int32_t getA(void) {
  const int32_t A = 0;
  return A;
}

// Allowed
const int32_t A = 0;
int32_t getA(void) {
  return A;
}
```

- The comma operator is not allowed for inline variable declarations. Each variable must be declared on a separate line.

```C
// Not allowed
int32_t a = 0, b = 1;

// Allowed
int32_t a = 0;
int32_t b = 1;
```

## Structs

- All structs are typedefs

```C
typedef struct {
  int32_t width;
  int32_t height;
} RectangleType;
```

- Prefer using designated initialisers for structs

```C
// Discouraged
RectangleType rectangle = { 10, 20 };

// Encouraged
RectangleType rectangle = {
  .width = 10,
  .height = 20
};
```

- Do not use flexible array members in structs

```C
typedef struct {
  int32_t width;
  int32_t height;
  int32_t data[]; // <- Not allowed
} RectangleType;
```

- Bitfields are not allowed. If storage is a concern use a bit mask instead.

```C
// Not allowed
typedef struct {
  int32_t width: 4;
  int32_t height: 4;
} RectangleType;

// Allowed
const int32_t WIDTH_MASK = 0b00001111;
const int32_t HEIGHT_MASK = 0b11110000;
typedef struct {
  int32_t dimensions;
} RectangleType;
```

- Prefer title case for struct names

```C
// Discouraged
typedef struct {
  int32_t width;
  int32_t height;
} rectangleType;

// Encouraged
typedef struct {
  int32_t width;
  int32_t height;
} RectangleType;
```

- Use constant structs instead of enums. This allows for more typed enums and prevents filling up the global namespace.
  
```C
// Not allowed
typedef enum {
  RED = 0
  GREEN = 1
  BLUE = 2
} ColorType;

int32_t color = RED;

// Allowed
typedef struct {
  const int32_t RED;
  const int32_t GREEN;
  const int32_t BLUE;
} ColorType;

const ColorType colors = {
  .RED = 0,
  .GREEN = 1,
  .BLUE = 2
};

int32_t color = colors.RED;
```

- Names of structs and unions must be unique

## Functions

- K&R function definition style is not allowed

```C
// K&R syntax not allowed
int32_t foo(a, b) 
    int32_t a; 
    int32_t b; 
{ 
    return 0; 
}

// ANSI syntax allowed
int32_t foo(int32_t a, int32_t b) { 
    return 0; 
}
```

- Function declaration without parameters is not allowed

```C
// Not allowed
int32_t foo() {
  return 0;
}

// Allowed
int32_t foo(void) {
  return 0;
}
```

- Prefer passing variables by value instead of reference

```C
// Discouraged
void add(int32_t *a, int32_t *b) {
  int32_t result = *a + *b;
  return result;
}

// Encouraged
int32_t add(int32_t a, int32_t b) {
  int32_t result = a + b;
  return result;
}
```

Return values or structs from functions instead of assigning to result pointers

```C
// Discouraged
void add(int32_t a, int32_t b, int32_t *result) {
  *result = a + b;
}

// Encouraged
int32_t add(int32_t a, int32_t b) {
  return a + b;
}
```

- If you need to return an array - do it inside a struct or use C9's `array.h`. 

```C
// Fixed array in a struct
typedef struct {
  int32_t value[2];
} ResultType;

ResultType addAndSubtract(int32_t a, int32_t b) {
  ResultType result = {0};
  result.value[0] = a + b;
  result.value[1] = a - b;
  return result;
}

// Flexible C9 array
#import "array.h"

Array *addAndSubtract(int32_t a, int32_t b) {
  Array *result = array_create(arena, sizeof(int32_t));
  int32_t a_plus_b = a + b;
  int32_t a_minus_b = a - b;
  array_push(result, &a_plus_b);
  array_push(result, &a_minus_b);
  return result;
}

```

## Arrays

- Variable length arrays are not allowed. Use fixed length arrays or C9's `array.h` instead
  
```C
// Not allowed
int32_t n = 10;
int32_t array_name[n];

// Allowed
int32_t array_name[10];

// Allowed
#include "array.h"
Array *array_name = array_create(arena, sizeof(int32_t));

```

## Pointers

- Pointers are allowed but avoided when possible by passing by value instead of reference.

```C
// Allowed
int32_t value = 0;
int32_t *value_pointer = &value;
function(value_pointer);

// Encouraged
int32_t value = 0;
function(value);
```

- Function pointers are allowed but discouraged. Typedef them if used.

```C
// Discouraged
int32_t (*function)(int32_t, int32_t);

// Allowed
typedef int32_t (*FunctionType)(int32_t, int32_t);
```

## Memory management
- Prefer using the stack instead of the heap when possible.

```C
// Discouraged
int32_t *array = malloc(10 * sizeof(int32_t));
free(array);

// Encouraged
int32_t array[10];
```

- Using `malloc` and `free` is allowed but discouraged. Use C9's arena allocator (`arena.h`) or your own custom allocator instead.

```C
// Discouraged
int32_t *heap_value = malloc(sizeof(int32_t));
int32_t *other_heap_value = malloc(sizeof(int32_t));
free(heap_value);
free(other_heap_value);

// Encouraged
#import "arena.h"
Arena *arena_name = arena_open(1024);
int32_t *heap_value = arena_fill(arena_name, sizeof(int32_t));
int32_t *other_heap_value = arena_fill(arena_name, sizeof(int32_t));
arena_close(arena_name);

```

## Preprocessor

- Preprocessor is only used for includes and include guards
  
```C
// Allowed
#include <stdio.h>

// Allowed
#ifndef FILENAME_H
...header code...
#define FILENAME_H
#endif
```

- No macros, object-like or functional-like are allowed

```C
// Not allowed
#define MAX 100

// Not allowed
#define ADD(a, b) a + b

```

Reasons:
- Macros makes the code harder to read and understand
- Macros are hard to debug
- Macros can't be type checked as they are just text replacements
- Macros have no namespace and can cause name collisions
- Macro functions can almost always be replaced with functions
- Macro defines can be replaced with constants
- The C spec for macros is not well defined and can vary between compilers
- C9 is one language, not 2 languages (C and the preprocessor)

## Comments
- Prefer single line `//` comments

```C
// This is a comment
```

Reasons:
- `/* */` comments are not nestable

## Operands
- The operands of the && and || operators must be surrounded by parentheses unless they are simple variables or constants.

```C
// Not allowed
if (a == 1 && b) {
  return 0;
}

// Allowed
if ((a == 1) && b) {
  return 0;
}
```

## Includes
- Include statements should be followed by a comment with the function or type that is being included. These can be rudimentally tested by running the `include_check.py` script in the `code` folder.

```C
#include <stdio.h> // printf
#include <stdlib.h> // malloc, free
```

## Linting
- A rudimental regex linting script is available in the `code` folder. This script can be used to check for some of the rules in the C9 language.

```bash
python3 lint.py test.c
```

## Code style
- Use 2 spaces for indentation. Tabs are not allowed.
- Format code with clang-format using the configuration file in the `code` folder.

## Compiling
> Note that C9 is built around having only one translation unit (one .c file). This avoids the need for a build system and make the build process very simple.

To compile C9 code, use the following flags:

### GCC:
`gcc -std=c99 -Wall -Wextra -Werror -O2 -std=c99 -pedantic test.c -o test`

### Clang:
`clang -std=c99 -Wall -Wextra test.c -o test`

## Inspiration
C9 is inspired by the following projects and people:

- [MISRA C](https://misra.org.uk/misra-c/)
- [Malcolm Ingils / C Style](https://github.com/mcinglis/c-style)
- [Chris Wellons](https://nullprogram.com/)
