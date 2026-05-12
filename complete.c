#include "complete.h"

completion *completions[MAX_TABLE_SIZE];

void initialize_completions(){

}

u32 hash(i8 *completion_name){
     
   
    u32 hash_value=0;
    u32 len=strlen(completion_name);

    for(u32 i=0;i<len;i++){
       hash_value+=completion_name[i];
       hash_value=(hash_value*completion_name[i])%MAX_TABLE_SIZE;
    } 

    return hash_value;

}


void insert(completion *comp){
       if(!comp){
            fprintf(stderr,"Cannot insert empty completion\n");
            exit(EXIT_FAILURE);
       }

     

   u32 index=hash(comp->completion_name);
   
   comp->next=completions[index];

   completions[index]=comp;


 
}


completion *search_completion(i8 *completion_name){
       u32 index=hash(completion_name);
       completion *tmp=completions[index];

       while(tmp && strcmp(tmp->completion_name,completion_name)!=0){
              tmp=tmp->next;
       }

       return tmp;
}



void complete(i8 *args[],u32 args_size){

      initialize_completions();


      if(args_size<2){
            fprintf(stderr,"complete:missing option\n");
            return;
      }


      if(strcmp(args[1],"-p")==0){
            if(args_size<3){
                  fprintf(stderr,"complete: missing completion name\n");
                   return;
            }

            completion *comp=search_completion(args[2]);

            if(!comp){
                  printf("complete: %s: no completion specification\n",args[2]);
            }else{
                  printf("complete -C '%s' %s\n",comp->completion_path,comp->completion_name);
            }
            
      }else if(strcmp(args[1],"-C")==0){
              if(args_size<4){
                  fprintf(stderr,"complete:missing arguments\n");
                  return;
              }

              completion *comp=malloc(sizeof(completion));
                   comp->completion_path=strdup(args[2]);
                   
                   comp->completion_name=strdup(args[3]);
                   comp->next=NULL;

                   insert(comp);


      }

     

}

