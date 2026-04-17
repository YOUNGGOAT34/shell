
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX_BUFFER_SIZE 128

typedef unsigned int u32;
typedef int i32;
typedef char i8;
typedef unsigned char u8;


void type_command(i8 * path_evn,i8 *cmd){

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
      strcpy(path_copy,path_evn);
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

      i8 *command=strtok(buffer->input," ");
      
      if(command==NULL){
          continue;
      }

      if(strcmp(command,"exit")==0){
         break;
      }else if(strcmp(command,"echo")==0){
          printf("%s\n",buffer->input+strlen(command)+1);
         
      }else if(strcmp("type",command)==0){
           
        
           i8 *cmd=strtok(NULL," ");
           if(cmd==NULL){
              fprintf(stderr,"type: missing argument\n");
              continue;
           }
           type_command(getenv("PATH"),cmd);
           
      }else{
         printf("%s: command not found\n",command);
      }
   
      free(buffer->input);
      free(buffer);
   }

   return 0;
}