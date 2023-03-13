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

	char _line[1024];
	char** arr = NULL;
	char* line = NULL;
	//continue taking input from user until exit is given
	int j = 1;
	while(1){
		printf("myshell-client:~/cwd%d> ", j);	//would be cool if we had cwd here
		fgets(_line,1024, stdin);

		line = strdup(_line);	//We create a copy so parse doesn't change our arguments
		arr = parse(line," \n");

		if (arr==NULL)
			exit(1);

		int i = 0;
		while (arr[i]!=NULL){
//			printf("arr[%d]: ", i);
//			printf("%s\n", arr[i++]);
			i++;
		}
//		printf("%d\n", i);

		if(i > 2){	//we should not have more than 2 arguments
			printf("Too many arguments for now\n");
			exit(1);
		}

		runBuiltIn(arr, i);


		free(arr);
		free(line);


	}



	return 0;
}
//=====================================Functions
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
void pwd(){
	char arr[1024];
	printf("%s\n", getcwd(arr,1024));
}
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


