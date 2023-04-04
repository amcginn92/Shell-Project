#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "helpers.h"

//parse returns the array of characters as
//a 2d array of each word separated by spaces
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
                        free(array);
                        array=NULL;
                        return array;
                }

                array=temp;
                temp[n++]=buf;
                temp[n]=NULL;

                buf = strtok(NULL,delim);

        }

        return array;   //needs to be freed in main
}
/* Takes in arguments, determines if there are any special characters,
 * returns which one is encountered, if any, or 0 if none
 */
void which_special(char* args[], flags *myFlags){
        int out = find_special(args,">");
        int in = find_special(args,"<");
        int pipe = find_special(args, "|");
        int amp = find_special(args, "&");

        if(out != -1){
//                printf("Out: %s, ", args[out]);
                myFlags->out = out;
        }
        if(in != -1){
//                printf("In: %s\n", args[in]);
                myFlags->in = in;
        }
        if(pipe != -1){
//                printf("Pipe: %s, ", args[pipe]);
                myFlags->pipe = pipe;
        }

        if(amp != -1){
//                printf("Amp: %s, ", args[amp]);
                myFlags->amp = amp;
        }
        //otherwise return -1

}

//=========================================================Check
/* check determines if we have an incorrect input such as output or input
 * redirection and piping  */
void check(flags* myFlags){
//	check for pipes and redirect
        if( (myFlags-> out && myFlags->pipe) || (myFlags->in && myFlags->pipe) ){
                puts("Cannot implement pipes and redirection");
                exit(1);
        }

}
/*Returns index of first instance of char* special*/
int find_special (char*args[], char * special){

        int i = 0;
        while(args[i]!=NULL){
//                printf("\'%s\'\n", args[i]);
                if(strcmp(args[i],special)==0){
//                        printf("Returning: %d", i);
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
//returns the number of pipes given, up to 512 as was discussed in lab
int pipeCount(char* argv[]){
        int i = 0;
        int pCount = 0;
        //count the number of pipes
        while(argv[i] != NULL){
                //strcmp returns 0 if equal, so we take the opposite
                if(!strcmp(argv[i],"|")){
                        pCount++;
                }
                i++;
        }
        return pCount;
}

