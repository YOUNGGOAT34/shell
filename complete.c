#include "complete.h"
#include "parser.h"

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


bool execute_completion_script(i8 *buffer,completion *comple){
         i8 completion_name_copy[MAX_BUFFER_SIZE];
         i8 *end=buffer+strlen(buffer)-1;


         while(end>buffer && *end==' '){
            end--;
         }

         strncpy(completion_name_copy,buffer,end-buffer+1);
         completion_name_copy[end-buffer+1]='\0';

         
         completion *comp=search_completion(completion_name_copy);

          

         if(comp){

           

            comple->completion_name=strdup(comp->completion_name);
            comple->completion_path=strdup(comp->completion_path);
            
            return true;
         }

         comple->completion_name=NULL;
         comple->completion_path=NULL;

         return false;


}


i8 *execute_completion_program(i8 *path,i8 *args[]){

      i32 pipefd[2];
      pipe(pipefd);

      pid_t pid=fork();

      if(pid<0){
            perror("fork failed");
            return NULL;
      }

      if(pid==0){

            close(pipefd[0]);
            dup2(pipefd[1],STDOUT_FILENO);
            close(pipefd[1]);

            execv(path,args);
            perror(path);
            exit(EXIT_FAILURE);

      }else{

            close(pipefd[1]);

            i8 buffer[1024];

            i32 n=read(pipefd[0],buffer,sizeof(buffer)-1);
            buffer[n-1]='\0';
          
            
            close(pipefd[0]);
            waitpid(pid,NULL,0);

              if(n>0){
               return strdup(buffer);
            }

            
      }

      return NULL;

}

