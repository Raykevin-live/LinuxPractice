#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
	//const char *fstr = "hello fwrite";
	const char *str = "hello write";

	//printf("hello world"); // stdout->1
	//fprintf(stdout, "hello fprintf"); // stdout->1
	//fwrite(fstr, strlen(fstr), 1, stdout);
	write(1, str, strlen(str)); // 1
	close(1);
	//fork();
	return 0;
}
