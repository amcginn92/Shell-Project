#include <stdio.h>
#include <stdlib.h>

int main(){
	size_t len = 1024;
	char* arr;
	arr = malloc(len);

	while(getline(&arr, &len, stdin) != -1){
		printf("%s", arr);
	}

	free(arr);
}
