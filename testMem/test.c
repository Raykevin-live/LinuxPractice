#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int gval = 100;//全局变量
int main()
{
	pid_t id = fork();
	if(id==0){
		// 子进程
		while(1)
		{
			printf("我是子进程, pid: %d, ppid: %d, gval: %d, &gval: %p\n", getpid(), getppid(), gval, &gval);
			sleep(1);
			gval++;
		}
	}
	else 
	{
		// 父进程
		while(1)
		{
			printf("我是父进程, pid: %d, ppid: %d, gval: %d, &gval: %p\n", getpid(), getppid(), gval, &gval);
			sleep(1);
		}
	}
	return 0;
}
