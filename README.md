<font size="10">Testing</font>  
Some of the paths inside of the PATH environment variable did not actually exist. After  
some research, we found that the values inside of the PATH variable are easily changed.  
Instead of treating this as an error, we can now just consider if the paths exist or not as  
expected behavior.
Redirect had a lot of test cases. In particular, we wanted to make sure that the program  
was being executed with the proper arguments (not including the redirection operators).  
This forced us to create a new argv array that would hold everything up until the last argument.  
We needed to make sure that all the arguments were taken though, so we used the location of
the smaller index of the two redirection operators to find this last argument. We also wanted  
to ensure that each file was in fact a file we could redirect to, so we used stat to check for  
the proper file type. From there, we wanted to ensure that we were closing any opened file   
descriptors. This lead to some tricky positioning of the opening and closing of the file  
descriptors. While testing, I noticed that there were instances where exit wasn't success  
-fully terminating the program. This was because we weren't exiting in the child program.  
I had previously believed that wait would kill this process, but that was incorrect. Wait  
will reap zombies, not kill the process. Furthermore, we had to make sure that we could run  
either input redirection, output direction, or both at the same time. Using the flags, this  
functionality was implemented. Testing was done using the 'io' program as well as 'printArgs.'  
The former will take input and provide output with the given files, and printArgs will simply  
print the arguments from the commandline to the given output file. 'Io' is also able to take    
input from a file and print to the screen.