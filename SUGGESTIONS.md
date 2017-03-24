# Suggestions to SEGGER

A general suggestion is to use standard C types, not the antique U8, U16, U32 definitions.

Standard C type are available since C98, so almost two decades should be enough for SEGGER to adopt them too.

## The frontend plug-in API

### Max size when returning pointers

`RTOS_GetThreadDisplay()`, `RTOS_GetThreadReg()`, `RTOS_GetThreadRegList()` have a pointer where the string should be returned, but the max size is not passed.

Suggestions: 

- add a `size_t out_size_bytes` parameter.

### Shutdown

There is no way to orderly free resouces allocated by the plug-in.

Suggestions: 

- add `RTOS_Shutdown()`

### Reorder prototype parameters

The usual recommandation is to have the output parameters at the end of the list, not at the beginning.

When passing pointers to objects, be sure the first pointer is the one refering to the object the function is a member (similar to C++ this).

### The init function return value

The `RTOS_Init()` return value is not consistent with the other calls.

Suggestions:

- change to 0 = ok, <0 = error

## The backend server API

### Target endianness

Suggestions: 

- add a function like `bool is_target_little_endian (void)`.

### 64 bits (long long)

There is no way to use a 64-bits value from the target.

Suggestions:

- add read/write/convert functions for 64 bits

### Return character count from printf()-like calls

Suggestions:

- change the return type to int, as all `printf()` system calls, and return the number of characters actually written.


