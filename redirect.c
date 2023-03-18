#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "helpers.h"

int redirect(char* file1, char* file2, char dir){
    int saveStdout = -1;
    //Assume first argument is a process
    struct stat st1, st2;
    if(stat(file1,&st1) == -1){
        perror("stat1 in redirect");
        exit(1);
    }
    if(stat(file2,&st2) == -1){
        perror("stat1 in redirect");
        exit(1);
    }
    //st_mode values: S_IFREG (REGULAR FILE), S_IFLNK (SYMLINK), S_IFDIR (DIR)
    //Verify file types
    //**WRITE ABOUT THIS IS TESTING IF ACTUALLY NEEDED**
    if(!(st1.st_mode == S_IFREG)){
        printf("Incorrect File Type");
        exit(1);
    }
    if(!(st2.st_mode == S_IFREG)){
        printf("Incorrect File Type");
        exit(1);
    }
    //Use dup to save stdout for our process
    saveStdout = dup(1);
    //We fork
    //Open the file to get  the fd
    //Use dup2 to redirect the output of the process to the file instead
    //Restore stdout
    //close file

    printf("%d", saveStdout);

    return(0);
}