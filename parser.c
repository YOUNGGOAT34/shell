
#include "parser.h"

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
         int i=0;
   
         i8 *token=strtok(buffer->input," ");
   
         while(token!=NULL){
               args[i++]=token;
               token=strtok(NULL," ");
         }
   
   
         args[i]=NULL;
   
   
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
         }else if(strcmp(command,"echo")==0){
             
             for(int j=1;args[j]!=NULL;j++){
                printf("%s ",args[j]);
             }
   
             printf("\n");
            
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
            execute_program(command,args);
            
         }
      
         free(buffer->input);
         free(buffer);
      }
    
}