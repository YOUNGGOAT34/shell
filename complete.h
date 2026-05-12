#ifndef COMPLETE_H
#define COMPLETE_H
#include "utils.h"

#define MAX_TABLE_SIZE 250

typedef struct completion{

     i8 *completion_name;
     i8 *completion_path;

     struct completion *next;


}completion;

void complete(i8 *args[],u32 args_size);
bool execute_completion_script(i8 *completion_name,completion *comple);
i8 *execute_completion_program(i8 *path,i8 *args[]);


#endif