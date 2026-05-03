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
#include "utils.h"





void execute_program(i8 *command,i8 *args[],Redirect *redirect);

#endif