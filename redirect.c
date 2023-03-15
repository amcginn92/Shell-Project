#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "helpers.h"

int redirect(char* file1, char* file2, char dir){

    //determine direction of redirection
    //Either if file1 > file2 else file1 < file2, choose a path

    //We fork, use dup to save stdout for our process
    //Open the file to get  the fd
    //Use dup2 to redirect the output of the process to the file instead
    //Restore stdout
    //close file

    return(0);
}