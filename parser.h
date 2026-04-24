#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include<stdint.h>
#include <unistd.h>
#include <string.h>
#include<stdbool.h>
#include<stdlib.h>
#include "type_command.h"
#include "execute.h"



#define MAX_BUFFER_SIZE 128
#define MAX_ARGS_SIZE 32

typedef unsigned int u32;
typedef int i32;
typedef char i8;
typedef unsigned char u8;

typedef struct{

   i8 *input;
   u8 size;

}InputBuffer;

void parse_commands();

#endif
