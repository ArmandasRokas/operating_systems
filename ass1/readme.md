## The manual of the shell
Author: Armandas Rokas (s185144)

### Intro

This manual describes the features of the shell application and how the shell is implemented. 

### Quick start

1. `gcc -o shell shell.c`
2. `./shell`
3. Write your favorite command e.g. `ls`

### Features

- Up to 9 separate arguments beside the actual command. 
- The pipe, where you can use one programs output as another program inputs. For example `ls -l | grep somestring`.  



### Code documentation

The shell is implemented like so. Firstly, it reads a command from stdin and parse it into arguments. Then the shell tries to find a program file in the shells  environment. 

#### The shells environment

The shell environment is defined in `$PATH` variable, which is just a list of directories.  

If the command contains slash so 



#### System calls

To execute a command there are used three-four different system calls depends on whether command includes a pipe or not.  Explain system calls generally

##### fork()

##### execvp()


##### wait()

##### pipe()




### Notes

 and execute runs executable with given input. If the executable is not found , the minishell prints the error massage. The default path, where the minishell looks for executable is `/bin` and `/usr/bin`. 

It executes binary applications in /bin directory by giving there names as input in the shell. For example `ls` or `top`





Program environment er $Path, som bruger execvp??, but it is possible to give an absolute path too. Like try ./home/helloworld



- https://www.wikihow.com/Check-Path-in-Unix - Step 2
- Answer 2: https://stackoverflow.com/questions/19205316/execvp-filepath-clarification
- https://linux.die.net/man/3/execvp

### fork()

The minishell uses `fork()` fdunction to create a new process and executes an applictation in the process by using `execvp()` function

 tries to find application in the system related to the input and executes that. If the application not found error message is printed. 

looks for executables in `/bin` `/user/bin` and `/usr/share` as input

###   pipe

https://stackoverflow.com/questions/9834086/what-is-a-simple-explanation-for-how-pipes-work-in-bash




### Some existing bugs: 

- Program crashes when entered a empty command
- using `grep` with a string in quotation marks  

### Concepts of systems calls

- e.ks. fork() . Hvad gør det og hvorfor, jeg har bugt dem



To execute a program, Bash calls `fork` (or maybe `vfork` or some other variant), which creates a new “child” process that is nearly a copy of the original “parent” one. This child process then makes the `execve` system call, which replaces its own program with the program requested. Then Bash calls some version of the system call `wait` in order to wait until the child process terminates.

### I/O redirection

- 

### The program environment

- 

### Background program execution

- 

Traps

