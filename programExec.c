#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "helpers.h"

int programExec(char* path, char* args[]){
    char envPath[4096];
    int status = 0;
    char* token, *cont; //used to parse the PATH env variable
    struct dirent **namelist;   //Used to check each PATH directory
    int n;  //number of files inside directories

    //first word is program name, rest are args
    //assume first word to be full path to the program ** at first**
    //** later ** implement simple program names
    pid_t pid;
    //fork a child and get its pid
    pid = fork();
    //determine if parent or child
    if(pid == 0){    //it's child
        //if it's the child we should execv
        //at first we will just give the full path of the program we want to run
        //Next we want to make it so it will run programs with simple path
        //if the path doesn't start with a '/' we search the path variables
        if(*path != '/'){
            strcpy(envPath, getenv("PATH"));
            cont = envPath;
//            printf("%s\n", envPath);

                //TOKEN IS CUR PATH
                while((token = strtok_r(cont,":", &cont))){ //from G4G.com
                    //=========================================================
                    printf("%s\n", token);
                    //Check if it exists? Why wouldn't it
                    n = scandir(token, &namelist,NULL,alphasort);   //get current dir files
                    if(n == -1) {	//check scandir return value
                        perror("scandir");
                        exit(EXIT_FAILURE);
                    }
                    while (n--) {
                        if(*namelist[n]->d_name == '.'){		//we want to ignore '.'
                            continue;	//go to next loop, since n-- is the conditional statement it should go next
                        }
                            printf("-%s\n", namelist[n]->d_name);

                    }
                    free(namelist);	//namelist if allocated via malloc() and must be freed
//=========================================================
//                    printf("%s\n", token);
                }
        }


        //we will do this by parsing the environment variable PATH using strtok_r looking for colons
        //The folders we find will each have to be checked for the program we intend to execute
        //if we find the program, we copy out the path leading up to it and use that
        //**WE ONLY DO THIS IF THE PROGRAM IS NOT A SYMLINK OR ABSOLUTE PATH**
        //We will reuse code from project A to do this
        if(execv(path, args) == -1){
            perror("Execv in programExec.c");
            exit(1);
        }
    }else if(pid > 0){  //it's the parent
        //wait for child to  complete
        waitpid(pid, &status, 0);
    }else{
        perror("This is why you should never fork a child(programExec.c)");
        exit(1);
    }

    return(0);
}