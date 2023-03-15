myShell: myShell.o helpers.o help.o programExec.o redirect.o myPipe.o
	gcc -o myShell myShell.o helpers.o help.o programExec.o redirect.o myPipe.o -Wall -Werror

myShell.o: myShell.c
	gcc -c myShell.c -Wall -Werror

helpers.o: helpers.c
	gcc -c helpers.c -Wall -Werror

help.o: help.c
	gcc -c help.c -Wall -Werror

programExec.o: programExec.c
	gcc -c programExec.c -Wall -Werror

redirect.o: redirect.c
	gcc -c redirect.c -Wall -Werror

myPipe.o: myPipe.c
	gcc -c myPipe.c -Wall -Werror

clean:
	rm -rf *.o