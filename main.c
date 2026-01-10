
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>


#define MAX_BUFFER_SIZE 128

typedef unsigned int u32;
typedef int i32;
typedef char i8;
typedef unsigned char u8;

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

      if(strcmp(command,"exit")==0){
         break;
      }else if(strcmp(command,"echo")==0){
          printf("%s\n",buffer->input+strlen(command)+1);
      }else{
         printf("%s: command not found\n",command);
      }
   
      free(buffer->input);
      free(buffer);
   }

   return 0;
}