#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#define DB_NAME "users.dat"
#define NUM_USERS_TO_PUT 1000000
#define NUM_USERS_TO_PRINT 20
typedef struct user{
	unsigned int id;
	char username[20];
} user;

/** 
 * Initialize users  
 */
void initUsers(); 

/**
 * Takes an address of an user struct as argument
 * and inserts the user into the file
 */
void putUser(user * userToPut);

/**
 * Prints all users with non-zero id in the file
 */
void printUsersToConsole();

/**
 * Runs putUser for all users in users array
 */ 
void * putUsers(void * param);

/**
 * Prints users from file.
 * Uses NUM_USERS_TO_PRINT to define, how many uses should bu printed
 */
void * printUsers(void * param);

/**
 * Puts dummy data into the Random Access file
 */
void createFilePutDummyData();

/**
 * Executes putUsers and printUsers sequentially
 */
void executeSequentially();

/**
 * Executes putUsers and printUsers concurrently, 
 * but withuot mutex protection.
 */
void executeMultiThreading();

/**
 * Executes putUsers and printUSers concurrently
 * using with mutex protection.
 */
void executeMultiThreadingMutexLock();


pthread_mutex_t *lock = NULL;
user users[NUM_USERS_TO_PUT];
pthread_t write_thread;
pthread_t read_thread;
FILE* fp;
clock_t start, end_write, end_read;

int main(int argc, char * argv[])
{
	if(argc < 2){
		printf("Error. No argumentes passed. 1 2 or 3 sould be passed\n");
		exit(EXIT_FAILURE);
	}
	
	double time_total_write, time_total_read;
	
	initUsers();
	printf("Creating dummy data...\n");	
	createFilePutDummyData();
	printf("Dummy data is created.\n");
	start = clock();
	if((fp = fopen(DB_NAME, "rb+")) == NULL){
		printf("The file could not be open");		
	};

	switch(atoi(argv[1])){ // atoi converts string type to integer type
		case 1:
			printf("Sequentially mode starts.\n");
			executeSequentially();
			break;
		case 2: 
			printf("Multi threading mode withuot mutex protection starts.\n");
			executeMultiThreading();
			break;
		case 3:
			printf("Multi threading mode with mutex protection starts.\n");
			executeMultiThreadingMutexLock();
			break;
		default:
			printf("Error. Wrong argument entered.\n");
			exit(EXIT_FAILURE);
			break;
	}
	
	time_total_read = ((double) end_read - start);
	time_total_write = ((double) end_write - start);
	printf("Read took: %f seconds\n", time_total_read/ CLOCKS_PER_SEC);
	printf("Write took: %f seconds\n", time_total_write/ CLOCKS_PER_SEC);
	printf("So read finished %f second earlier than write\n",(time_total_write- time_total_read)/ CLOCKS_PER_SEC );
	fclose(fp);
}

void executeSequentially(){
        putUsers(NULL);
        end_write = clock();
        printUsers(NULL);
        end_read = clock();
}


void executeMultiThreading(){
	if( pthread_create(&(write_thread), NULL, putUsers, NULL)!= 0){
                printf("Thread can't be created\n");
        }
        if( pthread_create(&(read_thread), NULL, printUsers, NULL) != 0){
                printf("Thread can't be created\n");
        }

        pthread_join(read_thread, NULL);
        end_read = clock();
        pthread_join(write_thread, NULL);
        end_write = clock();

}

void executeMultiThreadingMutexLock(){
        lock = malloc(sizeof(pthread_mutex_t));
        if (pthread_mutex_init(lock, NULL) != 0){
                        printf("\n Mutex init has failed\n");
                        exit(EXIT_FAILURE);
        }
       
       
	if( pthread_create(&(write_thread), NULL, putUsers, NULL)!= 0){
                printf("Thread can't be created\n");
        }
        if( pthread_create(&(read_thread), NULL, printUsers, NULL) != 0){
                printf("Thread can't be created\n");
        }

        pthread_join(read_thread, NULL);
        end_read = clock();
        pthread_join(write_thread, NULL);
        end_write = clock();
	
	pthread_mutex_destroy(lock);
}

void initUsers(){
	// set id and usernames that starts from 1 to NUM_USERS+1
	for(int c = 1; c < NUM_USERS_TO_PUT+1; c++){
		users[c-1].id = c;
		sprintf(users[c-1].username, "username%d", c);
	}
}

void putUser(user * userToPut){
	
	if( fp  == NULL)
	{
		printf("File could not be opened.");
	} else{
	       	// lock	
	        if(lock != NULL){
	                pthread_mutex_lock(lock);
       		 }

		fseek(fp, ( userToPut->id - 1) * sizeof(user), SEEK_SET);
		fwrite(userToPut, sizeof(user), 1, fp);
		// unclock
		if(lock != NULL){
	                pthread_mutex_unlock(lock);
        	}
	}
}
void createFilePutDummyData(){
        int i = 0;
        if((fp = fopen(DB_NAME, "wb")) == NULL)
        {
                printf("File could not be opened.");
        } else
        {
                while(i < NUM_USERS_TO_PUT){
			putUser(&users[i]);
			i++;
		}
		fclose(fp);
        }

}

void * putUsers(void * param){  
	int i = 0;
	while(i<NUM_USERS_TO_PUT){
		putUser(&users[i]);
		i++;
	}
}
void * printUsers(void * param){
	user blankUser = {0, ""};
	
	if(fp  == NULL)
	{
		printf("File could not be opened.");
	} else{ 
		printf("%-6s%-16s\n", "Id", "User name");
		int i = 0;
		while(i<NUM_USERS_TO_PRINT){
			// lock 
			if(lock != NULL){
				pthread_mutex_lock(lock);
			}

			fseek(fp,i*sizeof(user), SEEK_SET);
			fread(&blankUser, sizeof(user), 1, fp);
			// unlock 
			if(lock != NULL){
				pthread_mutex_unlock(lock);
			}

			if(blankUser.id != 0){
				printf("%-6d%-16s\n", blankUser.id, blankUser.username);
			}
			i++;
		}
	}
}

