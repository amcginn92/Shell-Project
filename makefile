myShell: myShell.o helpers.o help.o
	gcc -o myShell myShell.o helpers.o help.o -Wall -Werror

myShell.o: myShell.c
	gcc -c myShell.c -Wall -Werror

helpers.o: helpers.c
	gcc -c helpers.c -Wall -Werror

help.o: help.c
	gcc -c help.c -Wall -Werror

clean:
	rm -rf *.o