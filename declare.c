#include "declare.h"

void declare(i8 *args[],i32 args_size){
    if(args_size<2){
        fprintf(stderr,"Declare expected an option\n");
        return ;
    }

    if(strcmp(args[1],"-p")==0){
         if(args_size<3){
               fprintf(stderr,"Declare Missing variable name\n");
               return ;
         }

         printf("declare: %s: not found\n",args[2]);
    }
}