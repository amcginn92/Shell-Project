<font size="10">Testing</font>  
Some of the paths inside of the PATH environment variable did not actually exist. After  
some research, we found that the values inside of the PATH variable are easily changed.  
Instead of treating this as an error, we can now just consider if the paths exist or not as  
expected behavior.