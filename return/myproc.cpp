#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
int main()
{
	int i=0;
	std::vector<int> subIds;
	for(; i<10; i++)
	{
		pid_t id = fork();
		if(id==0)
		{
			int i=3;
			while(1)
			{
				printf("我是一个子进程，pid: %d, ppid: %d, count: %d\n", getpid(), getppid(), i);
				sleep(3);
				i--;
				break;
			}
			//printf("子进程退出了\n");
			int ret = 1;
			if(ret==0)
				exit(0);
			else
				exit(1);
			exit(0);
		}
		subIds.push_back(id);
	}
	sleep(5);
	// 父进程
	for(auto sid : subIds)
	{
		//pid_t rid = wait(NULL);
		int status = 0;
		printf("父进程开始等待子进车ing..., %d\n", sid);
		pid_t rid = waitpid(sid, &status, 0);
		if(rid > 0)
		{
			int exit_code = (status>>8)&0xFF;
			int exit_signal = status&0x7F;
			if(exit_code==0 && exit_signal==0)
			{
				printf("子进程运行完毕，结果正确\n");
			}
			else if(exit_code > 0 && exit_signal ==0)
			{
				printf("程序运行完毕，结果不正确, %d:%s\n", exit_code, strerror(exit_code));
			}
			else
			{
				printf("子进程出现异常了, exit_signal: %d\n", exit_signal);
			}
			//printf("父进程等待子进程成功，子进程pid: %d, status: %d, exit_code: %d, exit_signal: %d\n", rid, status, (status>>8)&0xFF, status&0x7F);
			//sleep(1);
		}
		else
		{
			printf("子进程等待失败\n");
		}
	}
	return 0;
}

