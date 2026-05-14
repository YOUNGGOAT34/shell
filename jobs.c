#include "jobs.h"
#include <unistd.h>
#include<sys/types.h>

i32 job_number=0;

void create_background_job(i8 *args[]){

     job_number++;

     pid_t pid=fork();

     if(pid<0){
        perror("execv");
        exit(EXIT_FAILURE);
     }
 
     if(pid==0){
         
         execvp(args[0],args);
         perror("execv");
         exit(EXIT_FAILURE);
     }else{
         
         printf("[%d] %d\n",job_number,getpid());
         fflush(stdout);

     }
    



}