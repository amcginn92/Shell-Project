#define LINE_MAX 4096 
#ifndef MAKEFILE_MYPS_H
#define MAKEFILE_MYPS_H
char ** parse(char*line,char*delim);
int find_special (char*args[], char * special);
FILE *getInput(int argc, char* argv[]);
void helpMan();
void helpShell();
void helpExit();
void helpPwd();
void helpCd();
int programExec(char* path, char* argv[]);
int myPipe(char* file1, char** args1, char* file2, char** args2);
int redirect(char* file1, char* file2, char dir);
#endif