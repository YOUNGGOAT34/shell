#ifndef DECLARE_H
#define DECLARE_H
#include "utils.h"


#define MAX_TABLE_SIZE 250

typedef struct variable{

     i8 *variable_name;
     i8 *variable_value;
     struct variable *next;
}variable;

void declare(i8 *args[],i32 args_size);

#endif