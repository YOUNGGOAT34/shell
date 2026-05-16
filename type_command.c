
#include "type_command.h"
#include "utils.h"

void type_command(i8 * path_env,i8 *cmd){
     
       

     //process inbuilt commands
      if(is_builtin(cmd)){
          printf("%s is a shell builtin\n",cmd);
          return;
      }
      

      i8 path_copy[1024];
      strcpy(path_copy,path_env);
      i8 *dir=strtok(path_copy,":");

      while(dir!=NULL){
          i8 full_path[255];
          snprintf(full_path,sizeof(full_path),"%s/%s",dir,cmd);

          if(access(full_path,X_OK)==0){
               printf("%s is %s\n",cmd,full_path);
               return;
          }

          dir=strtok(NULL,":");

      }

      printf("%s: not found\n",cmd);

   
}