#include "auto_complete.h"




int starts_with(i8 *word,i8* cmd){
    if(strcmp(word,"")==0){
        return 0;
    }

    
     return strncmp(cmd,word,strlen(word))==0;
}

int autocomplete(i8 *buffer,i8 *matches[],AUTO *auto_complete){

      i32 matches_count=0;
     
      if(starts_with(buffer,"echo")){
         
         matches[0]=strdup("echo");
        
         return 1;
      }else if(starts_with(buffer,"exit")){
         matches[0]=strdup("exit");
        
        return 1;
      }else{

          if(auto_complete->search_in_current_dir){
            
            
            DIR *d=opendir(".");

            if(d){

                struct dirent *entry;

              

                while((entry=readdir(d))!=NULL){
                     
                     if(starts_with(buffer,entry->d_name) || strcmp(buffer,"")==0){
                        
                        if(entry->d_type==DT_DIR){
                            if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0){
                                continue;
                            }

                            

                            matches[matches_count]=malloc(strlen(entry->d_name)+2);
                            strcpy(matches[matches_count],entry->d_name);
                            strcat(matches[matches_count],"/");
                            auto_complete->directory_autocomplete=true;

                            matches_count++;
                           
                        }else{
                            auto_complete->is_file=true;
                            matches[matches_count++]=strdup(entry->d_name);
                        }
                        
                     }
                }

            }

            closedir(d);

          }else if(auto_complete->search_in_subdirectory){

              
               
               
               i8 *last_forwadslash=strrchr(buffer,'/');
                
               i8 *file_name;
               i8 *file_name_copy;

               i8 *directory;

               if(last_forwadslash==NULL){
                 directory=".";
                 file_name=buffer;
               }else{
                    file_name=last_forwadslash+1;
                    
                    i32 len=last_forwadslash-buffer;
                    directory=malloc(len+1);
                    strncpy(directory,buffer,len);

                    directory[len]='\0';
               }

                DIR *d=opendir(directory);
                file_name_copy=strdup(file_name);
                
                if(d){
                    struct dirent *entry;
                    while((entry=readdir(d))!=NULL){
                         
                        
                          if(starts_with(file_name_copy,entry->d_name) || strcmp(file_name_copy,"")==0){
                               
                                   
                                   if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0){
                                        continue;
                                   }

                                
                                   strcpy(last_forwadslash+1,entry->d_name);
                                  

                                   if(entry->d_type==DT_DIR){

                                        matches[matches_count]=malloc(strlen(entry->d_name)+2);

                                        strcpy(matches[matches_count],entry->d_name);
                                       
                                        strcat(matches[matches_count],"/");
                                        auto_complete->directory_autocomplete=true;
                                        matches_count++;
                                   }else{
                                        auto_complete->is_file=true;
                                        matches[matches_count++]=strdup(buffer);
                                   }

                                   
                               
                              
                          }
                    }
                }

                if(last_forwadslash!=NULL){

                    free(directory);
                }
            
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
                          
                         }
                            
                    }
    
                  }

                  if(d){

                      closedir(d);
                  }
    
    
                  dir=strtok(NULL,":");
    
    
              }
          }
           

      }

      return matches_count;
}