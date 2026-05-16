#include "history.h"
void history(i8 *hist[],i32 *size,i32 n){

    if(n>0){
       
        for(i32 i=*size-n;i<*size;i++){
               printf("%d  %s\n",i+1,hist[i]);
        }

    }else{

          for(i32 i=0;i<*size;i++){
                
                printf("%d  %s\n",i+1,hist[i]);
          }  
    }

   
   
}