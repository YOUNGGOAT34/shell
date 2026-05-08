
#include "parser.h"
#include <termios.h>
#include <dirent.h>



i32 longest_common_prefix(i8 *matches[],i32 matches_count){

    i32 count=0;

    for(int i=0;matches[0][i]!='\0';i++){
          
        i8 current_char=matches[0][i];

        for(int j=1;j<matches_count;j++){
              if(matches[j][i]=='\0' || matches[j][i]!=current_char){
                 
                 return count;
              }

             
        }

         count++;

       
    }

    return count;
      
}

int comparator(const void *a,const void *b){
     return strcmp(*(i8 **)a,*(i8 **)b);
}


int starts_with(i8 *word,i8* cmd){
     return strncmp(cmd,word,strlen(word))==0;
}

int autocomplete(i8 *buffer,i8 *matches[]){

      i32 matches_count=0;
    
      if(starts_with(buffer,"echo")){
         matches[0]=strdup("echo");
        //  strcpy(buffer,"echo ");
         return 1;
      }else if(starts_with(buffer,"exit")){
         matches[0]=strdup("exit");
        // strcpy(buffer,"exit ");
        return 1;
      }else{
           
          i8 path_copy[1024];
          i8 *path_env=getenv("PATH");
          if(!path_env){
              fprintf(stderr,"Path is not set: %s\n",strerror(errno));
              exit(1);
          }

          strncpy(path_copy,path_env,sizeof(path_copy)-1);
          path_copy[sizeof(path_copy)-1]='\0';

          i8 *dir=strtok(path_copy,":");

          
       

          while(dir!=NULL){
            


              DIR *d=opendir(dir);
              
              if(d){


                 struct dirent *entry;

                  
                while((entry=readdir(d))!=NULL){

                     i8 full_path[1024];
                     snprintf(full_path,sizeof(full_path),"%s/%s",dir,entry->d_name);

                     if(starts_with(buffer,entry->d_name) && access(full_path,X_OK)==0){
                         matches[matches_count++]=strdup(entry->d_name);
                        //  strcpy(buffer,entry->d_name);
                        //  strcat(buffer," ");
                        //  closedir(d);
                        //  return 1;
                     }
                        
                }

              }

             closedir(d);

              dir=strtok(NULL,":");


          }

          if(matches_count>0){

              if(matches_count==1){
                strcpy(buffer,matches[0]);
                strcat(buffer," ");
                
              }

           
          }


          


      }

      return matches_count;
}


void enable_raw_mode(struct termios *original_termios){

    struct termios raw;


    tcgetattr(STDIN_FILENO,original_termios);
    raw=*original_termios;

    raw.c_lflag&=~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw);

}

void disable_raw_mode(struct termios *original_termios){
    tcsetattr(STDIN_FILENO,TCSAFLUSH,original_termios);

}

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

                 i32 fd=1;

                 if((c=='>' && input[k+1]=='>') || (input[k+1]=='>' && input[k+2]=='>')){

                    redirect->append=true;
                    k++;

                 }

                 if(c>='0' && c<='9'){
                   fd=c-'0';
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
                 if(fd==1){
                      redirect->stdout_file=strdup(file);
                 }else if(fd==2){
                     redirect->stderr_file=strdup(file);
                 }

                

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

      struct termios original_termios;


      static i32 tab_count=0;
      i8 last_buffer[MAX_BUFFER_SIZE];
      bool printed_something=false;

      


      while(true){

         printed_something=false;

         InputBuffer *buffer=malloc(sizeof(InputBuffer));
         if(!buffer){
             fprintf(stderr,"Failed to allocate memory for the input buffer (%s)\n",strerror(errno));
             exit(EXIT_FAILURE);
         }
   
         buffer->input=malloc(MAX_BUFFER_SIZE);
         buffer->size=0;



   
         
      
         i8 *cwd=getcwd(NULL,0);
         i8 *home=getenv("HOME");

         printf("$ ");

        //  if(cwd==NULL){
        //     printf("$ ");
        //  } else if(cwd && home && strstr(cwd,home)==cwd){
        //      printf("~%s$ ",cwd+strlen(home));
        //  } else{
        //        printf("%s$ ",cwd);
        //  }


        
        

        i32 len=0;
        buffer->input[0]='\0';
        enable_raw_mode(&original_termios);
        while(true){
             i8 c;


             

             read(STDIN_FILENO,&c,1);

             if(c=='\n'){
                 printf("\n");
                 break;
             }else if(c=='\t'){



            if(strcmp(buffer->input,last_buffer)!=0){
                      tab_count=0;
                     strcpy(last_buffer,buffer->input);
             }


             tab_count++;
            
             i8 *matches[255];

             i32 matches_count=autocomplete(buffer->input,matches);

             if(matches_count==0){
                  printf("\a");
             }else if(matches_count==1){
                   if(matches[0]!=NULL){

                       strcpy(buffer->input,matches[0]);
                       strcat(buffer->input," ");
                      
                   }

                  len=strlen(buffer->input);
                  tab_count=0;
             }else{

                
                
                    
                      i32 lcp_length=longest_common_prefix(matches,matches_count);

                      if(lcp_length>strlen(buffer->input)){
                         
                          strncpy(buffer->input,matches[0],lcp_length);
                          buffer->input[lcp_length]='\0';
                          len=strlen(buffer->input);
                          tab_count=0;
                      }else{
                          if(tab_count==1){
                             printf("\a");
                          }else{
                             qsort(matches,matches_count,sizeof(i8 *),comparator);
                             printf("\n");

                             for(i32 i=0;i<matches_count;i++){
                            
                                    printf("%s", matches[i]);
                                    if(i!=matches_count-1){
                                        printf("  ");
                                    }
                                    free(matches[i]);
                              }

                                printed_something=true;

                                printf("\n");
                          }
                      }
   
                  
             }


                     
                 
                 
             }else{
                   if(len<MAX_BUFFER_SIZE-1){

                       buffer->input[len++]=c;
                       buffer->input[len]='\0';
                   }
                   
             }
  
              

                    printf("\r\033[K$ %s",buffer->input);
                    fflush(stdout);
                


        }

        disable_raw_mode(&original_termios);
       
        //  fgets(buffer->input,MAX_BUFFER_SIZE,stdin);
   
   
         buffer->size=strlen(buffer->input);
   
         if(buffer->size>0 && buffer->input[buffer->size-1]=='\n'){
                buffer->input[buffer->size-1]='\0';
         }
   
         i8 *args[MAX_ARGS_SIZE];
         
         Redirect *redirect=malloc(sizeof(Redirect));
         redirect->stderr_file=NULL;
         redirect->stdout_file=NULL;
         redirect->redirect_flag=false;
         redirect->append=false;

   
   
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