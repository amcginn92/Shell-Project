#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "helpers.h"
//====================================Prototypes
void help(char** arr, int i);
int runBuiltIn(char** arr, int i);
int cd(char** arr, int i);
void pwd();
int wait();
//==================================
int main(int argc, char* argv[]){
	char cwd[4096];	//used to display cwd to user in prompt
	char _line[1024]; //Input from user
	char** arr = NULL;	//parsed input values separated into strings
	char* line = NULL;
	//continue taking input from user until exit is given
//	int j = 1;
	while(1){
		printf("myshell-client:~%s>", getcwd(cwd, 4096));	//would be cool if we had cwd here
		fgets(_line,1024, stdin);

		line = strdup(_line);
		//use 'parse' function given to use to parse the user input
		arr = parse(line," \n");

		if (arr==NULL){
			exit(1);
		}
		//get a count of arguments
		int i = 0;
		while (arr[i]!=NULL){ i++; }

		//determine if built-in, if yes run it, if not (returns 1) we exec program
		if(runBuiltIn(arr, i)){
			//before we execute the program we need to determine if there is a special character
			//using 'find_special' we can get the first index of a special character
			//if it is a '|' we will execute a function for piping
			//we will do this in a while loop, continually checking for the next instance of a pipe
			//pipe(char* file1, char** args1, char* file2, char** args2)

			//if it is a '>' we will execute a function for redirection
			//redirect(char* file1, char* file2, char dir) *dir stands for the direction

			//if special character is a & we will run programExec without waitPid flag (to be added)
//			printf("From Main: %s, %s\n", arr[0], arr[1]);
			programExec(arr[0], arr);
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
		exit(0);
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
		return(0);	//cd was only argument, do nothing
	}
	char cwd[1024];
	getcwd(cwd, 1024);
	printf("%s\n", cwd);
	if(chdir(arr[1]) == -1){
		perror("chdir in myShell");
	}
	getcwd(cwd, 1024);
	printf("%s\n", cwd);

	return(1);

}
//==========================================================Wait
int wait(){
	//waits until all processes have finished if 'wait' has been entered
	//use a while loop, reading return values for wait
	return(0);
}


