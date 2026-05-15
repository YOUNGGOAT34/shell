#ifndef JOBS_H
#define JOBS_H
#include "utils.h"

typedef enum{
    RUNNING,
    DONE,
    STOPPED

}jobStatus;

typedef struct{
     i32 job_number;
     i8 *command;
     pid_t pid;
     jobStatus status;

}Job;

void create_background_job(i8 *full_command,i8 *args[]);
void show_jobs();
void reap_done_jobs_before_next_prompt();

#endif