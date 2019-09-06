#include <stdio.h>

int main(){
	char * strings[2];

	strings[0] = "l";
	strings[1] = "aaaa";
	strings[0] = "bbbb";

	printf("%s %s", strings[0], strings[1]);

}
