#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]){
	while(1){
		
		//char* command = (char*) malloc(20 * sizeof(char)); 
		int status; 
		char input[30];
		char * argv[10];
		//argv[0] = NULL;
		argv[1] = NULL;
		argv[2] = NULL;
		printf("$");
		//scanf("%s", cmd);
		fgets(input, 29, stdin);
		char * cmd = strtok(input, " \n");	
		argv[0] = cmd;
		argv[1] = strtok(NULL, " \n");

		pid_t p = fork();
		//printf("%d", p);
		
		if(p != 0){
			// Parent code
			//printf("parent");
			waitpid(-1,&status,0);
		} else {
			printf("child");
			printf("%s", cmd);
			execvp(cmd, argv);
			printf("\n");
		}
	}
	
	return 0;
}
/*
 * waitpid
 *
 * command line argument into array
 *
 */
