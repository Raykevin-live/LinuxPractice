#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
	const char *fstr = "hello fwrite";
	const char *str = "hello write";

	printf("hello printf"); // stdout->1
	//sleep(2);
	fprintf(stdout, "hello fprintf"); // stdout->1
	//sleep(2);
	fwrite(fstr, strlen(fstr), 1, stdout);
	//sleep(2);

	write(1, str, strlen(str)); // 1
	//sleep(5);
	//close(1);
	fork();
	return 0;
}
