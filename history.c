#include "history.h"
void history(i8 *hist[],i32 *size,i32 n,i32 args_size,i8 *args[]){


    if(args_size>1){


        if(strcmp(args[1],"-r")==0){
              if(args_size<3){
                 fprintf(stderr,"Missing file name\n");
              } 
              
              FILE *file=fopen(args[2],"r");
              if(file==NULL){
                perror("Error opening file");
                exit(EXIT_FAILURE);
              }

              i32 index=*size;

              i8 line[255];

              while(fgets(line,sizeof(line),file)){

                    
                    
                     line[strcspn(line,"\n")]='\0';  
                      
                    
                     hist[index++]=strdup(line);
                     
              }

              *size=index;
        }else{

            if(n>0){
                    for(i32 i=*size-n;i<*size;i++){
                          printf("%d  %s\n",i+1,hist[i]);
                      }
            }

        }


    }else{

       
    
              for(i32 i=0;i<*size;i++){
                    
                    printf("%d  %s\n",i+1,hist[i]);
              }  
   
    }

   
}