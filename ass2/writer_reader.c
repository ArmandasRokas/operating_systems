#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#define DB_NAME "users.dat"
#define NUM_USERS 1000000
#define NUM_USERS_TO_PRINT 30
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
void putUser(user * userToPut);
/**
 * Prints all users with non-zero id in the file
 */
void printUsersToConsole();
/**
 * Creates a new thread for every user that needs to be put into the file.
 */
void executePutUserParallel();
/**
 * Users is inserted into the file sequentially
 * For testing purposes.
 */ 
void *  putRemainingUsers(void * param);
void putTenUsers();
void * printUsers(void * param);
void createFilePutDummyData();
pthread_t tid[NUM_USERS];
pthread_mutex_t *lock = NULL;
//int curr_id = 1;
user users[NUM_USERS];
//void * users_p[NUM_USERS];
pthread_t write_thread;
pthread_t read_thread;
FILE* fp;

int main(void)
{
	clock_t end_write, end_read;
	clock_t start = clock();
	double time_total_write, time_total_read;
	//setbuf(stdout, NULL);
	lock = malloc(sizeof(pthread_mutex_t));
	if (pthread_mutex_init(lock, NULL) != 0){
			printf("\n Mutex init has failed\n");
			exit(EXIT_FAILURE);
	}
	//createFileWithBlankUsers(NUM_USERS+1);
	initUsers();
	
	createFilePutDummyData();
	
	if((fp = fopen(DB_NAME, "rb+")) == NULL){
		printf("The file could not be open");		
	};
	//putTenUsers(); // put some dummy data into the Random Access file in order to work printTenUsers();


	// executeMultiThreading()
	// executeMultiThreadingWtihMutexLock() // initialize mutex 
	// executeSequentially()

	//executePutUserParallel();
	
	//executePutUserSequentially(); // Uncomment this in order to put user sequentially
	
	if( pthread_create(&(write_thread), NULL, putRemainingUsers, NULL)!= 0){
		printf("Thread can't be created\n");
	}
	if( pthread_create(&(read_thread), NULL, printUsers, NULL) != 0){
		printf("Thread can't be created\n");	
	}
	
	pthread_join(read_thread, NULL);
	end_read = clock();
	pthread_join(write_thread, NULL);
	end_write = clock();
	time_total_read = ((double) end_read - start);
	time_total_write = ((double) end_write - start);
	printf("Read took: %f seconds\n", time_total_read/ CLOCKS_PER_SEC);
	printf("Write took: %f seconds\n", time_total_write/ CLOCKS_PER_SEC);
	printf("So read finished %f second earlier than write\n",(time_total_write- time_total_read)/ CLOCKS_PER_SEC );
	fclose(fp);
	pthread_mutex_destroy(lock);
//	printUsersToConsole();
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
		users[c-1].id = c;
		sprintf(users[c-1].username, "username%d", c);
	}
	// initializes users pointer array 
	//for(int j = 0; j < NUM_USERS; j++){
	//	users_p[j] = &users[j];
	//}	
}

void putUser(user * userToPut){
	if(lock != NULL){
		//pthread_mutex_lock(lock);
	}
//	FILE * fp;   // users.dat file pointer
	
	if( fp  == NULL)
	{
		printf("File could not be opened.");
	} else{ 
		//userToPut->id = curr_id;
		fseek(fp, ( userToPut->id - 1) * sizeof(user), SEEK_SET);
		fwrite(userToPut, sizeof(user), 1, fp);
		//curr_id++;
	}
//	fclose(fp);
	if(lock != NULL){
		//pthread_mutex_unlock(lock);
	}
}

void putTenUsers(){
	int i = 0;
	while(i<10){
		putUser(&users[i]);
		i++;
	}	
}
/*
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
}*/
void createFilePutDummyData(){
	//FILE * fPtr;  // file pointer   
        int i = 0;
        if((fp = fopen(DB_NAME, "wb")) == NULL)
        {
                printf("File could not be opened.");
        } else
        {
                while(i < NUM_USERS){
			putUser(&users[i]);
			i++;
		}
		fclose(fp);
        }

}

void * putRemainingUsers(void * param){ // make a thread. to execute all users. Mutex lock only atomic in the file. 
	int i = 10;
	while(i<NUM_USERS){
	//	printf(".");
	//	fflush(stdout);
		putUser(&users[i]);
		i++;
	}
}
void * printUsers(void * param){
//	pthread_mutex_lock(lock);
//	FILE * fp;   // users.dat file pointer
	
//	if(( fp = fopen(DB_NAME, "rb+")) == NULL)
//	{
//		printf("File could not be opened.");
//	} else{ 

	fseek(fp,0, SEEK_SET);
	user blankUser = {0, ""};
	
	printf("%-6s%-16s\n", "Id", "User name");
	//fflush(stdout);
	int i = 0;
	while(i<NUM_USERS_TO_PRINT){
		fseek(fp,i*sizeof(user), SEEK_SET);
		fread(&blankUser, sizeof(user), 1, fp);
		if(blankUser.id != 0){
			printf("%-6d%-16s\n", blankUser.id, blankUser.username);
	//		fflush(stdout);
		}
		i++;
	}
//		fclose(fp);
//	pthread_mutex_unlock(lock);
//	}
}

void printUsersToConsole(){
	FILE * fp;
	fseek(fp, 0, SEEK_SET);
	user blankUser = {0, ""};/*
	if(( fp = fopen(DB_NAME, "rb")) == NULL)
	{
		printf("File could not be opened. \n");
	}*/
	printf("%-6s%-16s\n", "Id", "User name");

	while(!feof(fp)){
		fread(&blankUser, sizeof(user), 1, fp);
		if(blankUser.id != 0){
			printf("%-6d%-16s\n", blankUser.id, blankUser.username);
		}
	}
	//fflush(stdin);
	//fclose(fp);
}

