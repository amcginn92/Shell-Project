#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "helpers.h"

int myPipe(char* argv[]){
//    puts("test");
    int pipes = 0;
    pipes = pipeCount(argv);
    int* fds = malloc(512*sizeof(int*));
//    char buf;

    //pipe to get fds
    pipe(fds);  //picket fence kinda problem
    for(int i = 0; i < pipes; i++){

        if(pipe(&fds[2*i]) == -1){
            perror("Ya pipes are all outta whack in myPipe.c");
            exit(1);
        }
    }



////    process one doesn't have stdin redirected
//    if((pid = fork()) == -1){
//        perror("You got fork problems buddy!");
//        exit(1);
//    }else if(pid == 0){
//        close(fd[1]);   //close write end (i.e. 1)
//        while(read(fd[0],&buf, 1)
//
//    }else{
//        close(fd[0]);   //close read end (i.e. 0)
//        write[1]
//    }


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