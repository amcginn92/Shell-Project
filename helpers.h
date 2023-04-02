#define LINE_MAX 4096 
#ifndef MAKEFILE_MYSHELL_H
#define MAKEFILE_MYSHELL_H

typedef struct{
    int in;
    int out;
    int pipe;
    int amp;
}flags;


char ** parse(char*line,char*delim);
int find_special (char*args[], char * special);
FILE *getInput(int argc, char* argv[]);
void helpMan();
void helpShell();
void helpExit();
void helpPwd();
void helpCd();
int pipeCount(char* argv[]);
int programExec(char* path, char* argv[]);
int myPipe(char* argv[]);
int redirect(char* path, char* argv[], int inFile, int outFile);
void which_special(char* args[], flags *myFlags);
char** parseRedirect(char* argv[], int inFile, int outFile);
void check(flags* myFlags);
int pExec(char* path, char* argv[]);
#endif