#include "utils.h"
#include "cd.h"
#include "jobs.h"
#include "type_command.h"
#include "pwd.h"


bool is_builtin(i8 *command){

     return strcmp(command,"type")==0 ||
            strcmp(command,"exit")==0 ||
            strcmp(command,"echo")==0 ||
            strcmp(command,"cd")==0 ||
            strcmp(command,"pwd")==0  ||
            strcmp(command,"jobs")==0 ||
            strcmp(command,"history")==0 ||
            strcmp(command,"complete")==0;

}


void run_builtin(i8 *args[]){
    if(strcmp(args[0],"type")==0){
         type_command(getenv("PATH"),args[1]);
    }else if(strcmp(args[0],"pwd")==0){
         pwd();
    }else if(strcmp(args[0],"cd")==0){
        change_directory(args[1]);
    }else if(strcmp(args[0],"echo")==0){
        pid_t pid=fork();

        if(pid<0){
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if(pid==0){
            execvp(args[0],args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }else if(strcmp(args[0],"jobs")==0){
        show_jobs();
    }
}