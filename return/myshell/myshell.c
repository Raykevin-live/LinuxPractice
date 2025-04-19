#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define LEFT "["
#define RIGHT "]"
#define LABLE "#"
#define LINE_SIZE 1024


const char *getusername()
{
	return getenv("USER");
}
const char* gethostname()
{
	return getenv("HOSTNAME");
}
const char* getpwd()
{
	return getenv("PWD");
}
void interact(char *cline, int size)
{
	printf(LEFT"%s@%s:%s"RIGHT""LABLE" ", getusername(), gethostname(), getpwd());
	char *s = fgets(cline, size, stdin);
	assert(s);
	(void)s; // youxiebianyiqi, dingyile bushi yong hui baoming
	cline[strlen(cline)-1] = '\0'; // chulizifuchuan de '\n''
}
int main()
{
	char commandline[LINE_SIZE];
	interact(commandline, sizeof(commandline));
	printf("echo: %s\n", commandline);
	return 0;
}
