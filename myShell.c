#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "helpers.h"

int main(int argc, char* argv[]){

	char _line[1024];
	char** array = NULL;
	char* line = NULL;
	//continue taking input from user until exit is given
	int j = 1;
	while(1){
		printf("myshell-client:~/cwd%d> ", j);	//would be cool if we had cwd here
		fgets(_line,1024, stdin);

		line = strdup(_line);	//We create a copy so parse doesn't change our arguments
		array = parse(line," \n");

		if (array==NULL)
			exit(1);

		int i = 0;
		while (array[i]!=NULL){
			printf("%s\n",array[i++]);
		}
		

		free(array);
		free(line);

//		if(strcmp(_line, "exit") == 0){
//			return(0);
//		}else if(strcmp(_line, "help") == 0){
//
//		}
	}



	return 0;
}


