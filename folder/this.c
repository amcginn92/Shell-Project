#include <stdio.h>
#include <stdlib.h>

int main(){
	size_t len = 1024;
	char* arr;
	malloc(len);

	getline(&arr, &len, stdin);
	printf("%s", arr);
}
