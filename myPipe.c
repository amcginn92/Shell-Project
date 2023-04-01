#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "helpers.h"
#include <sys/wait.h>


/*
 * Left off: determining pattern for duping fds
 * Should we be forking all children first?
 */

//Need to implement first/last child behavior


int myPipe(char* argv[]){
    int pipes = 0;
    int pid = -1;
    pipes = pipeCount(argv);
    int processCount = pipes + 1;
    printf("Processes: %d, Pipes: %d\n", processCount, pipes);
    int totFds = (2 * pipes);
    int fds[1024];
    int status = 0;
    int arr[1024];
    //pipe to get fds
    for(int i = 0; i < pipes; i++){
        if(pipe(&fds[2*i]) == -1){
            perror("Ya pipes are all outta whack in myPipe.c");
            exit(1);
        }
    }

    int last = 0; //last pipe number, starts -1 to help iterate correctly
    int next = 0;

    int inFd = 0;
    int outFd = 3;

    //use a for loop to fork and ex
    for(int i = 0; i < processCount; i++){ //go through each process i.e. pipes + 1
        printf("Iteration: %d, pid: %u\n", i, getpid());
        if((pid = fork()) == -1){
            perror("You got fork problems buddy!");
            exit(1);
        }
        arr[i] = pid;
        next = find_special(&argv[last],"|");  //find next occurence of pipe, save its index

        //When there are no more pipes, we don't need to add a null
        if(next != -1){
            argv[next++] = NULL;  //Replace pipe with null for program execution, skip to next process name
            printf("Next Pipe: %d, pid: %u\n", next, getpid());
        }else{
            printf("No more, pid: %u\n", getpid());
        }



        //=============================1st process only dups stdout
        if(pid == 0 && i == 0){
            printf("I am child 0!\n");
            printf("Total Fds: %d\n", totFds);
            if( (dup2(fds[1], STDOUT_FILENO)) == -1){
                perror("dup2: Error 0");
                exit(1);
            }
            for(int j = 0; j < totFds; j++){//we created 2 x pipes fds, goes up until index (2* pipes) - 1
                close(fds[j]);   //close everything, we've already duped what we need
            }
            programExec(argv[last], &argv[last]);



        //=================================================last process only changes stdin
        }else if(pid == 0 && i == (processCount - 1)){
            puts("Last Process");
            if((dup2(fds[inFd], STDIN_FILENO)) == -1){   //overwrite stdin with read pipe
                perror("dup2: Error 1");
                exit(1);
            }
            for(int j = 0; j < totFds; j++){//we created 2 x pipes fds, goes up until index (2* pipes) - 1
                close(fds[j]);   //close everything, we've already duped what we need
            }
            programExec(argv[last], &argv[last]);

        //================================Every other process changes both stdin/out
        }else if(pid == 0 && i != 0){
            //overwrite stdin with read pipe
            printf("I am child %d", i);
            if((dup2(fds[inFd], STDIN_FILENO)) ==-1){
                perror("dup2: Error 3");
                exit(1);
            }
            //overwrite stdout with write pipe
            if((dup2(fds[outFd], STDOUT_FILENO))== -1){
                perror("dup2: Error 4");
                exit(1);
            }

            for(int j = 0; j < totFds; j++){//we created 2 x pipes fds, goes up until index (2* pipes) - 1
                    close(fds[j]);   //close everything, we've already duped what we need
            }
            programExec(argv[last], &argv[last]);
        }

        //parent updates last to the next set of proc/args
        //child WILL NEVER GET DOWN HERE
        last = next;
        if(i == 0){
            printf("outFd: %d\n", outFd);
            close(fds[1]);
        }else if(i == processCount - 1){
            printf("inFd: %d\n", inFd);
            close(fds[inFd]);
        }else{
            printf("Infile: %d, OutFile: %d\n", inFd, outFd);
            close(fds[inFd]);
            close(fds[outFd]);
        }
        inFd+=2;
        outFd+=2;

    }


//    if(pid > 0){    //parent closes all fd after forking
//        for(int j = 0; j < pipes * 2; j++){
//            close(fds[j]);   //parent closes every fd
//        }
//    }


    for(int i = 0; i < processCount; i++){
        waitpid(arr[i],&status,0);
    }





//    printf("%d", fds[0]);
    //fork
    //if child (0), fork again
    //exec process1 from child1
    //cut off read end
    //write
    //if child2 (0), execute process 2
    //cut off write end
    //read
    return 0;
}