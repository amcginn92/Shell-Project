#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "helpers.h"

int programExec(char* path, char* args[]){
    int status = 0;
    //first word is program name, rest are args
    //assume first word to be full path to the program ** at first**
    //** later ** implement simple program names
    pid_t pid;
    //fork a child and get its pid
    pid = fork();
    //determine if parent or child
    if(pid == 0){    //it's child
        //if it's the child we should execv
        //at first we will just give the full path of the program we want to run
        if(execv(path, args) == -1){
            perror("Execv in programExec.c");
            exit(1);
        }
    }else if(pid > 0){  //it's the parent
        //wait for child
        waitpid(pid, &status, 0);
    }else{
        perror("This is why you should never fork a child(programExec.c)");
        exit(1);
    }

    return(0);
}