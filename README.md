# Project-2-S23
## Project 2: Shell
In this homework, you’ll be building a shell, similar to the tsch shell you use on your CIS Linux2 system. When you open a terminal window on your computer, you are running a shell program, which is tsch on your system. The purpose of a shell is to provide an interface for users to access an operating system’s services, which include file and process management. sh (Bourne shell) is the original Unix shell, and there are many different flavors of shells available. Some other examples include ksh (Korn shell), bash (GNU Bourne again shell), and zsh (Z shell). Shells can be interactive or non-interactive. 

For instance, you are using tsch non-interactively when you run a tsch script. tsch is interactive when invoked without arguments, or when the -i flag is explicitly provided. The operating system kernel provides well-documented interfaces for building shells. By building your own, you’ll become more familiar with these interfaces, and you’ll probably learn more about other shells as well. 

## Directory Commands 
You should begin by creating skeleton code for your shell with a launcher/dispatcher for “built-in” commands. Every shell needs to support a number of built-in commands, which are functions in the shell itself, not external programs. For example, the exit command needs to be implemented as a built-in command, because it exits the shell itself. You should start with two built-ins: help, and exit, which exits the shell. 

Add a built-in pwd that prints the current working directory to standard output. Then, add a new built-in cd that takes one argument, a directory path, and changes the current working directory to that directory. You may find the syscalls chdir and getcwd helpful. Check out man pages for usage details. 

## Program Execution 
If you try to type something into your shell that isn’t a built-in command, at this point you should display a message that the shell doesn’t know how to execute programs. Modify your shell so that it can execute programs when they are entered into the shell. The first word of the command is the name of the program. The rest of the words are the command-line arguments to the program. 

You should assume that the first word of the command will be **the full path to the program**. So instead of running wc, you would have to run /usr/bin/wc. In the following parts of the project, you will implement support for simple program names like wc.  

You are to **use the source code we have supplied** with this assignment for separating the input text to the shell into words. When your shell needs to execute a program, it should fork a child process, which calls one of the functions from the exec family to run the new program. **You may not use** execvp (see Path Resolution). The parent process should wait until the child process completes and then continue listening for more commands. 

Once you implement this functionality, you should be able to execute programs. 
>./shell 
>
>0: /usr/bin/wc shell.c
>
>    77      262     1843 shell.c
>
>1: exit 

## Path Resolution 

You probably found that it was a pain to test your shell in the previous part because you had to type the full path of every program. Luckily, every program (including your shell program) has access to a set of **environment variables**, which is structured as a hashtable of string keys to string values. One of these environment variables is the PATH variable. You can print the PATH variable of your development environment. 

echo $PATH 

/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:... 

When tsch or any other shell executes a program like wc, it looks for a program called wc in each directory listed in the PATH environment variable and runs the first one that it finds. The directories in PATH are separated with a colon. You may find the function strtok_r to be useful for parsing the PATH string. 

Modify your shell so that it uses the PATH variable from the environment to resolve program names. Typing in the full pathname of the executable should still be supported. **Do not use** execvp. Use execv instead and implement your own PATH Resolution. 

## Redirection 
When running programs, it is useful to provide input from a file or to direct output to a file. The syntax [process] > [file] tells your shell to redirect the process’s standard output to a file. Similarly, the syntax [process] < [file] tells your shell to feed the contents of a file to the process’s standard input. 

Modify your shell so that it supports redirecting stdout and stdin to/from files. You do not need to support redirection for shell built-in commands. You do not need to support stderr redirection or appending to files (e.g. [process] >> [file]). You can assume that there will always be spaces around special characters < and >.  You should be able to process a command line redirecting both stdin and stdout. 

## Pipes 
### Task 
Other times, it is useful to provide the output of a program as the input of another program. The syntax [process A] |[process B] tells your shell to pipe the output of process A to the input of process B. In other words, the output of program A becomes the input of program B. 

Modify your shell so that it supports pipes between programs. You can assume there will always be spaces around the special character |. 
You must support an indefinite number of pipes in series, as seen in the example below. 

[process A] | [process B] | [process C] 

To implement this part of the shell, you will need to use the pipe syscall. An explanation for how this syscall works is provided below. Historically, this portion of the homework has been the most difficult for students to implement due to students not fully understanding what’s actually going on when they create pipes. Thus, we highly recommend that you first fully understand how pipes work on a conceptual level before you begin coding. Course staff reserves the right to not help you debug your code if you prove that you started coding without first attempting to understand the fundamental concepts outlined below. 

## Background 

A pipe is a mechanism for inter-process communication, where one process writes to the “write end” of the pipe, and another process reads from the “read end”. Understanding the implementation details of pipes is not required for this homework. 

### How the pipe syscall works 

To create a pipe in code, we use the pipe syscall: int pipe(int fds[2]). 

As a bit of review, when a process opens a regular file (using the open syscall), a file description is created and it is assigned a file descriptor (remember, this is just a non-negative integer). This mapping between file descriptor and description is stored in the process’s FDT (file descriptor table). 

On the other hand, when a process opens a pipe (using the pipe syscall), two file descriptions are created: one for the read end and one for the write end. After calling pipe, fds[0] gets set to the file descriptor for the read end of the pipe, and fds[1] gets set to that of the write end. 

