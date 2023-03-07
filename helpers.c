#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "helpers.h"

/*
  Function to break up a line (or any arbitrary string) into a memory allocated
  tokenized array of strings separated by the given delimiter or set of 
  delimiters (see strtok_r for more information about delim parameter). 
  
  Note: The array as well as the individual strings will be memory allocated,
  therefore they must be freed later. 
  
  Warning: LINE MUST BE MEMORY ALLOCATED!

  @param line : An arbitrary string
  @param args : An empty array to place the tokenized output
  @param delim : The delimiter or set of delimiters for separating 
    the line (see strtok_r for more information)
  
  @return argn : The length of the array
*/

int parse(char*line, char***_args, char*delim){
	
	*_args = malloc(sizeof(char*));
	char** args = *_args;
	
	
	if (line[(strlen(line)-1)]=='\n'){
		line[strlen(line)-1]='\0'; //clean trailing new line, WILL SEGFAULT IF LINE NOT MEMORY ALLOCATED!
	}
	
	char*buf, *ptr;
	int argn = 0;
	buf = strtok_r (line,delim,&ptr);
	args[argn++]=strdup(buf);
	
	while((buf=strtok_r(NULL,delim,&ptr))!=NULL){
		args[argn++]=strdup(buf);
	}
	
	args[argn]=NULL; 
	
	return argn; 

}

/*Returns index of first instance of char * special*/
int find_special (char*args[], char * special){

	int i = 0;
	while(args[i]!=NULL){
		if(strcmp(args[i],special)==0){
			return i;
		}
		i++;
	}
	return -1;
}

/*
 * Returns an input stream to be used depending on if the shell is launched in interactive / batch mode
 * Best used with getline https://man7.org/linux/man-pages/man3/getline.3.html
 */
FILE *getInput(int argc, char* argv[]){
    //if the argument count is 2, attempt to open the file passed to the shell
    FILE *mainFileStream = NULL;
    if(argc == 2){
        mainFileStream = fopen(argv[1], "r");
        if(mainFileStream == NULL){
            printf("Error opening batch file\n");
            exit(1);
        }
    }
    //set the file stream to standard input otherwise
    else if(argc ==1){
        mainFileStream = stdin;
    }
    else{
        printf("Too many arguments\n");
        exit(1);
    }
    return mainFileStream;
}



/*
  Demonstration main()
*/
int main(){
  
  char**an_array;//=malloc(sizeof(char*));
  char*delim=" ";
  char*demo_line="a line of text\n";
  char*line = strdup(demo_line); //line must be memory allocated
  
  printf("%s\n",line);
  
  int argn = parse(line,&an_array," \t\n");
  
  printf("argn:%d\n",argn);
  
  int i=0;
  while(an_array[i]!=NULL){
    printf("%s\n",an_array[i++]);

  }
  while(i!=0) 
    free(an_array[i--]);
  free(an_array);
  
  char**another_array;
  char*another_demo_line = "another demo & line > with < special >> tokens |";
  char*another_memallocated_line = strdup(another_demo_line);
  
  parse(another_memallocated_line,&another_array," \t\n");
  
  printf("&:%d,>:%d,<:%d,>>:%d,|:%d\n",find_special(another_array,"&"),find_special(another_array,">"),find_special(another_array,"<"),find_special(another_array,">>"),find_special(another_array,"|"));
  
}


