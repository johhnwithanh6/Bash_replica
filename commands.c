#define _GNU_SOURCE
#include <stdio.h>    //Standard I/O functions and definitions
#include <stdlib.h>   //Standard library (includes exit function)
#include <string.h>
#include <fcntl.h>
#include "smash.h"
#include "history.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAXLINE 4096
char *cop;
int executeExternalCommands(char *[]);
void executeCommand(char *str){

    char * args [MAXLINE];
     cop=strndup(str,strlen(str));
    int exitStatus=0;
    int count=0;
    char* token=strtok(str, " ");
    if(token==NULL){
      free(cop);
      return; 
    }
    while (token != NULL) {// creating the token
     // if(token=="<"){
//printf("Test");
    //  }
        args[count]=token;
        token = strtok(NULL, " ");
        count++;
    }
    
    if(strcmp(args[0],"exit")==0){ // exiting 
      clear_history();
      free(cop);
      exit(0);
    } else if(strcmp(args[0],"cd")==0){// for entering a file
      if(args[1]!=NULL){
      int i;
      char dir[233];
        i =  chdir(args[1]);
        if (i==0){
  getcwd(dir,100);
  printf("%s\n",dir);
  exitStatus=0;
 
        }else{ 
   exitStatus=1;
  perror(args[1]);
        }
      }else{
printf("Please enter a directory: cd [directory]\n");
exitStatus=1;
      }
    }else if(strcmp(args[0],"history")==0){ // for getting history
      
        exitStatus=0;
        print_history(3);
        
        
    }else {// any other commands that did not work
        int i =0;
exitStatus = executeExternalCommands(args);
if(exitStatus==127){
        while(i<count){
    fprintf(stderr, "[%d] ",i);   
    fprintf(stderr, args[i]);  
    fprintf(stderr, "\n");  
            i++;
        }
}

        
        
    }
     add_history(cop,exitStatus );
    
}

int executeExternalCommands(char *args[]){
  int checker = 0;
  int copyIn, copyOut, index;
  int indexPipe=-2;
  int pipeCheck=-2;
   char * file;
   char* new;
  copyIn= dup(0);
  copyOut= dup(1);
  for (int i=1; args[i] !=NULL;i++){
      
      
      new = args[i];
      //printf(new);
      if(new[0]=='<'){
        args[i]= new +1;
        file = args[i];
        checker =1;
      }else if(new[0]=='>'){
        args[i]= new +1;
        file = args[i];
        checker =2;

      }else if(0== strcmp(">",args[i])){
        index=i;
        checker =4;

      }else if(0== strcmp("<",args[i])){
        index=i;
        checker =3;

      }else if(new[0]== '|'||(0== strcmp("|",args[i] ))){
        printf("djdj");
        checker =5;
    if(pipeCheck==-2){
        
        indexPipe=i;
        printf("%d",indexPipe);
    }else{
        printf("Please put only one pipe for each argument");
    }

      }
      if(checker==1||checker==2){

      //file =new+1;
      break;
      }else if(checker==3||checker==4){
for ( int j=index; args[i] !=NULL; j++){
  args[j]=args[j+1];
}
file=args[index];
break;
      }
     
    }
    if(checker==1||checker==3){
      int redirectIn;
      redirectIn= open(file, O_RDONLY);
      dup2(redirectIn,0);
      close(redirectIn);
      
    } else if(checker==2||checker==4){
      int redirectOut;
      redirectOut= open(file, O_CREAT |  O_WRONLY  , S_IWGRP | S_IWUSR | S_IRUSR | S_IRGRP );
      dup2(redirectOut,1);
      close(redirectOut);
      
    }
    
        int exitStatus=0;
        fflush(stdout);               //Flush parent's stdout buffer into kernel
        int pid = fork(); 
//Fork will return twice, once in each process
     if (pid==0) {
        execvp(args[0], args);
        exitStatus=127;
        exit(exitStatus);
  
    }else if (pid>0) {
        fflush(stdout);                    //Flush stdout buffer to kernel                
        wait(&exitStatus);                 //Wait for child to exit.
    }else {
        
        perror("Fork failed");
        exitStatus=(0-1);
    }
    
    dup2(copyIn,0);
    dup2(copyOut,1);
    
      close(copyIn);
       close(copyOut);
    return   WEXITSTATUS(exitStatus);
    
    }
