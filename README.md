<font size="10">Design Plan</font>  
- I've decided to split my program into a few key components. The shell will launch and dispatch  
from the **_myShell.c_** file. This file will determine whether we run in batch/interactive mode,  
start taking input from the user, parse the input, determine if we have redirection or pipes  
to handle, and check if invalid (pipe and redirect) input is given. Furthermore, we had to make  
sure that we could run either input redirection, output direction, or both at the same time. Using  
a stucture for flags, this functionality was implemented.

- Parsing, determining if there are special characters, and whether they are valid is handled in  
the functions from the **_helpers.c_** file.

- I have decided to implement redirection and pipes each in their own file,  
which might have been better off being broken up in the case of redirection, as it started to  
feel a bit bloated. 

- Redirection was handled in the **_redirect.c_** file. We are given the positions of the input  
and output redirection operators, as well as everything else entered at the command line. We began  
by creating a new array of strings pertaining to the command line input, without the redirection  
operators. In hindsight this could have been done by just adding a null character to the first  
occurrence of a redirection operator, but since it was already created, I decided to keep it  
just in case we needed the original string  for something later. All of this was done inside of  
a function we included in the redirect.c file called 'parseRedirect'. From there we opened the  
existing files (if necessary), forked/executed our process, and rerouted stdin/out to the correct  
file descriptor positions. Of course the execution was done with the programExec function, so we  
could run standard programs in our PATH.


<font size="10">Testing</font>  
Some of the paths inside of the PATH environment variable did not actually exist. After  
some research, we found that the values inside of the PATH variable are easily changed.  
Instead of treating this as an error, we can now just consider if the paths exist or not as  
expected behavior.
Redirect had a lot of test cases. In particular, we wanted to make sure  
that the program was being executed with the proper arguments (not including the redirection operators).  
This forced us to create a new argv array that would hold everything up until the last argument.  
We needed to make sure that all the arguments were taken though, so we used the location of  
the smaller index of the two redirection operators to find this last argument. We also wanted  
to ensure that each file was in fact a file we could redirect to, so we used stat to check for  
the proper file type. From there, we wanted to ensure that we were closing any opened file   
descriptors. This lead to some tricky positioning of the opening and closing of the file descriptors.  

While testing, I noticed that there were instances where exit wasn't success  
-fully terminating child processes. This was because we weren't exiting the process but  
instead, just exiting redirect in the child process. I had previously believed that wait  
would kill this process, but that was incorrect. Wait only reaps zombies and does not kill  
the process. In this case, our process never finished, allowing it to be reaped.   
A lot of testing was done using the 'io' program as well as 'pargs'. These executables were  
made from programs in the 'folder' directory. 'io' takes input from stdin and outputs to stdout.  
This was used to test redirection, which in the batch file uses commands that vary in redirecting  
output, input, or both to/from files.

The former will take input and provide output with the given files, and printArgs will simply  
print the arguments from the commandline to the given output file. 'Io' is also able to take    
input from a file and print to the screen.

<font size="4">Pseudocode was done throughout the c files in comments. They should have covered  
the full breadth of the project.</font>  