
#include "parser.h"

void parse_arguments(i8 *input,i8 *args[],Redirect *redirect){

  
         i8 *current_arg=malloc(1024);
         int i=0;//keep track of args index
         int j=0;//keep track of current arg index

         bool in_single_quotes=false;
         bool in_double_qoutes=false;

         //parse the input

         for (int k=0;input[k]!='\0';k++){
            i8 c=input[k];

             if(c=='>' || (c>='0' && c<='9' && input[k+1]=='>')){
                 redirect->redirect_flag=true;

                 if(j>0){
                     current_arg[j]='\0';
                     args[i++]=strdup(current_arg);
                     j=0;
                 }

                 if(c>='0' && c<='9'){
                   k++;
                 }

                 k++;

                 while(input[k]==' ') k++;

                 i8 file[1024];
                 i32 f=0;

                 while(input[k]!='\0' && input[k]!=' '){
                     file[f++]=input[k++];

                 }

                 file[f]='\0';
                 redirect->stdout_file=strdup(file);

                 break;
               
             }


            if(c=='\'' && !in_double_qoutes){
                in_single_quotes=!in_single_quotes;
                continue;
            }

            if(c=='\"' && !in_single_quotes){
                in_double_qoutes=!in_double_qoutes;
                continue;
            }

            //backslashes are special outside quotes and inside double quotes, but inside single quotes they are just literal characters

            if(c=='\\' && !in_single_quotes){
                k++;

                if(input[k]!='\0'){
                    current_arg[j++]=input[k];
                }

                continue;
                  
            }

            if(!in_single_quotes && !in_double_qoutes && c==' '){
                if(j>0){

                   current_arg[j]='\0';
                   args[i++]=strdup(current_arg);
                   j=0;
                }
            }else{

               current_arg[j++]=c;

            }
  
         }

         if(j>0){
             current_arg[j]='\0';
             args[i++]=strdup(current_arg);
         }

         args[i]=NULL;

         free(current_arg);

         return;


}



void parse_commands(){

      setbuf(stdout,NULL);


      while(true){

         InputBuffer *buffer=malloc(sizeof(InputBuffer));
         if(!buffer){
             fprintf(stderr,"Failed to allocate memory for the input buffer (%s)\n",strerror(errno));
             exit(EXIT_FAILURE);
         }
   
         buffer->input=malloc(MAX_BUFFER_SIZE);
         buffer->size=0;
   
         
      
         i8 *cwd=getcwd(NULL,0);
         i8 *home=getenv("HOME");

         // printf("$ ");

         if(cwd==NULL){
            printf("$ ");
         } else if(cwd && home && strstr(cwd,home)==cwd){
             printf("~%s$ ",cwd+strlen(home));
         } else{
               printf("%s$ ",cwd);
         }
       
         fgets(buffer->input,MAX_BUFFER_SIZE,stdin);
   
   
         buffer->size=strlen(buffer->input);
   
         if(buffer->size>0 && buffer->input[buffer->size-1]=='\n'){
                buffer->input[buffer->size-1]='\0';
         }
   
         i8 *args[MAX_ARGS_SIZE];
         
         Redirect *redirect=malloc(sizeof(Redirect));
         redirect->stderr_file=NULL;
         redirect->stdout_file=NULL;
         redirect->redirect_flag=false;

   
   
         parse_arguments(buffer->input,args,redirect);

      

         i8 *command=args[0];
         
         if(command==NULL){
             free(buffer->input);
             free(buffer);
             continue;
         }
   
   
         if(strcmp(command,"exit")==0){
            free(buffer->input);
            free(buffer);
            break;
         

         
         }else if(strcmp("type",command)==0){
              
           
              i8 *cmd=args[1];
              if(cmd==NULL){
                 fprintf(stderr,"type: missing argument\n");
                 free(buffer->input);
                 free(buffer);
                 continue;
              }
              type_command(getenv("PATH"),cmd);
              
         }else if(strcmp("pwd",command)==0){

            pwd();

         }else if(strcmp("cd",command)==0){
                 change_directory(args[1]);
         }
         
         else{
            execute_program(command,args,redirect);
            
         }


         if(redirect->stderr_file){
             free(redirect->stderr_file);
         }

         if(redirect->stdout_file){
             free(redirect->stdout_file);
         }


         free(redirect);
      
         free(buffer->input);
         free(buffer);
         
      }
    
}