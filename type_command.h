#ifndef TYPE_COMMAND_H
#define TYPE_COMMAND_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

typedef unsigned int u32;
typedef int i32;
typedef char i8;
typedef unsigned char u8;

void type_command(i8 * path_env,i8 *cmd);


#endif