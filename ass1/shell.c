#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_LEN 30
#define MAX_ARGS 10

void executeCommand(char ** args);
void executeCommandWithPipe(char ** args1, char ** args2);

int main(int argc, char *argv[]){
	while(1){
	       	int arg_num = 0;	
		char input[INPUT_LEN] = "\n";
		char * args[MAX_ARGS] = {0};
		char * args1[MAX_ARGS] = {0};
		char * args2[MAX_ARGS] = {0};
		int isPipeFound = 0;
		int pipeIndex;
		
		printf("$");
		fgets(input, INPUT_LEN, stdin); 
	        // Check if input is not empty	
		if(!strcmp(input,"\n")){
			printf("Enter a command\n");
			continue;
		}

		// Split input to tokens(array of arugments)
		char * cmd = strtok(input, " \n");		
		while(cmd != NULL){
			args[arg_num] = cmd;
			cmd = strtok(NULL, " \n");
			arg_num++;
		}
		// The last argument should be NULL
		args[arg_num] = NULL;
		if(!strcmp(args[0],"exit")){
			break;
		}	
		
		// Search for pipe
		for(pipeIndex = 0; pipeIndex < arg_num; pipeIndex++){
			if(strcmp(args[pipeIndex], "|") == 0){
				isPipeFound = 1;
				break;
			}
		}

		if(isPipeFound){
			for(int z = 0; z < pipeIndex; z++){
				args1[z] = args[z];
			}
			int k=0;
			for(int d = pipeIndex+1; d < arg_num; d++){
				args2[k] = args[d];
				k++;	
			}
			executeCommandWithPipe(args1, args2);
		} else {
			executeCommand(args);
		}
	}
	return 0;
}

void executeCommand(char ** args) {
		int status;
		pid_t p = fork(); 
		if(p == 0){
			// child process
			execvp(args[0], args);
			printf("The entered command does not exists\n");
		} else {	
			// parent process
			// p contains PID of the child
			waitpid(p,&status,0);
		}

}
/*
This function is made by using this code as an example:
https://stackoverflow.com/questions/33912024/shell-program-with-pipes-in-c
*/
void executeCommandWithPipe(char ** args1, char ** args2){
	int status;
	pid_t p = fork();
	if(p==0){
		int fds[2]; // fds[0] is for reading and, fds[1] for writing
		pipe(fds);
		pid_t p2 = fork();
		if(p2!=0){ 
			// parent process
			// executes the second command
			// Should terminates last in order to waitpid() one level above to work
			close(fds[1]); // close pipe for writing, because it should only read
			// grep læser fra stdin, som bliver streamet fra fds[0]
			dup2(fds[0],0); //stdin reads from pipe. The process is suspended until something is written. 
			execvp(args2[0], args2);
			printf("The entered command does not exists\n");
			exit(-1);
		} else{
			// child process
			// executes the first command
			close(fds[0]); // close pipe for reading, because it should only write
			dup2(fds[1],1); //stdout writes to pipe
			execvp(args1[0], args1);
			printf("The entered command does not exists\n");
			exit(-1);
		}
	} else {
		waitpid(p, &status, 0); // waits for a child who is a parent of an another child
	}  
}

