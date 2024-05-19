# C9 lang
C9 is a subset of the C99 programing language with opinionated coding style.

## Purpose
The purpose of C9 is to create a learnable, portable and readable programming language that can be compiled on existing C99 compilers. It does this by limiting the number of keywords, types and features available in the language as well as promoting a set of opinionated coding styles.

## Keywords
All C99 keywords are reserved in C9, but many are not allowed.

Some keywords are only allowed in specific contexts:
- `break` is not allowed outside of switch statements
- `goto` is not allowed except for error handling inside functions
- `const` is not allowed in local scope (inside functions)

ANSI C keywords:

|              |              |              |            |
| ------------ | ------------ | ------------ | ---------- |
| ~~auto~~     | _break_      | case         | ~~char~~   |
| _const_      | ~~continue~~ | default      | ~~do~~     |
| double       | else         | enum         | ~~extern~~ |
| float        | for          | _goto_       | if         |
| ~~int~~      | ~~long~~     | ~~register~~ | return     |
| ~~short~~    | ~~signed~~   | sizeof       | static     |
| struct       | switch       | typedef      | union      |
| ~~unsigned~~ | void         | ~~volatile~~ | while      |


C99 keywords:

|            |              | 
| ---------- | ------------ | 
| ~~inline~~ | ~~restrict~~ |
| _Bool      |  _Complex    |
| _Imaginary |              |


As C9 is guaranteed to be C99 compliant, any keywords from newer C standards are not allowed.

This leaves the following keywords available in C9:

|         |          |            |          |
| ------- | -------- | ---------- | -------- |
| _break_ | case     | _const_    | double   |
| else    | enum     | float      | for      |
| _goto_  | if       | return     | sizeof   |
| static  | struct   | switch     | typedef  |
| union   | void     | while      |          |

_Bool, _Complex, _Imaginary

## Types

- No built in integer types are allowed as they can vary in size. Use `inttypes.h` instead.

```C
// Not allowed
int a = 0;

// Allowed
int32_t a = 0;
```

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

- The long and short keywords are not allowed as they can vary in size between systems.
  
```C
// Not allowed
short a = 0;
long b = 0;
long long c = 0;

// Allowed
int16_t a = 0;
int32_t b = 0;
int64_t c = 0;
```

- All variables and pointers must be initiated with a value.

```C
int32_t a = 0;
int32_t *ptr = 0; // Yes, literal 0
```

## Variables

- Prefer using long variable names and avoid abbreviations

```C
// Discouraged
int32_t p;
int32_t pc;
int32_t procCnt;

// Encouraged
int32_t processCount;
```

- Tentative definitions are not allowed

```C
int32_t n;
int32_t n; // Not allowed
```

- Names of structs, unions and enums must be unique
- Use `const` for constants in global scope
- Never use `const` in local scope (inside functions) as it lifts the variable to global scope.

```C
const int32_t MAX = 100;
```

## Structs

- All structs are typedefs

```C
typedef struct {
  int32_t: width;
  int32_t: height;
} RectangleType;
```

- Prefer using designated initialisers for structs

```C
// Discouraged
RectangleType rectangle = { 10, 20 };

// Encouraged
RectangleType rectangle = { .width = 10, .height = 20 };
```

- Do not use flexible array members in structs

```C
// Not allowed
typedef struct {
  int32_t width;
  int32_t height;
  int32_t data[];
} RectangleType;
```


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
int32_t foo(int32_t a, int32_t b) 
{ 
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

- Prefer initalizing functions as static except main

```C
static int32_t add(int32_t a, int32_t b) {
  return a + b;
}
```

- Prefer passing variables by value instead of reference

```C
typedef struct {
  int32_t value;
} MyType;

static MyType add(MyType a, MyType b) {
  MyType result;
  result.value = a.value + b.value;
  return result;
}
```

Return values or structs from functions instead of assigning to result pointers

```C
// Discouraged
static void add(int32_t a, int32_t b, int32_t *result) {
  *result = a + b;
}

// Encouraged
static int32_t add(int32_t a, int32_t b) {
  return a + b;
}
```

- In case of arrays - return the result inside a struct.

```C
typedef struct {
  int32_t value[2];
} result_t;

static result_t addAndSubtract(int32_t a, int32_t b) {
  result_t result;
  result.value[0] = a + b;
  result.value[1] = a - b;
  return result;
}
```

## Arrays

- Variable length arrays are not allowed.
  
```C
// Not allowed
int32_t n = 10;
int32_t array[n];

// Allowed
int32_t array[10];
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

## Memory management
- Prefer using the stack instead of the heap when possible.

```C
// Discouraged
int32_t *array = malloc(10 * sizeof(int32_t));
free(array);

// Encouraged
int32_t array[10];
```

- Using `malloc` and `free` is allowed but discouraged.

- Freeing in the same scope as the allocation is mandatory.

```C
// Not allowed
int32_t getArray() {
  int32_t *array = malloc(10 * sizeof(int32_t));
  return array;
}
int32_t *array = getArray();
free(array);

// Allowed
int32_t *array = malloc(10 * sizeof(int32_t));
free(array);
```

- Consider using an arena allocator instead of malloc and free

## Preprocessor

- Preprocessor is only used for includes and conditionals
  
```C
// Allowed
#include <stdio.h>

// Allowed
#if 0
  int32_t a = 0;
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
- Macro defines can be replaced with constants or enums
- The C spec for macros is not well defined and can vary between compilers
- C9 is one language, not 2 languages (C and the preprocessor)

## Comments
- Only single line `//` comments are allowed

```C
// This is a comment
```

- To comment out multiline code, use `#if 0` and `#endif`

```C
#if 0
  // This comment is ignored
  int32_t a = 0;
  int32_t b = 1;
#endif
```

Reasons:
- `/* */` comments are not nestable

## Compiling
To compile C9 code, use the following flags:

### GCC:
`gcc -std=c99 -Wall -Wextra -Werror -O2 -std=c99 -pedantic test.c -o test`

### Clang:
`clang -std=c99 -Wall -Wextra test.c -o test`
