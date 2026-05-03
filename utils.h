#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>


typedef unsigned int u32;
typedef int i32;
typedef char i8;
typedef unsigned char u8;

typedef struct{

   i8 *input;
   u8 size;

}InputBuffer;

typedef struct{
    i8* stdout_file;
    i8 *stderr_file;
    bool redirect_flag;

}Redirect;

#endif