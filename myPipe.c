#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "helpers.h"
#include <sys/wait.h>


/*
 * Left off: using exit to debug
 * Linux shell interrupting
 */




int myPipe(char* argv[]){
    int pipes = 0;
    int pid = -1;
    pipes = pipeCount(argv);
    int processCount = pipes + 1;
//    printf("Parent: %d, Processes: %d, Pipes: %d\n", getpid(), processCount, pipes);
    int totFds = (2 * pipes);
    int fds[1024];
    int status = 0;
    int arr[1024];

//    int k = 0;
//    while(argv[k] != NULL){
//        printf("%s ", argv[k]);
//        k++;
//    }
//    puts("");



    //create an array of n-1 pipes
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

    //=========================================================================== Start fork/exec
    int cur;
    for(cur = 0; cur < processCount; cur++) { //fork each process

        if ((pid = fork()) == -1) {
            perror("You got fork problems buddy!");
            exit(1);
        } else if (pid == 0) { //children break out
            break;
        }
        arr[cur] = pid;
    }
        if(pid == 0){
//            printf("Process %d Created, pid: %u\n", cur, getpid());
        }


    //=====================parent closes all fd after forking
    if(pid > 0){
        for(int j = 0; j < pipes * 2; j++){
            close(fds[j]);   //parent closes every fd
        }
    }

    //=======================children have to find their program/args they need to find and define
    if(pid == 0){
//        printf("Pid: %d, cur: %d\n", getpid(), cur);
        for(int i = 0; i <= cur;i++){
            last = next;
            next = find_special(&argv[last],"|");  //find next occurence of pipe
            next = last + next;
//            printf("Pid: %d, Next: %s, Last: %s\n", getpid(), argv[next], argv[last]);
            if(next == -1){
//                puts("No more pipes broseph!");
                break;
            }
            argv[next++] = NULL;
//            printf("Pid: %d, Next: %d %s, Last: %d %s\n", getpid(), next, argv[next], last, argv[last]);
        }
//        printf("Pid: %d, Next: %d %s, Last: %d %s\n", getpid(), next, argv[next], last, argv[last]);
    }
    //last has the address of current process name and the string is null terminated

    //===================================================  1ST  process only dups stdout
    if(pid == 0 && cur == 0){
//        printf("I am child %d, running: %s, fd: %d\n", cur, argv[last], 1);
//        printf("Total Fds: %d\n", totFds);
        if( (dup2(fds[1], STDOUT_FILENO)) == -1){
            perror("dup2: Error 0");
            exit(1);
        }
        for(int j = 0; j < totFds; j++){//we created 2 x pipes fds, goes up until index (2* pipes) - 1
            close(fds[j]);
        }
        pExec(argv[last], &argv[last]);

        //========================================================================  LAST  process only changes stdin
    }else if(pid == 0 && cur == (processCount - 1)){
//        printf("I am child %d, running: %s, using fd: %d\n", cur, argv[last], (2*cur) - 2);
//        printf("cur: %d, pid: %d, (2*cur) - 2: %d, fd[(2*cur) - 2]: %d\n", cur, getpid(), (2*cur) - 2, fds[(2*cur) - 2]);
        if((dup2(fds[(2*cur) - 2], STDIN_FILENO)) == -1){   //overwrite stdin with read pipe
            perror("dup2: Error 1");
            exit(1);
        }
        for(int j = 0; j < totFds; j++){//we created 2 x pipes fds, goes up until index (2* pipes) - 1
//            printf("Closing j: %d\n", j);
            close(fds[j]);
        }

        pExec(argv[last], &argv[last]);
    }
//===============================================Every other process changes both stdin/out
    else if(pid == 0){
        //overwrite stdin with read pipe

//        printf("Not first or last, Process: %d\n", cur);
//        printf(">inFd: %d, outFd: %d\n",inFd + ((cur-1)*2),outFd + ((cur-1)*2));


        if((dup2(fds[inFd + ((cur-1)*2)], STDIN_FILENO)) ==-1){
            perror("dup2: Error 3");
            exit(1);
        }
        //overwrite stdout with write pipe
        if((dup2(fds[outFd + ((cur-1)*2)], STDOUT_FILENO))== -1){
            perror("dup2: Error 4");
            exit(1);
        }

        for(int j = 0; j < totFds; j++){//we created 2 x pipes fds, goes up until index (2* pipes) - 1
            close(fds[j]);   //close everything, we've already duped what we need
        }
//        printf("%s, %s\n", argv[last], argv[last + 1]);
        pExec(argv[last], &argv[last]);
    }

    //child WILL NEVER GET DOWN HERE





    for(int i = 0; i < processCount; i++){
        waitpid(arr[i],&status,0);
    }

//    printf("Parent: %d, Finished Waiting\n", getpid());


//    printf("%d", fds[0]);
    //fork
    //if child (0), fork again
    //exec process1 from child1
    //cut off read end
    //write
    //if child2 (0), execute process 2
    //cut off write end
    //read
//    exit(0);
    return 0;
}