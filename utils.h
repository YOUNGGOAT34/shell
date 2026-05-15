#ifndef UTILS_H
#define UTILS_H


#include <stdio.h>
#include <errno.h>
#include<stdint.h>
#include <unistd.h>
#include <string.h>
#include<stdbool.h>
#include<stdlib.h>
#include <termios.h>
#include <dirent.h>


typedef unsigned int u32;
typedef int i32;
typedef char i8;
typedef unsigned char u8;

typedef struct{

   i8 *input;
   u8 size;

}InputBuffer;

typedef struct {
    i8* stdout_file;
    i8 *stderr_file;
    bool redirect_flag;
    bool append;
    bool pipe;

}Redirect;


typedef struct{
        bool search_in_current_dir;
         bool search_in_subdirectory;
         bool directory_autocomplete;
         bool is_file;
}AUTO;

#endif