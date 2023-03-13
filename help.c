#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "helpers.h"

void helpMan(){
    puts("HELP(1)\n\n");
    puts("NAME");
    puts("\thelp - interfaces to the myShell user manuals.\n\n");
    puts("DESCRIPTION");
    puts("\thelp allows a user to find manuals for myShell, as well as find information for the "
         "\n\tmultitude of command line actions that it implements\n");
}
void helpShell(){
    puts("myShell\n\n");
    puts("Custom Shell Implementation");
    puts("\tmyShell is a custom implementation of a linux shell. It was developed by a huge nerd...\n\n");
    puts("DESCRIPTION");
    puts("\tmyShell allows users to navigate a file system, request current position, request manuals "
         "\n\tfor features, and exit from the current instance using built in commands."
         "\n\tIt also allows program execution, redirection, and the ability to run programs in the background.\n");
}
void helpExit(){
        puts("EXIT(1)\n\n");
        puts("NAME");
        puts("\texit - terminates the current instance of myShell\n\n");
        puts("DESCRIPTION");
        puts("\texit allows the user to discard the current myShell instance with a successful status (0).\n");
}
void helpPwd(){
    puts("PWD(1)\n\n");
    puts("NAME");
    puts("\tpwd - Informs the user of the current place in the working directory.\n\n");
    puts("DESCRIPTION");
    puts("\tpwd will return the current working directory using getcwd library function.\n");
}
void helpCd(){
    puts("CD(1)\n\n");
    puts("NAME");
    puts("\tcd - Changes the current working directory of the current user.\n\n");
    puts("DESCRIPTION");
    puts("\tcd uses getcwd to find the current working directory and chdir system call to change"
         "\n\tthe current working directory of the active user.\n");
}