Just as the read and write syscalls operate on file descriptors in the “regular file” case, read and write are used to interact with the file descriptors corresponding to the read and write ends of the pipe respectively. 

When a process tries to read from the read end of the pipe, it will block until data shows up at the write end (i.e. another process writes to the write end). When all file descriptors pointing to the write end of the pipe have been closed, calling read on the read end of the pipe will return EOF (i.e. 0). 

Pipes have limited capacity, meaning they can only store so much data. When a process tries to write to the write end of the pipe and the pipe is full (i.e. another process hasn’t read from the read end to “empty out the pipe”), the writing process will block. When all file descriptors pointing to the read end of the pipe have been closed, writing to the pipe will raise the SIGPIPE signal. 

### Walkthrough of a simple pipe example 
Let’s say that we want a parent process P1 to write data to a pipe and have its child process P2 read the data. 

P1 first creates a pipe and then creates P2 using fork(). This [diagram](/Picture1.jpg) depicts the Filed Descriptor Tables (FDTs) of P1 and P2, assuming that the pipe syscall set FDs 3 and 4 to point to the read and write end of the pipe respectively. Remember that because P2 was created using fork(), P1 and P2 have identical FDTs that point to the same file descriptions. 

P1 calls close(3) and P2 calls close(4). Now, P1 only has an open file descriptor for the write end of the pipe, and P2 only has an open descriptor for the read end. P1 can happily write data to the write end and P2 will continue to read this data until either
1.	P1 chooses to stop sending data, in which case P1 would simply call close(4).  
2.	P2 chooses to stop reading from the pipe, in which case P2 would simply call close(3).  

In case 1, after P1 closes its write-end FD, P2 will be notified that there’s nothing left to read (the read syscall will return 0 bytes read), so it won’t block indefinitely. In case 2, after P2 closes its read-end FD, P1 will be notified that there’s no one left to read the data (via the SIGPIPE signal, which P1 can choose to handle gracefully so as to not exit), so it also won’t block. [changes shown in this diagram](/Picture2.jpg)
 
The steps above where the writer process closes its read-end FD and the reader process closes its write-end FD are extremely important. Let’s consider the importance of each step separately. 

Recall that a process that reads from a pipe will continue to expect data to show up at the write end as long as there are open file descriptors pointing to the write end. Thus, if P2 doesn’t call close(4), even when P1 is done sending its data through the pipe and closes its own write-end FD, P2 will continue to hang, waiting for data that will never come. 

Similarly, consider the case where P1 doesn’t call close(3). Let’s assume that in this scenario, P1’s job is to keep writing data to the pipe until there are no longer any processes reading from the pipe. Consider what happens when P2 finishes execution at some point in time. When P2 exits, all of its FDs get closed. However, there remains an open FD for the read end of the pipe in P1’s FDT, so P1 thinks that there’s still a process that’s reading data from the pipe. P1 continues writing data to the pipe and eventually, the pipe fills up completely, since there’s no longer a reader to “empty it out”. When P1 attempts to write to the full pipe, it will get blocked indefinitely. 

### Recommended approach for implementing piping 

Let’s say that the user inputs a command that involves creating n processes, where process i treats the output of process i - 1 as its stdin and process i sends its stdout into process i + 1’s stdin. Having n processes means we need n - 1 pipes. 

1.	Create an array of n - 1 pipes in the shell process. Let’s call this array pipe_arr.
2.	Use a for loop to fork() the n child processes.
3.	For every index i (aside from the first and last, which are edge cases), processi changes its stdin FD (0) to point to the read end of pipe_arr[i - 1] and changes its stdout FD (1) to point to the write end of pipe_arr[i]. The dup2 syscall will prove to be useful here. The idea here is that for every index i (other than the edge cases), process i is responsible for writing to the ith pipe in pipe_arr. Because every process does this, process i can read from the read end of pipe_arr[i - 1] to get data that’s being written by process i - 1. 
4.	After changing its stdin and stdout, process i **must close all of its pipe FDs**. If these FDs are not closed, **the process will hang** for the reasons provided in the previous section. After closing all of these pipe FDs, the process will still have access to a single read-end FD and a single write-end FD for the appropriate pipes due to using dup2. 
5.	The shell process also needs to close all of the pipe FDs since it doesn’t make use of them at all (remember, the pipes are used exclusively by the child processes). 

Hopefully, these guidelines allow you to implement piping in a relatively hassle-free manner. Remember, if you ever run into issues with your code hanging, there’s a very high chance that you didn’t close all of the necessary pipe FDs. 

## Background processes 

So far, your shell waits for each program to finish before starting the next one. Many shells allow you run a command in the background by putting an & at the end of the command line. After the background program is started, the shell allows you to start more processes without waiting for the background process to finish. 

Modify your shell so that it runs commands that end in an & in the background. Once you’ve implemented this feature, you should be able to run programs in the background with a command such as /bin/ls &. 

You should also add a new built-in command wait, which waits until all background jobs have terminated before returning to the prompt.
You can assume that there will always be spaces around the & character. You can assume that, if there is a & character, it will be the last token on that line. 
