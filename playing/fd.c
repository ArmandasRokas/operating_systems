#include <stdio.h>
#include <string.h>
#include <unistd.h>

void main(){
	write(1, "hello", strlen("hello"));
}
