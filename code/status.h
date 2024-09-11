#ifndef C9_STATUS

#include "types.h" // i32

#if 0

This header defines a struct that can be used as a namespaced enum for status codes. The struct has two fields: OK and ERROR, which are both int32_t. The values of the fields are 0 and 1, respectively.

Use the status struct to return status codes from functions. For example:

```c
#include "status.h"

int32_t my_function(void) {
  if (error_condition) {
    return status.ERROR;
  }
  return status.OK;
}
```

#endif

typedef struct {
  i32 OK;
  i32 ERROR;
} StatusType;

const StatusType status = {
  .OK = 0,
  .ERROR = 1,
};

#define C9_STATUS
#endif