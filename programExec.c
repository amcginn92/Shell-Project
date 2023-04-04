#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "helpers.h"


/* Input: Program path, Array of strings with program path and arguments
 * Output: Returns 0 on success
 *
 * programExec allows us to execute a program that is in one of our PATH
 * folders. We first attempt to run a program with an absolute/relevant path,
 * and if this is not the case, we use the PATH environment variable to search
 * for local programs to run. This of course requires us to parse the string of
 * colon seperated values. We then check each directory for the given function,
 * and if found, we run the first instance found. We fork and execute the program
 * to allow for background processes to run while we continue executing commands
 * in the shell. This is also implemented in the programExec function, depending
 * if the array of Strings contains the background character at the end.
 */
int programExec(char* path, char* argv[]){
    char envPath[4096];
    int status = 0;
    char newPath[4096];
    newPath[0] = '\0';
    int flag = -1;
    char* token, *cont; //used to parse the PATH env variable
    struct dirent **namelist;   //Used to check each PATH directory
    int n;  //number of files inside directories
    int bgIndex = 0;
    int bgRun = 0;
    int k = 0;
    while(argv[k] != NULL){
        k++;
    }
    //k has the location of NULL

//    printf("Program Exec Test: %s, %s\n", path, argv[1]);
    if( (bgIndex = find_special(argv, "&")) != -1){ //we have a bg character
        if(bgIndex +1 != k){    //bg has the index of &, so null MUST be next
            printf("Background character must be the last!!");
            exit(1);
        }else{  //if & is last character, we set a bool for runnign bg process
            bgRun = 1;
//            printf("%s\n", argv[bgIndex]);
            argv[bgIndex] = NULL;
        }
    }

    //first word is program name, rest are argv
    //assume first word to be full path to the program ** at first**
    //** later ** implement simple program names
    pid_t pid;
    //fork a child and get its pid
    if( (pid = fork()) == -1){
        perror("This is why you should never fork a child(programExec.c)");
        exit(1);
    }


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
                        //if the file we're looking for exists inside a directory
                        //concatenate the new path  with the file name
                        if(strcmp(path, namelist[n]->d_name) == 0){
                            strcpy(newPath, token);
                            strcat(newPath, "/");
                            strcat(newPath, path);
                        }
                        free(namelist[n]);  // must free each array element returned from scandir
                    }

                }
        }



        //we will do this by parsing the environment variable PATH using strtok_r looking for colons
        //The folders we find will each have to be checked for the program we intend to execute
        //if we find the program, we copy out the path leading up to it and use that
        //**WE ONLY DO THIS IF THE PROGRAM IS NOT A SYMLINK OR ABSOLUTE PATH**
        //We will reuse code from project A to do this


        //if we do not have an absolute/relative path, use the new constructed path
        if(flag == 1 && *newPath == '\0'){
            printf("%s: Command not found\n", path);
            return(1);  //return unsuccessful
        }else if(flag == 1){
            if(execv(newPath, argv) == -1){
                perror("Execv in programExec.c");
                exit(1);
            }
        }
        else{
//            puts("Should print this");
            if(execv(path, argv) == -1){
                perror("Execv in programExec.c");
                exit(1);
            }


        }
    }else if(pid > 0){  //it's the parent
        //wait for child to  complete
        if(bgRun == 0){ //we don't have a background character
            if( (waitpid(pid, &status, 0)) == -1){
                perror("Waitpid in programExec");
                exit(1);
            }
        }else{
            if( (waitpid(pid, &status, WNOHANG)) == -1){
                perror("Waitpid in programExec WNOHANG");
                exit(1);
            }
        }
        //otherwise we have a bg process and won't wait

        }

        return(0);
}

//==================================================================Exec without forking
//Input: same as programExec
//Output: Same as programExec
//This function only differs from program exec in that it does not fork
//and execute, and also does not implement its own background processing.
//This was needed for piping, as processes that are being run
//have already splintered from their parent and no longer require forking
//and background processes are handled locally in that function.
int pExec(char* path, char* argv[]){
    char envPath[4096];
    char newPath[4096];
    int flag = -1;
    char* token, *cont; //used to parse the PATH env variable
    struct dirent **namelist;   //Used to check each PATH directory
    int n;  //number of files inside directories

    //    printf("Program Exec Test: %s, %s\n", path, argv[1]);


    //PSEUDOCODE
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


        //PSEUDOCODE
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
            if(execv(path, argv) == -1){
                perror("Execv in programExec.c");
                exit(1);
            }

        }


    return(0);
}

