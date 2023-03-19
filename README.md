<font size="10">Testing</font>  
Some of the paths inside of the PATH environment variable did not actually exist. After  
some research, we found that the values inside of the PATH variable are easily changed.  
Instead of treating this as an error, we can now just consider if the paths exist or not as  
expected behavior.
Redirect had a lot of test cases. In particular, we wanted to make sure that the program  
was being executed with the proper arguments (not including the redirection operators).  
This forced us to create a new argv array that would hold everything up until those arguments.  
We needed to make sure that all of the arguments were take though, so we used the location of
the smaller index of the two redirection operators to find this last argument. We also wanted  
to ensure that each file was in fact a file we could redirect to, so we used stat to check for  
the proper file type. From there, we wanted to ensure that we were closing any opened file   
descriptors. This lead to some tricky positioning of the opening and closing of the file  
descriptors.