#include "execute.h"


void execute_program(i8 *command,i8 *args[],Redirect *redirect){

    
   //if the command contains / the shell shouldn't search it in PATH

   if(strchr(command,'/')){

      
          pid_t pid=fork();


         if(pid<0){
               perror("Fork failed");
               return;
         }
   
          if(pid==0){
               
              if(redirect->stdout_file!=NULL){
                        if(redirect->append){

                              i32 fd=open(redirect->stdout_file,O_WRONLY | O_CREAT | O_APPEND,0644);
                              if(fd<0){
                                     perror("open");
                                     exit(1);
                              }

                              dup2(fd,STDOUT_FILENO);
                              close(fd);
                              

                        }else{
                            
                              i32 fd=open(redirect->stdout_file,O_WRONLY | O_CREAT | O_TRUNC,0644);

                              if(fd<0){
                                    perror("open");
                                    exit(1);
                              }

                              dup2(fd,STDOUT_FILENO);
                              close(fd);
                        }

              }

              if(redirect->stderr_file!=NULL){

                        if(redirect->append){

                              i32 fd=open(redirect->stderr_file,O_WRONLY | O_CREAT | O_APPEND,0644);
                              if(fd<0){
                                    perror("open");
                                    exit(1);
                              }
      
                              dup2(fd,STDERR_FILENO);
                              close(fd);

                        }else{

                              i32 fd=open(redirect->stderr_file,O_WRONLY | O_CREAT | O_TRUNC,0644);
                              if(fd<0){
                                    perror("open");
                                    exit(1);
                              }
      
                              dup2(fd,STDERR_FILENO);
                              close(fd);
                        }
              }
                
               
               execv(command,args);
               perror(command);
               exit(1);
          }else{
             waitpid(pid,NULL,0);
          }
    

      return;
   }

   
   //if the above case was false then the shelll has to search in PATH

   i8 *path_env=getenv("PATH");
         i8 path_copy[1024];
         if(!path_env){

             fprintf(stderr,"Command not found\n");
             return;
             
         }
         strncpy(path_copy,path_env,sizeof(path_copy)-1);
         path_copy[sizeof(path_copy)-1]='\0';

         i8 *dir=strtok(path_copy,":");
         while(dir!=NULL){
              i8 full_path[255];
              snprintf(full_path,sizeof(full_path),"%s/%s",dir,command);


            if(access(full_path,X_OK)==0){

   
               pid_t pid=fork();

               if(pid<0){
                   perror("Fork failed");
                   return;
               }
   
               if(pid==0){ 

                     if(redirect->stdout_file!=NULL){

                              if(redirect->append){

                                    i32 fd=open(redirect->stdout_file,O_WRONLY | O_CREAT | O_APPEND,0644);
      
                                    if(fd<0){
                                          perror("open");
                                          exit(1);
                                    }

                                    dup2(fd,STDOUT_FILENO);
                                    close(fd);


                              }else{

                                    i32 fd=open(redirect->stdout_file,O_WRONLY | O_CREAT | O_TRUNC,0644);
      
                                    if(fd<0){
                                          perror("open");
                                          exit(1);
                                    }
      
                                    dup2(fd,STDOUT_FILENO);
                                    close(fd);
                              }
                     }
                     
                     if(redirect->stderr_file!=NULL){

                              if(redirect->append){
                                    i32 fd=open(redirect->stderr_file,O_WRONLY | O_CREAT | O_APPEND,0644);
      
                                    if(fd<0){
                                          perror("open");
                                          exit(1);
                                    }
      
                                    dup2(fd,STDERR_FILENO);
                                    close(fd);

                              }else{
                                    i32 fd=open(redirect->stderr_file,O_WRONLY | O_CREAT | O_TRUNC,0644);
      
                                    if(fd<0){
                                          perror("open");
                                          exit(1);
                                    }
      
                                    dup2(fd,STDERR_FILENO);
                                    close(fd);

                              }
                            
                       }

                  execv(full_path,args);
                  perror(command);
                  exit(1);
               }else{
                     waitpid(pid,NULL,0);
               }
   
   
                  return;
            }

            dir=strtok(NULL,":");

                        
            }

         if(dir==NULL){
              printf("%s: command not found\n",command);
         }

    
 
}
