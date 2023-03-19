#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	if(argc > 1){
		int i = 1;
		while(i < argc){
			printf("%s ", argv[i]);
			i++;
		}
		puts("");

	}else{
		puts("Nothing really");
	}
}
