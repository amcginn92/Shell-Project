#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "helpers.h"
/* Redirect allows us to provide alternate input/output for a
 * program to be executed. We take in the program to be executed,
 * program arguments, and if there were redirection operators, our
 * 'inFile' and 'outFile' will have the indices of them (otherwise 0)
 * in our argv array. We determine if there were input/output redirection,
 * open the files given after the operator, and then replace the STDIN/STDOUT
 * with the given files in the forked child program we execute.
 */


/*
 *  LEFT OFF WITH PROBLEM OF NOT 'SOMETHING' PROGRAM TAKING INPUT WHEN REDIRECTION OUTPUT
 *  USING SOMETHING WITH ARGS > OUT.TXT
 */



int redirect(char* path, char* argv[], int inFile, int outFile){
    int fd1 = 0, fd2 = 0;
    struct stat st1, st2;
    int saveStdin = 0, saveStdout = 0;
    char** pArgv; //we need to build a new argv without redirection operators etc
    pArgv = parseRedirect(argv, inFile, outFile);
//    printf("Redirect main: pArgv[last] = \"%s\" (last is outfile - 1)\nNull Check for end = %s\n", pArgv[outFile - 1], pArgv[outFile]);
//    printf("Redirect main: argv[out] \'%s\'\n", argv[outFile]);




    if(inFile != 0){
        inFile++;   //the file to be redirected to is following the redirection operators

        if( (stat(argv[inFile], &st1)) == -1){  //input must exist
            perror("Stat inFile: ");
            exit(1);    //don't want to return
        }
        if(!S_ISREG(st1.st_mode)){
            puts("inFile is not a regular file (redirect.c)");
        }
        if((fd1 = open(argv[inFile], O_RDONLY)) == -1){
            perror("Open inFile:");
            exit(1);
        }

//        printf("\nfd1: %u\n", fd1);

    }
    if(outFile != 0){
        outFile++;

        if( (stat(argv[outFile], &st2)) == -1){ //output doens't have to exist yet (i.e. ls > out.txt, we want to create file)
//            perror("Stat outFile: ");
//            exit(1);
        }
        if(!S_ISREG(st2.st_mode)){  // not needed anymore
//            puts("outFile is not a regular file (redirect.c)");
        }
        if((fd2 = open(argv[outFile], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXO | S_IRWXG | S_IRWXU)) == -1){
            perror("Open inFile:");
            exit(1);
        }

    }







    pid_t pid;

    pid = fork();
//    printf("pid: %u\n", pid);
    if(pid == 0){//child
//        printf("Child test: fd1: %u, fd2: %u\n", fd1, fd2);
        if(fd1 != 0){
            saveStdin = dup(STDIN_FILENO);
            dup2(fd1,STDIN_FILENO);
            close(fd1);
        }
        if(fd2 != 0){
            saveStdout = dup(STDOUT_FILENO);
            dup2(fd2,STDOUT_FILENO);
            close(fd2);
        }
//        printf("in fd: %d\n", fd1);
        if( (programExec(path, pArgv)) == -1){   // use our programExec program to run with PATH programs
            perror("execv in redirect");
            exit(1);
        }
        if(fd1 != 0){
            dup2(saveStdin, STDIN_FILENO);
            close(saveStdin);
        }
        if(fd2 != 0){
            dup2(saveStdout, STDOUT_FILENO);
            close(saveStdout);
        }
        exit(0);


    }else if(pid > 0){
//        puts("Parent test!\n");
        int status = 0;
        waitpid(pid, &status, 0);
    }

//    puts("Finished");
    return(0);
}
/* Return a new array that contains the process and all of
 * the arguments without the redirection operators and
 * files to redirect to. inFile and outFile are the indices
 * of our input/output redirection operators (or 0 if none) */
char** parseRedirect(char* argv[], int inFile, int outFile){
    char** pArgv;
    //determine the value of the last arguments
    int last = -1;  // location of our last argument
    if( (inFile != 0) && (outFile != 0) && inFile < outFile){   //we will use inFile as lowest indice, and one past the last argument
        last = inFile - 1;
    }else if(inFile == 0){
        last = outFile - 1;
    }else if(outFile == 0){
        last = inFile - 1;
    }else{//otherwise they're both nonzero and outfile is smaller
        last = outFile - 1;
    }


    pArgv = malloc(sizeof(char*));
    *pArgv = NULL;
    for(int i = 0; i <= last; i++){
        char** temp = realloc(pArgv, (i+2) * sizeof(char*));
        pArgv = temp;
        temp[i] = argv[i];
        temp[i+1] = NULL;
//        printf("i%d, argv[i]: %s, pargv[i]: %s\n", i, argv[i], pArgv[i]);
    }
//    printf("pArgv[last]: %s\n", pArgv[last]);



    return pArgv;    //just to avoid error ** NEEDS CHANGE, also need to FREE PARGV IN REDIRECT**
}