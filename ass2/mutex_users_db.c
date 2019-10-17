#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define DB_NAME "users.dat"
#define NUM_USERS 1000000

typedef struct user{
	unsigned int id;
	char username[20];
} user;

/**
 * Creates Random Access File with blank users in order to
 * to insert randomly into the file
 */
void createFileWithBlankUsers(int numOfUsers);
/** 
 * Initialize users and inserts to users_p array
 */
void initUsers(); 
/**
 * Takes an user struct as argument
 * and inserts the user into the file
 */
void * putUser(void * userToPut);
/**
 * Prints all users with non-zero id in the file
 */
void printUsersToConsole();
/**
 * Creates a new thread for every user that needs to be put into the file.
 */
void executePutUserParallel();
/**
 * Users is inserted into the file sequentially from the main thread
 * For testing purposes.
 */ 
void executePutUserSequentially();

pthread_t tid[NUM_USERS];
pthread_mutex_t *lock = NULL;
int curr_id = 1;
user users[NUM_USERS];
void * users_p[NUM_USERS];

int main(void)
{
	createFileWithBlankUsers(NUM_USERS+1);
	initUsers();

	//executePutUserParallel();
	executePutUserSequentially(); // Uncomment this in order to put user sequentially

	//printUsersToConsole();
}


void createFileWithBlankUsers(int numOfUsers){
	FILE * fPtr;  // file pointer	
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

void initUsers(){
	// set usernames that starts from 1 to NUM_USERS+1
	for(int c = 1; c < NUM_USERS+1; c++){
		sprintf(users[c-1].username, "username%d", c);
	}
	// initializes users pointer array 
	for(int j = 0; j < NUM_USERS; j++){
		users_p[j] = &users[j];
	}	
}

void * putUser(void *userToPut){
	if(lock != NULL){
		pthread_mutex_lock(lock);
	}
	FILE * fp;   // users.dat file pointer
	if(( fp = fopen(DB_NAME, "rb+")) == NULL)
	{
		printf("File could not be opened.");
	} else {
		(*(user *) userToPut).id = curr_id;
		fseek(fp, ( (*(user *)  userToPut).id - 1) * sizeof(user), SEEK_SET);
		fwrite((user *)userToPut, sizeof(user), 1, fp);
		curr_id++;
	}
	fclose(fp);
	if(lock != NULL){
		pthread_mutex_unlock(lock);
	}
}

void executePutUserParallel(){
	lock = malloc(sizeof(pthread_mutex_t));
	if (pthread_mutex_init(lock, NULL) != 0){
			printf("\n Mutex init has failed\n");
			exit(EXIT_FAILURE);
	}

	int i = 0;
	int error;
	while(i<NUM_USERS){
		error = pthread_create(&(tid[i]), NULL, putUser, users_p[i]);
		if(error != 0){
			printf("Thread can't be created\n");
		}
		i++;
	}
	for(int d = 0; d < NUM_USERS; d++){
		pthread_join(tid[d],NULL);
	}
	pthread_mutex_destroy(lock);
}

void executePutUserSequentially(){ // make a thread. to execute all users. Mutex lock only atomic in the file. 
	int i = 0;
	while(i<NUM_USERS){
		putUser(users_p[i]);
		i++;
	}
}

void printUsersToConsole(){
	FILE * fPtr;
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

