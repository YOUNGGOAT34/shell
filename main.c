
#include <stdio.h>
#include <string.h>


typedef unsigned int u32;
typedef int i32;
typedef char i8;

int main(){
   setbuf(stdout,NULL);
   printf("$ ");

   i8 command[100];
   fgets(command,100,stdin);

   command[strlen(command)-1]='\0';

   printf("%s: command not found\n",command);

   return 0;
}