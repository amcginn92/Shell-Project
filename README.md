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

- Piping was handled in **myPipes.c** file. We count the number of pipes, create a pipe array  
of this length, and then fork all of our children. From here, the parent close all of its fds,  
and we implemented logic to have each child redirect their output as necessary. Executing the  
process with our original programExec function was creating an additional process for each child,  
so we had to create another program execution function that essentially did the same things  
without forking. This lead to further divided logic pertaining to background processes. This  
logic needed to be applied to both redirection and piping functions and was really the most  
flawed part of the program, albeit functional.

- Background processes, as said previously, have been applied to separate areas of my code,  
as we got further in the project, this seemed to be the only feasible approach. In hindsight  
creating a more reusable way of doing this would be beneficial for future changes to the project.  


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

Later Stages of testing required a lot more particular test cases. Redirection and piping had  
had to cause an error if applied in one command. Multiple redirections had to be stopped for  
both input and output. We had to be sure that background processes were waiting for both pipes  
and redirected commands. Further, making sure that pipes worked for an indefinite number (up to 512)  
pipes was a concern and was tested for up to 10 processes with success. Considering the logic was  
based on the process number in the list, this should work indefinitely. Making sure zombie processes  
were being reaped routinely throughout the project (1x per command execution) and the wait function was  
successfully reaping all children was also difficult. Executing commands with the sleep # & call and  
then running ps -u to see the running programs and those that were reaped seemed to be an efficient way  
to do this. 

<font size="4">Pseudocode was done throughout the c files in comments. They should have covered  
the full breadth of the project.</font>  