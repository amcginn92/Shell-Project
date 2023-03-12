#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "helpers.h"
char** parse(char* line,char* delim){

        char** array=malloc(sizeof(char*));
        *array=NULL;
        int n = 0;

        char*buf = strtok(line,delim);

        if (buf == NULL){
                free(array);
                array=NULL;
                return array;
        }

        while(buf!=NULL ){

                char**temp = realloc(array,(n+2)*sizeof(char*));

                if(temp==NULL){
                        printf("Temp: %s\n", *temp);
                        free(array);
                        array=NULL;
                        return array;
                }

                array=temp;
                temp[n++]=buf;
                temp[n]=NULL;

                buf = strtok(NULL,delim);

        }

        return array;
}
/*Returns index of first instance of char * special*/
int find_special (char*args[], char * special){

        int i = 0;
        while(args[i]!=NULL){
                if(strcmp(args[i],special)==0){
                        return i;
                }
                i++;
        }
        return -1;
}

/*
 * Returns an input stream to be used depending on if the shell is launched in interactive / batch mode
 * Best used with getline https://man7.org/linux/man-pages/man3/getline.3.html
 */
FILE *getInput(int argc, char* argv[]){
        //if the argument count is 2, attempt to open the file passed to the shell
        FILE *mainFileStream = NULL;
        if(argc == 2){
                mainFileStream = fopen(argv[1], "r");
                if(mainFileStream == NULL){
                        printf("Error opening batch file\n");
                        exit(1);
                }
        }
                //set the file stream to standard input otherwise
        else if(argc ==1){
                mainFileStream = stdin;
        }
        else{
                printf("Too many arguments\n");
                exit(1);
        }
        return mainFileStream;
}

