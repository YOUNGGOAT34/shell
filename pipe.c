#include "pipe.h"
#include <sys/types.h>
#include <sys/wait.h>

void pipeline(i8 *args[]){
     i8 *args1[32];
     i8 *args2[32];


     i32 i;
     i32 args_index=0;

     for(i=0;args[i]!=NULL;i++){
         if(strcmp(args[i],"|")==0){
            i++;
            break;
         }

         args1[args_index++]=strdup(args[i]);
     }

     args1[args_index]=NULL;

     args_index=0;

     for(i32 j=i;args[j]!=NULL;j++){
        
      args2[args_index++]=strdup(args[j]);
     }

     args2[args_index]=NULL;


     i32 pipefd[2];
     pipe(pipefd);
     pid_t pid1=fork();

     if(pid1==0){

      close(pipefd[0]);
      dup2(pipefd[1],STDOUT_FILENO);
      close(pipefd[1]);

      execvp(args1[0],args1);
      perror("execvp");
      exit(EXIT_FAILURE);

     }
       
     
     pid_t pid2=fork();

     if(pid2==0){
        close(pipefd[1]);
        dup2(pipefd[0],STDIN_FILENO);
        close(pipefd[0]);
        execvp(args2[0],args2);
        perror("execvp");
        exit(EXIT_FAILURE);
     }


      close(pipefd[0]);
      close(pipefd[1]);
      waitpid(pid2,NULL,0);
   
      waitpid(pid1,NULL,0);


}