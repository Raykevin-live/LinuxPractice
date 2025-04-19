#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(){
	char *who = getenv("USER");
	if(strcmp(who, "cf"))
	{
		printf("我不让你执行: %s\n", who);
	}
	else
	{
		printf("Hello, cf!\n");
	}
	return 0;
}
