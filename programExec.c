#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "helpers.h"


/* This program allows us to execute a path with arguments passed from main.
 * We first implemented the functionality to run full paths, determining their
 * validity by checking for paths that start with '/', which would imply an
 * absolute path. This is done by forking and executing the given aboslute path.
 *
 * Further, we decided to implement functionality to run common
 * commands from the PATH environment variable directories. This could be
 * implemented using the execvp function, but since we are limited to using
 * other exec functions, we must rely on obtaining the directories in the
 * PATH variable using getenv(), then parsing the string of colon seperated
 * values. We then check each directory for the given function, and if found
 * run the first instance found. 
 */
int programExec(char* path, char* argv[]){
    char envPath[4096];
    int status = 0;
    char newPath[4096];
    int flag = -1;
    char* token, *cont; //used to parse the PATH env variable
    struct dirent **namelist;   //Used to check each PATH directory
    int n;  //number of files inside directories


//    printf("Program Exec Test: %s, %s\n", path, argv[1]);




    //first word is program name, rest are argv
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
        //if the path doesn't start with a '/' we search the path variables as instructed
        if(*path != '/' && *path != '.'){   //I've decided to take relative paths as well from './' paths
            flag = 1;
//            printf("ENV PATH: %s\n\n", getenv("PATH"));
            strcpy(envPath, getenv("PATH"));
            cont = envPath;
//            printf("%s\n", envPath);

                //TOKEN IS CUR PATH
                while((token = strtok_r(cont,":", &cont))){ //from G4G.com
                    n = scandir(token, &namelist,NULL,alphasort);   //get current dir files
                    //check scandir return value, if the directory doesn't exist, we just skip it (dir added don't always exist)
                    if(n == -1) {
                        continue;
                    }
                    while (n--) {
//                        printf("-%s\n", namelist[n]->d_name);
                        if(strcmp(path, namelist[n]->d_name) == 0){
                            strcpy(newPath, token);
                            strcat(newPath, "/");
                            strcat(newPath, path);
                        }
                        free(namelist[n]);
                    }

                }
        }

        //**HAVE TO CHECK IF NOTHING WAS FOUND**


        //we will do this by parsing the environment variable PATH using strtok_r looking for colons
        //The folders we find will each have to be checked for the program we intend to execute
        //if we find the program, we copy out the path leading up to it and use that
        //**WE ONLY DO THIS IF THE PROGRAM IS NOT A SYMLINK OR ABSOLUTE PATH**
        //We will reuse code from project A to do this



        if(flag == 1){  //if we do not have an absolute path, use the new constructed path
            if(execv(newPath, argv) == -1){
                perror("Execv in programExec.c");
                exit(1);
            }
        }else{
//            puts("Should print this");
            if(execv(path, argv) == -1){
                perror("Execv in programExec.c");
                exit(1);
            }


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

//==================================================================Exec without forking
int pExec(char* path, char* argv[]){
    char envPath[4096];
    char newPath[4096];
    int flag = -1;
    char* token, *cont; //used to parse the PATH env variable
    struct dirent **namelist;   //Used to check each PATH directory
    int n;  //number of files inside directories

//    printf("Program Exec Test: %s, %s\n", path, argv[1]);

    //first word is program name, rest are argv
    //assume first word to be full path to the program ** at first**
        //at first we will just give the full path of the program we want to run
        //Next we want to make it so it will run programs with simple path
        //if the path doesn't start with a '/' we search the path variables per instructions
        if(*path != '/' && *path != '.'){
            flag = 1;
            strcpy(envPath, getenv("PATH"));
            cont = envPath;
//            printf("%s\n", envPath);

            //TOKEN IS CUR PATH
            while((token = strtok_r(cont,":", &cont))){ //from G4G.com
                n = scandir(token, &namelist,NULL,alphasort);   //get current dir files
                //check scandir return value, if the directory doesn't exist, we just skip it (dir added don't always exist)
                if(n == -1) {
                    continue;
                }
                while (n--) {
//                        printf("-%s\n", namelist[n]->d_name);
                    if(strcmp(path, namelist[n]->d_name) == 0){
                        strcpy(newPath, token);
                        strcat(newPath, "/");
                        strcat(newPath, path);
                    }
                    free(namelist[n]);
                }
            }
        }
        //**HAVE TO CHECK IF NOTHING WAS FOUND**


        //we will do this by parsing the environment variable PATH using strtok_r looking for colons
        //The folders we find will each have to be checked for the program we intend to execute
        //if we find the program, we copy out the path leading up to it and use that
        //**WE ONLY DO THIS IF THE PROGRAM IS NOT A SYMLINK OR ABSOLUTE PATH**
        //We will reuse code from project A to do this

//        int i = 0;
//        puts("Begin");
//        while(argv[i] != NULL){
//            printf("%s\n", argv[i]);
//            i++;
//        }

        if(flag == 1){  //if we do not have an absolute path, use the new constructed path
            if(execv(newPath, argv) == -1){
                perror("Execv in programExec.c");
                exit(1);
            }
        }else{
            if(execv(path, argv) == -1){
                perror("Execv in programExec.c");
                exit(1);
            }

        }

//    free(namelist);	//namelist if allocated via malloc() and must be freed

    return(0);
}

