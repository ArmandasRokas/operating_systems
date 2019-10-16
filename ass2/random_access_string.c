#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define DB_NAME "users.dat"
#define NUM_USERS 10

typedef struct user{
	unsigned int id;
	char username[20];
} user;

void createFileWithBlankUsers(int numOfUsers);
void * putUser(void * userToPut);
void printUsersToConsole();


FILE * fPtr;  // users.dat file pointer
pthread_t tid[NUM_USERS];
//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t *lock;
int curr_id = 1;


int main(void)
{
	createFileWithBlankUsers(NUM_USERS+1);
	user users[NUM_USERS];
	// set usernames that starts from 1 to NUM_USERS+1
	for(int c = 1; c < NUM_USERS+1; c++){
		sprintf(users[c-1].username, "username%d", c);
	}
	void * users_p[NUM_USERS];
	
	for(int j = 0; j < NUM_USERS; j++){
		users_p[j] = &users[j];
	}	
	lock = malloc(sizeof(pthread_mutex_t));
	if (pthread_mutex_init(lock, NULL) != 0){
			printf("\n Mutex init has failed\n");
			return -1;
	}


//	if(( fPtr = fopen(DB_NAME, "rb+")) == NULL)
//	{
//		printf("File could not be opened.");
//	} else {
		int i = 0;
		int error;
		while(i<NUM_USERS){
			//putUser(users_p[i]);
			error = pthread_create(&(tid[i]), NULL, putUser, users_p[i]);
			if(error != 0){
				printf("Thread can't be created\n");
			}
			i++;
		}
//	}
//
//	fclose(fPtr);
	for(int d = 0; d < NUM_USERS; d++){
		pthread_join(tid[d],NULL);
	}
	pthread_mutex_destroy(lock);
	printUsersToConsole();
}


void createFileWithBlankUsers(int numOfUsers){
	unsigned int i;
	user blankUser = {0, ""};
	if((fPtr = fopen(DB_NAME, "wb")) == NULL)
	{
		printf("File could not be opened.");
	} else
	{
		for(i = 0; i < numOfUsers; i++)
		{
			fseek(fPtr, i*sizeof(user), SEEK_SET);
			fwrite(&blankUser, sizeof(user), 1, fPtr);	
		}
		fclose(fPtr);
	}
}

void * putUser(void *userToPut){
	pthread_mutex_lock(lock);
	FILE * fp;   // users.dat file pointer
	if(( fp = fopen(DB_NAME, "rb+")) == NULL)
	{
		printf("File could not be opened.");
	} else {
		printf("Lock\n");
		(*(user *) userToPut).id = curr_id;
		printf("%d %s", (*(user *) userToPut).id, (*(user *) userToPut ).username);
		fseek(fp, ( (*(user *)  userToPut).id - 1) * sizeof(user), SEEK_SET);
		size_t returnWrite = fwrite((user *)userToPut, sizeof(user), 1, fp);
		//printf("Returned size: %d", returnSeek);	
		curr_id++;
		printf("Unlock\n");
	}
	fclose(fp);
	pthread_mutex_unlock(lock);
}

void printUsersToConsole(){
	user blankUser = {0, ""};

	if(( fPtr = fopen(DB_NAME, "rb")) == NULL)
	{
		printf("File could not be opened. \n");
	}
	printf("%-6s%-16s\n", "Id", "User name");

	while(!feof(fPtr)){
		fread(&blankUser, sizeof(user), 1, fPtr);
		if(blankUser.id != 0){
			printf("%-6d%-16s\n", blankUser.id, blankUser.username);
		}
	}
	fclose(fPtr);
}

