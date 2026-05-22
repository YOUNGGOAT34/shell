#ifndef HISTORY_H
#define HISTORY_H
#include "utils.h"

void history(i8 *hist[],i32 *size,i32 n,i32 args_size,i8 *args[]);
void load_history_from_file_on_start_up(i8 *hist[],i32 *size);
#endif