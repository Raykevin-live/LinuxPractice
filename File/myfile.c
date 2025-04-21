#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
	printf("stdin->fd: %d\n", stdin->_fileno);
	printf("stdout->fd: %d\n", stdout->_fileno);
	printf("stderr->fd: %d\n", stderr->_fileno);
	//char buffer[1024];
	//	
	//size_t s = read(0, buffer, sizeof(buffer));
	//if(s<0) return 1;
	//buffer[s] = '\0';
	//printf("echo: %s\n", buffer);




	//umask(0);
	//int fd = open("log.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	//if(fd < 0)
	//{
	//	printf("open file error\n");
	//	return 1;
	//}
	//const char *message = "Linux";
	//write(fd, message, strlen(message));
	//close(fd);

	return 0;
}












//int main()
//{
//	//chdir("/home/cf");
//	printf("Pid: %d\n", getpid());
//	FILE* fp = fopen("log.txt", "w");
//	if(!fp)
//	{
//		perror("fopen");
//		return 1;
//	}
//	const char *message = "hello Linux ";
//	fwrite(message, strlen(message), 1, fp);
//	fclose(fp);
//	//sleep(10);
//	return 0;
//}
