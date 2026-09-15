#include "declare.h"



variable *variables[MAX_TABLE_SIZE];


u32 hash_variable(i8 *variable_name){
     
   
    u32 hash_value=0;
    u32 len=strlen(variable_name);

    for(u32 i=0;i<len;i++){
       hash_value+=variable_name[i];
       hash_value=(hash_value*variable_name[i])%MAX_TABLE_SIZE;
    } 

    return hash_value;

}


void insert_variable(variable *var){
       if(!var){
            fprintf(stderr,"Cannot insert empty completion\n");
            exit(EXIT_FAILURE);
       }

     

      u32 index=hash_variable(var->variable_name);
      
      var->next=variables[index];

      variables[index]=var;

}



variable *search_variable(i8 *variable_name){
       u32 index=hash_variable(variable_name);
       variable *tmp=variables[index];

       while(tmp && strcmp(tmp->variable_name,variable_name)!=0){
              tmp=tmp->next;
       }

       return tmp;
}



i8  *expand_parameter(i8 *variable_name){

   

   variable *var=search_variable(variable_name);
   if(var){
       return strdup(var->variable_value);
   }else{
        return strdup("");
   }

   
}




void declare(i8 *args[],i32 args_size){
    if(args_size<2){
        fprintf(stderr,"Declare expected an option\n");
        return ;
    }
    
    if(strcmp(args[1],"-p")==0){
         if(args_size<3){
               fprintf(stderr,"Declare Missing variable name\n");
               return ;
         }


         variable *var=search_variable(args[2]);
         if(var){

            printf("declare -- %s=\"%s\"\n",var->variable_name,var->variable_value);

         }else{

            printf("declare: %s: not found\n",args[2]);
         }

         
    }else{
          
           if(((args[1][0]!='\0' && (isdigit((u8)args[1][0]))) || strchr(args[1],'-'))){

                   printf("declare: `%s': not a valid identifier\n",args[1]);
 
           }else{

              i8 *equal_sign=strchr(args[1],'=');
   
              if(equal_sign!=NULL){
                   *equal_sign='\0';
                   i8 *key=args[1];
                   i8 *value=equal_sign+1;
                   
   
                   variable *var=malloc(sizeof(variable));
                   var->variable_name=strdup(key);
                   var->variable_value=strdup(value);
   
                   insert_variable(var);
              } 
           }

    }

}