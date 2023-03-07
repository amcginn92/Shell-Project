#define LINE_MAX 4096 

int parse(char*line, char***_args, char*delim);
int find_special (char*args[], char * special);
FILE *getInput(int argc, char* argv[]);