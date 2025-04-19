#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{	putenv("MYENV=456");	
	pid_t id = fork();
	if(id==0)
	{
		printf("before: I am a process, pid: %d, ppid: %d\n", getpid(), getppid());
		//sleep(5);
		// 这类方法的标准写法
		//execl("/usr/bin/ls", "ls", "-l", "-a", NULL); // 必须以NULL结尾
		char *const myargv[] =
		{
			"myother",
			"-a",
			"-b",
			"-c",
			NULL
		};
		char *const myenv[]=
		{
			"MYVAL=1111",
			"MYPATH=/usr/bin/XXXX",
			NULL
		};
		extern char **environ;
		execle("./myother", "myother", "-a", "-b", NULL, myenv);
		printf("after: I am a process, pid: %d, ppid: %d\n", getpid(), getppid());
		exit(0);
	}	
	// 父进程
	pid_t ret = waitpid(id, NULL, 0);
	if(ret > 0) printf("Wait success!, father_pid: %d, ret_id: %d\n", getpid(), ret);
	sleep(5);
	return 0;
}
