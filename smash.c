#define _GNU_SOURCE
#include <stdio.h>    //Standard I/O functions and definitions
#include <stdlib.h>   //Standard library (includes exit function)
#include <string.h>  
#include <signal.h>
#define MAXLINE 4096
#include "history.h" 
char bfr[MAXLINE];
void executeCommand(char *str);
void signalHandler();
void signalHandler(){
    SIGINT;
    fflush(stdout);
    fflush(stdin);
    fflush(stderr);
     printf("\n");
     fputs("$ ",stderr); 
  
}

int main(int argc, char **argv) {
fputs("$ ",stderr);  //Output the first prompt
signal(SIGINT, signalHandler);


 //Loop reading commands until EOF or error while void
setvbuf(stdout,NULL,_IONBF,0);
void init_histor();
while (fgets(bfr, MAXLINE, stdin)!=NULL) {
  
  
  if(strnlen(bfr, MAXLINE+1)>MAXLINE){
            return 1;
        }
  
    bfr[strlen(bfr)-1] = '\0';    //Replace newline with NULputs(bfr);
     
    executeCommand(bfr);
    fputs("$ ",stderr);
    
}
clear_history();
return 0;
}

