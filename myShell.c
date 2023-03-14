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
		arr = parse(line," \n");

		if (arr==NULL){
			exit(1);
		}
		//get a count of arguments
		int i = 0;
		while (arr[i]!=NULL){
//			printf("arr[%d]: ", i);
//			printf("%s\n", arr[i++]);
			i++;
		}
		//determine if input was a built in
		if(i > 2){	//we should not have more than 2 arguments for built ins
			printf("Too many arguments for now\n");
			exit(1);
		}
		runBuiltIn(arr, i);

		//if it is not a built-in, run program
		//programExec();



		free(arr);
		free(line);


	}



	return 0;
}
//=====================================Functions

//====================================================RUN BUILT INS
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


