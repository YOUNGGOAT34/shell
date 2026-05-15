#ifndef PARSE_H
#define PARSE_H


#include "type_command.h"
#include "auto_complete.h"
#include "execute.h"
#include "pwd.h"
#include "cd.h"
#include "complete.h"
#include "jobs.h"
#include "pipe.h"




#define MAX_BUFFER_SIZE 128
#define MAX_ARGS_SIZE 32



void parse_commands();

#endif
