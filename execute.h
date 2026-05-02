#ifndef EXECUTE_H
#define EXECUTE_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>


typedef unsigned int u32;
typedef int i32;
typedef char i8;
typedef unsigned char u8;

void execute_program(i8 *command,i8 *args[],i8 *redirect_file);

#endif