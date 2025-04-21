#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define filename "log.txt"

int main()
{
	//close(1);
	int fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, 0666);
	if(fd < 0)
	{
		perror("open");
		return 1;
	}
	dup2(fd, 1);
	close(fd);
	printf("fd: %d\n", fd);
	printf("Hello Linux!\n");
	fprintf(stdout, "hello world!\n");

	//const char* message = "hello Linux!\n";
	//int cnt = 5;
	//while(cnt--)
	//{
	//	write(1, message, strlen(message));
	//}

	return 0;
}
