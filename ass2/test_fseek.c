#include <stdio.h>
typedef struct user{
	unsigned int id;
	char username[20];
}user ;


int main(){
	FILE * fp1;
	FILE * fp2;
	user blankUser = {0, ""};
	if((fp1 = fopen("users.dat", "rb+")) == NULL){
		printf("the file could not be open\n");
	}
	if((fp2 = fopen("users.dat", "rb+")) == NULL){
		printf("the file could not be open\n");
	}
	printf("fp1: %p, fp2: %p", fp1, fp2);
	fread(&blankUser, sizeof(user), 1, fp1);
        if(blankUser.id != 0){
               printf("%-6d%-16s\n", blankUser.id, blankUser.username);
        }

	

	fseek(fp2, 5*sizeof(user), SEEK_SET );
	
	fread(&blankUser, sizeof(user), 1, fp2);
        if(blankUser.id != 0){
               printf("%-6d%-16s\n", blankUser.id, blankUser.username);
        }

	fclose(fp1);
	fclose(fp2);
}
