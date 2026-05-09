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
#include "pwd.h"
#include "cd.h"



#define MAX_BUFFER_SIZE 128
#define MAX_ARGS_SIZE 32

typedef struct{
        bool search_in_current_dir;
         bool search_in_subdirectory;
         bool directory_autocomplete;
}AUTO;

void parse_commands();

#endif
