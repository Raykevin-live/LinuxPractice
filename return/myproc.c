#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
int main()
{
	pid_t id = fork();

	if(id==0)
	{
		int cnt = 5;
		while(cnt--)
		{
			printf("子进程: %d\n", getpid());
			sleep(1);
		}
		exit(0);
	}
	int status = 0;
	while(1)
	{
		pid_t rid = waitpid(id, &status, WNOHANG);
		if(rid > 0)
		{
			printf("wait success\n");
			if(WIFEXITED(status))
			{
				printf("正常运行结束: %d\n", WEXITSTATUS(status));
			}
			else
			{
				printf("进程异常了\n");
			}
			break;
		}
		else if(rid < 0)
		{
			printf("wait failed!\n");
			break;
		}
		else
		{
			printf("子进程还没有结束，我在等待...\n");
			sleep(1);
		}
	}
}
