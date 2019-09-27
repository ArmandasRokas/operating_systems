## The manual of the shell
Author: Armandas Rokas (s185144)
21/9 2019

### Intro

This manual describes the features and the implementation of the shell application. The shell works as a command interpreter by reading an user input as a command and executing that.

### Quick start

1. `gcc -o shell shell.c`
2. `./shell`
3. Write your favorite command e.g. `ls -l | greb shell` 

### Features

- Up to 9 separate arguments.
- Pipe, which allows to use first commands output as another commands inputs. 

### Code documentation

The shell is implemented as follows. Firstly, it reads a command from stdin and parses it into separate arguments. Then the shell tries to find a program file in the shells  environment. 

#### The shells environment
The shells environment depends on whether a command contains `/` or not. If a command contains `/`, so the environment can be absolute or a relative. Absolute if it starts with `/` or relative if does not start with `/`. 

If a command does not contain `/` at all, so the shell looks for an executable application in one of directories listed in the `$PATH` environment variable, because there is used `execvp()` system call, which is explained more in details below.

###### References: 

8, 14, 15 in Bibliography 

#### System calls
In order to execute a command there are used three to five different system calls depends on whether the entered command includes a pipe or not. System calls is basically used for interaction with a kernel in order to get some resources (RAM, hardware)/services, which is not available in User mode. 

###### References: 

17 in Bibliography 

##### fork()

The first system call is `fork()`. It is used in order to create an exact copy of a process named a child process. However at first the child process points to the same memory allocations as the parent process do and just after mutation it redirects to another location. It is done like this because of optimization. 
A child process has the same program counter(pc), CPU registers and open files as the parent process.
`fork()` can return either negative value, zero or positive value. Negative is of course when creation was unsuccessful. Zero if it is returned to child process. And positive value if it is returned to parent and the value contains PID of the child. 

###### References: 

1, 5, 8, 9, 10,11  in Bibliography 

##### execvp()

The second system call that is used in the shell application is `execvp()`. `execvp()` launches a new program in the child process replacing address space, text segment(program code), data segment(static variables). Only process ID, environment and file descriptor remains the same.  

###### References: 

16  in Bibliography 


##### wait()

`wait()` system call is used in the parent process in order to wait (run in the background) until the child process terminates. That means that the shell waits until command executed before an user can enter a new command. 

###### References: 

12  in Bibliography 

##### pipe()

The `pipe()` is used in order to communicate between two the parent and processes. It creates a "virtual file", so that one process writes to this and another reads. 

###### References: 

3, 4, 6, 13 in Bibliography 


##### dup2() for I/O redirection

`dup2()` is used together with`pipe()`. It makes a file descriptor of `stdout` point to the end of the pipe that writes to "virtual file" in the first process (executes the first command). And a file descriptor of `stdin` point to the end of the pipe that reads from the pipe in the other process (executes the second command).

###### References: 

7  in Bibliography 

### Some existing bugs: 

- Using `grep` with a string in quotation marks does not return an expected result . E.g. `ls -l | grep 'shell'` wont work, but `ls -l | grep shell` works.

