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

int curr_id = 1;

int main(void)
{
	createFileWithBlankUsers(NUM_USERS+1);
	/*
	user user1 = {1, "username1"};
	user user2 = {2, "username2"};
	user user3 = {3, "username3"};
	user user4 = {4, "username4"};
	user user5 = {5, "username5"};
	user user6 = {6, "username6"};
	user user7 = {7, "username7"};
	user user8 = {8, "username8"};
	user user9 = {9, "username9"};
	user user10 = {10, "username10"};
	user user11 = {11, "username11"};
	*/


	user users[NUM_USERS];
	// set usernames that starts from 1 to NUM_USERS+1
	for(int c = 1; c < NUM_USERS+1; c++){
		//users[c].id = c;
		//users[c].username = c+1 ;
		sprintf(users[c-1].username, "username%d", c);
	}

	void * users_p[NUM_USERS];
	for(int j = 0; j < NUM_USERS; j++){
		users_p[j] = &users[j];
		
		//user u = {j, "username"};
		//users_p[j] = &u
		//;	
		//users_p[j].id = j ;
		//*((user *)  users_p[j]).username = "username" ;
	}
	/*
	users_p[0] = &user1;
	users_p[1] = &user2;
	users_p[2] = &user3;
	users_p[3] = &user4;
	users_p[4] = &user5;
	users_p[5] = &user6;
	users_p[6] = &user7;
	users_p[7] = &user8;
	users_p[8] = &user9;
	users_p[9] = &user10;
	users_p[10] = &user11;
 	*/

	//iputUser(user1_p);
	if(( fPtr = fopen(DB_NAME, "rb+")) == NULL)
	{
		printf("File could not be opened.");
	} else {
		int i = 0;
		int error;
		while(i<NUM_USERS){
			putUser(users_p[i]);
			/*error = pthread_create(&(tid[i]), NULL, &putUser, users_p[i]);
			if(error != 0){
				printf("Thread can't be created\n");
			}*/
			i++;
		}
	}

	fclose(fPtr);
	/*for(int d = 0; d < NUM_USERS; d++){
		pthread_join(tid[d],NULL);
	}*/
	/*
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);
	pthread_join(tid[3], NULL);
	pthread_join(tid[4], NULL);
	pthread_join(tid[5], NULL);
	pthread_join(tid[6], NULL);
	pthread_join(tid[7], NULL);
	pthread_join(tid[8], NULL);
	pthread_join(tid[9], NULL);
	pthread_join(tid[10], NULL);
	*/
	/*
	putUser(users_p[1]);
	putUser(users_p[2]);
	putUser(users_p[3]);
	putUser(users_p[4]);
	*/
	
	printUsersToConsole();
	// Excpeted array actual array
	// Function test() som checker both arrays. 
}


void createFileWithBlankUsers(int numOfUsers){
	unsigned int i;
	user blankUser = {0, ""};
	if((fPtr = fopen(DB_NAME, "wb")) == NULL)
	{
		printf("File could not be opened.");
	} else
	{
		for(i = 1; i <= numOfUsers; i++)
		{
			fwrite(&blankUser, sizeof(user), 1, fPtr);	
		}
		fclose(fPtr);
	}
}

void * putUser(void *userToPut){
	if(fPtr == NULL){
		printf("File could not be opened\n");
	} else {
		(*(user *) userToPut).id = curr_id;
		fseek(fPtr, ( (*(user *)  userToPut).id - 1) * sizeof(user), SEEK_SET);
		fwrite((user *)userToPut, sizeof(user), 1, fPtr);	
		curr_id++;
	}
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

