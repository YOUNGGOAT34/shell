
#include "parser.h"




i32 longest_common_prefix(i8 *matches[],i32 matches_count){

    i32 count=0;

    for(i32 i=0;matches[0][i]!='\0';i++){
          
        i8 current_char=matches[0][i];

        for(i32 j=1;j<matches_count;j++){

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
    //   i8 last_buffer[MAX_BUFFER_SIZE]={0};
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

        //  i8 *cwd=getcwd(NULL,0);
        //  i8 *home=getenv("HOME");

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
             i32 bytes_read=read(STDIN_FILENO,&c,1);
             if(bytes_read<=0){
                disable_raw_mode(&original_termios);
                exit(0);
             }

             if(c=='\n'){
                 printf("\n");
                 break;
             }else if(c=='\t'){

             


             tab_count++;
             i8 *matches[255];

             i8 *last_space=strrchr(buffer->input,' ');
             


             AUTO *auto_complete=malloc(sizeof(AUTO));
             auto_complete->search_in_current_dir=false;
             auto_complete->directory_autocomplete=false;
             auto_complete->search_in_subdirectory=false;
             auto_complete->is_file=false;

             

             if(last_space!=NULL){

                  

                   if(*(last_space+strspn(last_space," "))=='\0'){
                        
                         completion *comp=malloc(sizeof(completion));


                        
                        
                        if(execute_completion_script(buffer->input,comp)){

                            
                             i8 *args[]={comp->completion_path,comp->completion_name," ",NULL};

                              
                             

                             i8 *match_=execute_completion_program(comp->completion_path,args);
                             strcat(match_," ");

                            

                             if(match_){


                                 strcpy(last_space+1,match_);
                                 free(match_);
                             }


                             if(comp->completion_name){
                                 free(comp->completion_name);
                             }
                            
                             if(comp->completion_path){
                                free(comp->completion_path);
                             }


                              free(comp);

                              printf("\r\033[K$ %s",buffer->input);
                              fflush(stdout);
                              break;;

                             
                        }else{
                             auto_complete->search_in_current_dir=true;
                        }

                   }else{
                       
                        auto_complete->search_in_current_dir=true;
                   } 
                   
             }

             i8 *current_word=(last_space==NULL)?buffer->input:last_space+1;

             i8 *forwadslash=strrchr(current_word,'/');

             if(forwadslash!=NULL){
               
                 auto_complete->search_in_current_dir=false;
                 auto_complete->search_in_subdirectory=true;
             }


             i8 current_word_copy[MAX_BUFFER_SIZE];
             strcpy(current_word_copy,current_word);

             i32 matches_count=autocomplete(current_word_copy,matches,auto_complete);

               
            
      
            
             if(matches_count==0){
                  printf("\a");
                  continue;
             }else if(matches_count==1){
                  
                  
                 
                   
                   if(matches[0]!=NULL){

                       if(auto_complete->search_in_current_dir){
                             
                             
                             strcpy(current_word,matches[0]);
                             
                       }else if(auto_complete->search_in_subdirectory){

                              
                            
                              i8 *last_slash_in_current_word=strrchr(current_word,'/');

                              if(last_slash_in_current_word==NULL){
                                    strcpy(current_word,matches[0]);
                              }else{

                                  
                                  if(auto_complete->is_file){
                                        strcpy(current_word,matches[0]);
                                  }else{
                                        strcpy(last_slash_in_current_word+1,matches[0]);
                                  }
                                 
                              }
                               
                              
                              
                       } else{
                            strcpy(buffer->input,matches[0]);
                       }

                       if(!auto_complete->directory_autocomplete){

                           strcat(buffer->input," ");
                       }
                      
                   }

                   free(auto_complete);

                  len=strlen(buffer->input);
                  tab_count=0;
             }else{

                       i32 lcp_length=longest_common_prefix(matches,matches_count);

                                i32 current_word_len;

                                if(last_space==NULL){
                                     current_word_len=strlen(buffer->input);
                                }else{
                                     current_word_len=strlen(last_space+1);
                                }

                     

                      if(tab_count==1){
                           if(lcp_length>current_word_len){

                               strncpy(current_word,matches[0],lcp_length);
                                        current_word[lcp_length]='\0';
                                        len=strlen(buffer->input);
                                        tab_count=0;
                           }else{
                                 printf("\a");
                                  continue;
                           }
                           
                          
                          
                      }else if(tab_count>=2){
                               

                                if(lcp_length>current_word_len){

                                        
                                        
                                        strncpy(current_word,matches[0],lcp_length);
                                        current_word[lcp_length]='\0';
                                        len=strlen(buffer->input);
                                        tab_count=0;

                         
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
                                                
                                                // printed_something=true;

                                                printf("\n");
                                        
                                    }
                      }      
                  
             }
 
             }else{

                    tab_count=0;

                    
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

         u32 args_size=sizeof(args)/sizeof(args[0]);

      

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
         }else if(strcmp("complete",command)==0){

             
              complete(args,args_size);
                 

         }else{
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