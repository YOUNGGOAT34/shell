#ifndef CD_H
#define CD_H
#include <stdio.h>
#include <unistd.h>
#include<errno.h>
#include <string.h>

typedef unsigned int u32;
typedef int i32;
typedef char i8;
typedef unsigned char u8;

void change_directory(i8 *path);

#endif