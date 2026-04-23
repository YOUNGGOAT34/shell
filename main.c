
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_BUFFER_SIZE 128
#define MAX_ARGS_SIZE 32

typedef unsigned int u32;
typedef int i32;
typedef char i8;
typedef unsigned char u8;



void execute_program(i8 *command,i8 *args[]){


   //if the command contains / the shell shouldn't search it in PATH

   if(strchr(command,'/')){

      
          pid_t pid=fork();


         if(pid<0){
               perror("Fork failed");
               return;
         }
   
          if(pid==0){
               
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

void type_command(i8 * path_env,i8 *cmd){

     //process inbuilt commands
      i8 *inbuilts[]={"exit","echo","type"};
      i32 n=3;

      for(i32 i=0;i<n;i++){
         if(strcmp(cmd,inbuilts[i])==0){
              printf("%s is a shell builtin\n",cmd);
               return;
         }
        
      }

     

      i8 path_copy[1024];
      strcpy(path_copy,path_env);
      i8 *dir=strtok(path_copy,":");

      while(dir!=NULL){
          i8 full_path[255];
          snprintf(full_path,sizeof(full_path),"%s/%s",dir,cmd);

          if(access(full_path,X_OK)==0){
               printf("%s is %s\n",cmd,full_path);
               return;
          }

          dir=strtok(NULL,":");

      }

      printf("type: %s: not found\n",cmd);


      
}

typedef struct{

   i8 *input;
   u8 size;

}InputBuffer;

int main(){
  

   while(true){


      InputBuffer *buffer=malloc(sizeof(InputBuffer));
      if(!buffer){
          fprintf(stderr,"Failed to allocate memory for the input buffer (%s)\n",strerror(errno));
          exit(EXIT_FAILURE);
      }

      buffer->input=malloc(MAX_BUFFER_SIZE);
      buffer->size=0;


      setbuf(stdout,NULL);

      printf("$ ");
    
      fgets(buffer->input,MAX_BUFFER_SIZE,stdin);


      buffer->size=strlen(buffer->input);

      if(buffer->size>0 && buffer->input[buffer->size-1]=='\n'){
             buffer->input[buffer->size-1]='\0';
      }



      i8 *args[MAX_ARGS_SIZE];
      int i=0;

      i8 *token=strtok(buffer->input," ");

      while(token!=NULL){
            args[i++]=token;
            token=strtok(NULL," ");
      }


      args[i]=NULL;


      i8 *command=args[0];
      
      if(command==NULL){
          free(buffer->input);
          free(buffer);
          continue;
      }


      if(strcmp(command,"exit")==0){
         break;
      }else if(strcmp(command,"echo")==0){
          
          for(int j=1;args[j]!=NULL;j++){
             printf("%s ",args[j]);
          }

          printf("\n");
         
      }else if(strcmp("type",command)==0){
           
        
           i8 *cmd=args[1];
           if(cmd==NULL){
              fprintf(stderr,"type: missing argument\n");
              continue;
           }
           type_command(getenv("PATH"),cmd);
           
      }else{
         execute_program(command,args);
         
      }
   
      free(buffer->input);
      free(buffer);
   }

   return 0;
}