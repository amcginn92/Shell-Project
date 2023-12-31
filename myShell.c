#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "helpers.h"
//====================================Prototypes
void help(char** arr, int i);
int runBuiltIn(char** arr, int i);
int cd(char** arr, int i);
void pwd();
int wait();
int myWait();
void check(flags* myFlags);
//==================================
/* myShell

This shell implements some features for navigating file systems like pwd, cd, and executes programs.
It has additional features, such redirecting input and output to files in the current folder, piping
input and output between multiple processes, and executing files in the local PATH environment. Built
-in functions are:

* wait - waits for all child processes to finish and reap their zombies

* pwd - prints the current path working directory to screen

* cd - allows you to change to the given directory within your file system

* help - prints this help manual

* exit - exits from the shell

 * /
//=============TEST CASES
  * dir (good)
  * pwd (good)
  * ls | wc (good)
  * ls > file (good)
  * echo "io" (good)
  * wc < in.txt (good)

  * ls | grep myShell (good)
  * ls | grep myShell | wc (good)
  * ls | grep myShell | sleep 5 & (good)
  * sleep 5 & (good)
  * sleep 2 (good)
  * exit (good)
 */
int main(int argc, char* argv[]){
	char cwd[4096];	//used to display cwd to user in prompt
//	char _line[1024]; //no longer needed
	char* _line = malloc(128 * sizeof(char));
	char** arr = NULL;	//parsed input values separated into strings
	flags myFlags = {0};
	int exitFlag = 0;
	size_t size = 1024;
	int status = 0;
	int i = 0; //gets count of command line arguments


//	FILE* fp = getInput(argc, argv);	//batch mode



	//continue taking input from user until exit is given
	while(1){

//		printf("How did I end up here? Pid: %u\n", getpid());

		myFlags.in = 0; myFlags.out = 0; myFlags.amp = 0; myFlags.pipe = 0;	//HAVE TO RESET VALUES FOR EACH LOOP
		if(argc == 1){
			printf("myshell-client:~%s>", getcwd(cwd, 4096));	//would be cool if we had cwd here
		}

//		printf("PID: %d\n\n", getpid());

		if( (getline(&_line,&size, stdin)) == -1){
			perror("Getline failed in myShell.c");
			exit(0);
		}

//			fgets(_line,1024, fp);	//was using fgets before batch mode *holding onto just to be safe*
//			line = strdup(_line);

		//use 'parse' function given to use to parse the user input
		arr = parse(_line," \n");


		if (arr==NULL){
			exit(1);
		}
		//get a count of arguments
		while (arr[i]!=NULL){ i++; }


		//determine if built-in, if yes run it, if not (returns 1) we exec the program
		if( (exitFlag = runBuiltIn(arr, i))){	//runBuiltIn returns 42 if 'exit' command was given
			if(exitFlag == 42){
				free(arr);
				exit(0);
			}

			//before we execute the program we need to determine if there is a special character
			//using 'find_special' we can get the first index of a special character
//			Determine which special chars we have, if any (helpers.c)
			which_special(arr, &myFlags);
			check(&myFlags);






			//REDIRECTION
			//if it is a '>' we will execute a function for redirection
			//redirect(char* file1, char* file2, char dir), where dir stands for the direction
			if(myFlags.out != 0 || myFlags.in != 0){	//if we have redirection somewhere
				//the arguments for input and output are after the indices we saved for the redirection operators
//				puts("Redirect");
				redirect(arr[0], arr, myFlags.in, myFlags.out);
			}
			//PIPES
			//if it is a '|' we will execute a function for piping
			else if(myFlags.pipe == 1){
//				printf("Pipes: %d\n", myFlags.pipe);

				//we will do this in a while loop, continually checking for the next instance of a pipe
				myPipe(arr);
			}
			//Execute program, Check path variables
			else{
//				puts("ProgramExec");
				programExec(arr[0], arr);
			}
		}

		free(arr);


		//regularly check for processes to reap without interrupting process flow
		waitpid(-1, &status, WNOHANG);	//no error check since we expect to not find children
		//could check for more improtant errors but seems outside of scope of this project

	}
	return 0;
}
//====================================================RUN BUILT INS
/*runBuiltIn will determine if the input was a built in function. If not we execute
 *a program instead. We return 1 if */
int runBuiltIn(char** arr, int i){
//	printf("%s\n", arr[0]);
	if(strcmp(arr[0], "exit") == 0 && arr[1] == NULL){
//		printf("RunBuiltIn: \'%s\'\n", arr[0]);
		return(42);
	}else if(strcmp(arr[0], "help") == 0 && arr[1] == NULL){
		help(arr,i);
		return(0);
	}else if(strcmp(arr[0], "pwd") == 0 && arr[1] == NULL){
		pwd();
		return(0);
	}else if(strcmp(arr[0], "cd") == 0 && arr[2] == NULL){
		cd(arr, i);
		return(0);
	}else if(strcmp(arr[0], "wait") == 0 && arr[1] == NULL){
		myWait();
//		puts("You're gonna be waiting a long time for that one..");
		return(0);
	}
	return(1);	//we don't have a builtin
}
//====================================================HELP
void help(char** arr, int i){

	FILE* fp;
	char ch;
	fp = fopen("help.txt", "r");
	ch = fgetc(fp);
	while(ch != EOF){
		printf("%c", ch);
		ch = fgetc(fp);
	}
	//causing problems implemented for no reason
//	if(i == 1){
//		helpShell();//only built-in with 1 arg
//	}else if(strcmp(arr[1], "exit") == 0){
//		helpExit();
//	}else if(strcmp(arr[1], "help") == 0){
//		helpMan();
//	}else if(strcmp(arr[1], "pwd") == 0){
//		helpPwd();
//	}else if(strcmp(arr[1], "cd") == 0){
//		helpCd();
//	}else if(strcmp(arr[1], "wait") == 0){
//		myWait();
//	}else{
//		printf("Built-ins aren't supposed to do that...\n");
//		exit(1);
//	}
}
//====================================================PWD
void pwd(){
	char arr[4096];

	printf("%s\n", getcwd(arr,4096));
	if( arr == NULL){
		perror("PWD in myShell");
		exit(1);
	}
}
//====================================================CD
int cd(char** arr, int i){
	if(i == 1){
		return(0);	
	}
	char cwd[1024];
	if( (getcwd(cwd, 1024)) == NULL){
		perror("getcwd in cd built-in");
		exit(1);
	}

	if(chdir(arr[1]) == -1){
		perror("chdir in myShell");
	}
	getcwd(cwd, 1024);

	return(0);

}
//==========================================================Wait
int myWait(){

	//waits until all processes have finished if 'wait' has been entered
	//use a while loop, reading return values for wait

	while(waitpid(-1,NULL,0) > 0);

	return(0);
}



