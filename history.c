#define _GNU_SOURCE
#include <stdio.h>    //Standard I/O functions and definitions
#include <stdlib.h>   //Standard library (includes exit function)
#include <string.h>
#include "history.h"
struct Cmd * commands[MAXHISTORY];
int count;

void init_history(){
    count=0;
}

void add_history(char *cmd, int exitStatus){// this adds history to the the history struct
    struct Cmd* new=(struct Cmd *) malloc(sizeof(struct Cmd));
    new->cmd=cmd;
    new->exitStatus=exitStatus;
    new->sequenceNumber=count;
    if(count>=MAXHISTORY){
      struct Cmd *remove = commands[0];
        
        for (int i =0; i<9; i++){
           commands[i]=commands[i+1];
        }
        commands[9]=new;
free(remove->cmd);
free(remove);
    }else{
      commands[count]=new;
}
      count++;
        }

void clear_history(void){
  int i=0;
  while(commands[i]!=NULL){
   free(commands[i]->cmd);
    struct Cmd *co =commands[i];
   free(co);
    i++;
  }
}



void print_history(int firstSequenceNumber){ // this command will output all of the history.
   int i=0;
   while(commands[i]!=NULL) {   
           struct Cmd new=*commands[i];
   printf( "%d ", new.sequenceNumber+1);  
   printf( "[%d] ", new.exitStatus);  
    printf("%s ", new.cmd);  
    i++;
    printf("\n");  
}
}





