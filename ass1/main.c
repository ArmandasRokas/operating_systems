#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Executes system calls: fork and execv with
// given parameters. 
void execute(char ** args);

int main(int argc, char *argv[]){
	while(1){
	       	int arg_num = 0;	
		char input[30];
		char * args[10];
		
		printf("$");
		fgets(input, 29, stdin);
		
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
		execute(args);
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

