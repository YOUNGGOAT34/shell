#include "execute.h"


void execute_program(i8 *command,i8 *args[],i8 *redirect_file){


   //if the command contains / the shell shouldn't search it in PATH

   if(strchr(command,'/')){

      
          pid_t pid=fork();


         if(pid<0){
               perror("Fork failed");
               return;
         }
   
          if(pid==0){
               
              if(redirect_file!=NULL){
               i32 fd=open(redirect_file,O_WRONLY | O_CREAT | O_TRUNC,0644);

                dup2(fd,STDOUT_FILENO);
                close(fd);
              }
                
               
               execv(command,args);
               perror(command);
               exit(1);
          }else{
             wait(NULL);
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

                     if(redirect_file!=NULL){
                     i32 fd=open(redirect_file,O_WRONLY | O_CREAT | O_TRUNC,0644);

                     dup2(fd,STDOUT_FILENO);
                     close(fd);
                  }
                        execv(full_path,args);
                        perror(command);
                        exit(1);
               }else{
                     wait(NULL);
               }
   
   
                  return;
            }

            dir=strtok(NULL,":");

                        
            }

         if(dir==NULL){
              printf("%s: command not found\n",command);
         }

    
 
}
