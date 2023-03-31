#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "helpers.h"
//====================================Prototypes
void help(char** arr, int i);
int runBuiltIn(char** arr, int i);
int cd(char** arr, int i);
void pwd();
int wait();
void check(flags* myFlags);
//==================================

/* //
 * NEEDS PROGRAM DESCRIPTION
 */

/*
//=============TEST CASES
  * dir (good)
  * pwd (good)
  * ls | wc
  * ls > file (good)
  * echo "file"
  * wc < file

  * ls | grep shell
  * ls | grep shell | wc
  * ls | grep shell | sleep 5 &
  * sleep 5 &
  * sleep 2
  * exit
 *
 * LEFT OFF: Need to do a fresh install to ensure programExec.c is working
 *
 */
int main(int argc, char* argv[]){
	char cwd[4096];	//used to display cwd to user in prompt
//	char _line[1024]; // input from user //** IF INPUT IS FAILING CHECK GETLINE **
	char* _line = malloc(128 * sizeof(char*));
	char** arr = NULL;	//parsed input values separated into strings
	char* line = NULL;
	flags myFlags = {0};
	int exitFlag = 0;
	size_t size = 1024;
	int i = 0; //gets count of command line arguments
	//continue taking input from user until exit is given

	FILE* fp = getInput(argc, argv);	//batch mode




	while(1){
		myFlags.in = 0; myFlags.out = 0; myFlags.amp = 0; myFlags.pipe = 0;	//HAVE TO RESET VALUES FOR EACH LOOP
		if(argc == 1){
			printf("myshell-client:~%s>", getcwd(cwd, 4096));	//would be cool if we had cwd here
		}

		if( (getline(&_line,&size, fp)) == -1){
			perror("Getline failed in myShell.c");
			exit(0);
		}
//			fgets(_line,1024, fp);	//was using fgets before batch mode *holding onto just to be safe*
		line = strdup(_line);

		//use 'parse' function given to use to parse the user input
		arr = parse(line," \n");

		if (arr==NULL){
			exit(1);
		}
		//get a count of arguments
		while (arr[i]!=NULL){ i++; }

		//determine if built-in, if yes run it, if not (returns 1) we exec the program
		//**NEED TO VERIFY CORRECT NUMBER OF ARGUMENTS FOR BUILT INS**
		if( (exitFlag = runBuiltIn(arr, i))){	//runBuiltIn returns 42 if 'exit' command was given
			if(exitFlag == 42){
				puts("");
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
			}else{
//				puts("ProgramExec");
				programExec(arr[0], arr);
			}


			//PIPES
			//if it is a '|' we will execute a function for piping
			//we will do this in a while loop, continually checking for the next instance of a pipe
			//pipe(char* file1, char** args1, char* file2, char** args2)
			if(myFlags.pipe == 1){
//				printf("Pipes: %d\n", myFlags.pipe);
			}

			//BACKGROUND PROCESSES
			//if special character is a & we will run programExec without waitPid flag (to be added)
			if(myFlags.amp == 1){
				printf("Ampersand: %d\n", myFlags.amp);
			}
//			printf("From Main: %s, %s\n", arr[0], arr[1]);


		}

		free(arr);
		free(line);
	}
	return 0;
}
//====================================================RUN BUILT INS
/*runBuiltIn will determine if the input was a built in function. If not we execute
 *a program instead. We return 1 if */
int runBuiltIn(char** arr, int i){

	if(strcmp(arr[0], "exit") == 0){
//		printf("RunBuiltIn: \'%s\'\n", arr[0]);
		return(42);
	}else if(strcmp(arr[0], "help") == 0){
		help(arr,i);
		return(0);
	}else if(strcmp(arr[0], "pwd") == 0){
		pwd();
		return(0);
	}else if(strcmp(arr[0], "cd") == 0){
		cd(arr, i);
		return(0);
	}else if(strcmp(arr[0], "wait") == 0){
		puts("You're gonna be waiting a long time for that one..");
		return(0);
	}
	return(1);
}
//====================================================HELP
void help(char** arr, int i){
	if(i == 1){
		helpShell();
	}else if(strcmp(arr[1], "exit") == 0){
		helpExit();
	}else if(strcmp(arr[1], "help") == 0){
		helpMan();
	}else if(strcmp(arr[1], "pwd") == 0){
		helpPwd();
	}else if(strcmp(arr[1], "cd") == 0){
		helpCd();
	}else if(strcmp(arr[1], "wait") == 0){
		puts("Not just yet..");
		//implement helpWait()
	}
}
//====================================================PWD
void pwd(){
	char arr[4096];
	printf("%s\n", getcwd(arr,4096));
}
//====================================================CD
int cd(char** arr, int i){
	if(i == 1){
		return(0);	
	}
	char cwd[1024];
	getcwd(cwd, 1024);
//	printf("%s\n", cwd);
	if(chdir(arr[1]) == -1){
		perror("chdir in myShell");
	}
	getcwd(cwd, 1024);
//	printf("%s\n", cwd);

	return(1);

}
//==========================================================Wait
int wait(){
	//waits until all processes have finished if 'wait' has been entered
	//use a while loop, reading return values for wait
	return(0);
}



