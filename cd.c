#include "cd.h"
#include <sys/stat.h>
#include <stdlib.h>


void change_directory(i8 *path){
  

    if(path==NULL){
        path=getenv("HOME");
        if(path==NULL){

           fprintf(stderr,"cd: Home is not set\n");
           return;
        }
        
    }

     if(chdir(path)!=0){
         perror("cd");
     }
}