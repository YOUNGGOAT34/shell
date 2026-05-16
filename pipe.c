#include "pipe.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void pipeline(i8 *args[],i32 pipe_count){

      
     

     i8 *args_array[pipe_count+1][32];

     pid_t pids[pipe_count+1];

     for(i32 i=0;i<=pipe_count;i++){
          pids[i]=-1;
     }



     i32 index=0;
     i32 j=0;

     

     for(i32 i=0;i<pipe_count+1;i++){
            
            while(args[index]!=NULL && strcmp(args[index],"|")!=0){
               
                 
                 args_array[i][j++]=strdup(args[index++]);    
            }

   
            args_array[i][j]=NULL;
            
            if(args[index]!=NULL) index++;

            j=0;

     }

     

     i32 pipefd[pipe_count][2];

     for(i32 i=0;i<pipe_count;i++){
          pipe(pipefd[i]);
     }

    
     for(i32 i=0;i<=pipe_count;i++){

         
               pids[i]=fork();

               if(pids[i]<0){
                  perror("fork");
                  exit(EXIT_FAILURE);
               }
            
               if(pids[i]==0){

  
                  if(i>0) dup2(pipefd[i-1][0],STDIN_FILENO);
                  if(i<pipe_count) dup2(pipefd[i][1],STDOUT_FILENO);


                   for(i32 k=0;k<pipe_count;k++){
                      close(pipefd[k][0]);
                      close(pipefd[k][1]);
                  }

                   if(is_builtin(args_array[i][0])){
            
                     
                     if(i>0) dup2(pipefd[i-1][0],STDIN_FILENO);
                     if(i<pipe_count) dup2(pipefd[i][1], STDOUT_FILENO);
                     
                     run_builtin(args_array[i]);
                     exit(0);
                     
                  }else{


                 
                
            
                  execvp(args_array[i][0],args_array[i]);
                  perror("execvp");
                  exit(EXIT_FAILURE);
                  
               }

                }

               if(i>0) close(pipefd[i-1][0]);

               if(i<pipe_count) close(pipefd[i][1]);
  
     }

     

     for(i32 i=0;i<=pipe_count;i++){
          if(pids[i]!=-1) waitpid(pids[i],NULL,0);
     }


}