#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define ADD(a, b) a + b

int a = 0;
int n;
int n;

volatile int volatile_var = 0;
unsigned int unsigned_var = 0;
short short_var = 0;
long long_var = 0;
long long long_long_var = 0;
inline void inline_func() {}

char char_var = 'a';
int int_var = 0;
signed int signed_var = 0;

int foo(a, b)
int a;
int b;
{
  return a + b;
}

int bar()
{
  return 0;
}

int main()
{
  int n = 10;
  int array[n];

  int *p = malloc(10 * sizeof(int));
  free(p);

  free(array);

  /* This is a comment */

  return 0;
}