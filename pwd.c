
#include "pwd.h"


void pwd(){
   
   i8 *current_working_directory=getcwd(NULL, 0);

   if(!current_working_directory){
       perror("getcwd");
       exit(1);
   }

   printf("%s\n",current_working_directory);



}