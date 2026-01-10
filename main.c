
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


typedef unsigned int u32;
typedef int i32;
typedef char i8;

int main(){
   setbuf(stdout,NULL);

   while(true){

      printf("$ ");
   
      i8 command[100];
   
      fgets(command,100,stdin);
   
      command[strlen(command)-1]='\0';

      if(strcmp(command,"exit")==0){
         break;
      }
   
      printf("%s: command not found\n",command);
   }

   return 0;
}