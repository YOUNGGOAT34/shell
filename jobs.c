#include "jobs.h"

#include <unistd.h>
#include<sys/types.h>
#include <sys/wait.h>

i32 job_number=0;
i32 current_job_index=0;

Job jobs[256];





void create_background_job(i8 *full_command,i8 *args[]){


   
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


         job_number++;

         jobs[current_job_index].job_number=job_number;
         jobs[current_job_index].command=strdup(full_command);
         jobs[current_job_index].status=RUNNING;
         jobs[current_job_index].pid=pid;

         current_job_index++;

         printf("[%d] %d\n",job_number,pid);
         fflush(stdout);

     }
    

}


void reap_job(i32 index){
     free(jobs[index].command);
     for(i32 i=index;i<current_job_index;i++){
         jobs[i]=jobs[i+1];

     }

     current_job_index--;
}


void show_jobs(){
      for(i32 i=0;i<current_job_index;i++){


           i32 status;

           pid_t result=waitpid(jobs[i].pid,&status,WNOHANG);

           if(result>0 && WIFEXITED(status)){
               
               jobs[i].status=DONE;

               i32 command_len=strlen(jobs[i].command);

               while(command_len>0 && (jobs[i].command[command_len-1]=='&' || jobs[i].command[command_len-1]==' ')){
                   jobs[i].command[--command_len]='\0';
               }
                
           }
           
           i8 *status_str;
           switch(jobs[i].status){
                  case RUNNING:
                       status_str="Running";
                       break;
                  case STOPPED:
                       status_str="Stopped";
                       break;
                  case DONE:
                       status_str="Done";
                       break;      

           }
           if(i==current_job_index-1){

              printf("[%d]+ %-24s  %s\n",jobs[i].job_number,status_str,jobs[i].command);
           }else if(i==current_job_index-2){
              printf("[%d]- %-24s  %s\n",jobs[i].job_number,status_str,jobs[i].command);
           }else{
              printf("[%d]  %-24s  %s\n",jobs[i].job_number,status_str,jobs[i].command);
           }

           if(result>0 && WIFEXITED(status)){
               reap_job(i);
           }
      }
}