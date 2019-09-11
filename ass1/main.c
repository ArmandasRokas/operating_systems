/*
 * Opgave 1:
 *Finde en måde til ' måske med for loop
 *som tjekker, hvis det en ' så lave 
 * mellemrum i stedet
 *
 * Opgave 2:
 * Crashes on empty input
 */

#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define INPUT_LEN 30
#define MAX_ARGS 10
// Executes system calls: fork and execv with
// given parameters. 
void execute(char ** args);
void executePipe(char ** args1, char ** args2);

int main(int argc, char *argv[]){
	while(1){
	       	int arg_num = 0;	
		char input[INPUT_LEN];
		char * args[MAX_ARGS];
		char * args1[MAX_ARGS] = {0};
		char * args2[MAX_ARGS] = {0};
		int isPipeFound = 0;
		int pipeIndex;
		
		printf("$");
		fgets(input, INPUT_LEN, stdin);
		
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
				printf("Pipe found \n");
				break;
			}
		}

		if(isPipeFound){
			printf("Executing pipe");
			printf("Pipe index %d \n" , pipeIndex);
			for(int z = 0; z < pipeIndex; z++){
				args1[z] = args[z];
				printf("isPipeFound %s", args1[z]);
			}

			int k=0;
			for(int d = pipeIndex+1; d < arg_num; d++){
				args2[k] = args[d];
				printf("isPipeFound %s ", args2[k]);
				k++;	
			}
			
			
			
			//int status;
			//pid_t p = fork();
			//if(p == 0){
				//child
				//char * testArgs[] = {"./executePipe", args};
				//execvp(testArgs[0], testArgs);  
				executePipe(args1, args2);
			//} else {
			//	waitpid(p, &status, 0);
			//}
		} else {
			execute(args);
		}



	}
	
	return 0;
}

void execute(char ** args) {
		int status;
		pid_t p = fork();
		
		if(p == 0){
			//printf("child");
			//printf("%s", args[0]);
			execvp(args[0], args);
			printf("Error");
			printf("\n");
		} else {	
			//printf("parent");
			waitpid(p,&status,0);
			//printf("Done waiting\n");
		}

}

void executePipe(char ** args1, char ** args2){
	int status;
	int fds[2];
	pipe(fds);
	int i;
	pid_t p = fork();
	
	if(p==0){ 
		// child process
		close(fds[1]);
		dup2(fds[0],0);
		execvp(args2[0], args2);
	} else{
		// parent process
		close(fds[0]);
		dup2(fds[1],1);
		execvp(args1[0], args1);
	}
		
}

