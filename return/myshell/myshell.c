#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define LEFT "["
#define RIGHT "]"
#define LABLE "#"
#define DELIM " "
#define LINE_SIZE 1024
#define ARGC_SIZE 30
#define EXIT_CODE 44

#define NONE -1
#define IN_RDIR 0
#define OUT_RDIR 1
#define APPEND_RDIR 2

int lastcode = 0;
extern char **environ;	
char commandline[LINE_SIZE];
char *argv[ARGC_SIZE];
char pwd[LINE_SIZE];
char myenv[LINE_SIZE];
int quit = 0;
char *rdirfilename = NULL;
int rdir = NONE;
const char *getusername()
{
	return getenv("USER");
}
const char* gethostname()
{
	return getenv("HOSTNAME");
}
void getpwd()
{
	getcwd(pwd, sizeof(pwd)-1);
}
void check_redir(char *cmd)
{
	//ls -al >/</>> filename.txt
	char *pos = cmd;
	while(*pos)
	{
		if(*pos=='>')
		{
			*pos++ = '\0';
			if(*pos =='>')
			{
				*pos++ = '\0';
				while(isspace(*pos)) pos++;
				rdirfilename = pos;
				rdir = APPEND_RDIR;
				break;
			}
			else
			{
				while(isspace(*pos)) pos++;
				rdirfilename = pos;
				rdir = OUT_RDIR;
				break;
			}
		}
		else if(*pos=='<')
		{
			*pos ++ = '\0';
			while(isspace(*pos)) pos++;
			rdirfilename = pos;
			rdir = IN_RDIR;
			break;
		}
		pos++;
	}
}

void interact(char *cline, int size)
{
	getpwd();
	printf(LEFT"%s@%s:%s"RIGHT""LABLE" ", getusername(), gethostname(), pwd);
	char *s = fgets(cline, size, stdin);
	assert(s);
	(void)s; // 有些编译器声明了变量不使用会报错
	cline[strlen(cline)-1] = '\0'; // 去掉结尾的 '\n'
	// ls -a -l > myfile.txt
	check_redir(cline);
}
int splitstring(char cline[], char *_argv[])
{
	int i = 0;
	_argv[i++] = strtok(cline, DELIM);// 第一次传字符，之后传NULL
	while(_argv[i++] = strtok(NULL, DELIM));// 故意写的=
	return i-1;
}
void normalexcute(char *_argv[])
{
	pid_t id = fork();
	if(id < 0)
	{
		perror("fork failed\n");
		return;
	}
	else if(id == 0)
	{
		// 重定向
		int fd = 0;
		if(rdir == IN_RDIR)
		{
			fd = open(rdirfilename, O_RDONLY);
			dup2(fd, 0);
		}
		else if(rdir == OUT_RDIR)
		{
			fd = open(rdirfilename, O_CREAT|O_WRONLY|O_TRUNC, 0666);
			dup2(fd, 1);
		}
		else if(rdir == APPEND_RDIR)
		{
			fd = open(rdirfilename, O_CREAT|O_WRONLY|O_APPEND, 0666);
			dup2(fd, 1);
		}
		// 子进程执行命令
		//execvpe(_argv[0], _argv, environ);
		execvp(_argv[0], _argv); // 子进程也可以拿到环境变量
		if(fd!= 0)
		{
			close(fd);
		}
		exit(EXIT_CODE);
	}
	else
	{
		int status = 0;
		pid_t rid = waitpid(id, &status, 0);
		if(rid==id) 
		{
			lastcode = WEXITSTATUS(status);
		}
	}
}
int buildcommand(int _argc, char *_argv[])
{
	if(_argc==2 && !strcmp(_argv[0], "cd"))
	{
		chdir(_argv[1]);
		getpwd();
		sprintf(getenv("PWD"), "%s", pwd);
		//putenv(argv[1]);
		return 1;
	}
	else if(_argc==2 && !strcmp(_argv[0], "export"))
	{
		// 目前只维护了一个自定义环境变量
		strcpy(myenv, _argv[1]);
		putenv(myenv);
		return 1;
	}
	else if(_argc==2 && !strcmp(_argv[0], "echo"))
	{
		if(!strcmp(_argv[1], "$?"))
		{
			printf("%d\n", lastcode);
			lastcode = 0;
		}
		else if(*_argv[1] == '$')
		{
			char *val = getenv(_argv[1]+1);
			if(val) printf("%s\n", val);// _argv[1]+1 从$ 指向下一个
		}
		else
		{
			printf("%s\n", _argv[1]);
		}
		return 1;
	}
	// 特殊处理
	if(!strcmp(_argv[0], "ls")) 
	{
		_argv[_argc++]="--color";
		_argv[_argc] = NULL;
	}
	return 0;

}
int main()
{
	while(!quit)
	{
		rdirfilename = NULL;
		rdir = NONE;
		// 2. 交互问题，获取命令行
		interact(commandline, sizeof(commandline));
		int argc = splitstring(commandline, argv);
		// 3. 字串的分割问题
		if(argc==0) continue;
		// 4. 指令的判断
		//内键命令，本质就是shell内部的一个函数
		int n = buildcommand(argc, argv);

		// 实现管道通信
		// 4.0 分析输入的命令中含有几个"|", 命令打散成多个子命令字符串
		// 4.1 malloc申请空间，pipe先申请多个管道
		// 4.2 循环创建多个子进程，每个子进程的重定向情况。
		//		1. 最开的：输出重定向 1 -> 指定的一个管道写端
		//		2. 中间的：输入输出重定向 0->上一个管道读端，1->下一个管道的写端
		//		3. 最后一个：输入重定向 0->将标准输入重定向到最后一个管道的读端
		// 4.3 分别让不同的子进程执行不同的命令---exec* 系统调用进行替换，因为--exec* 不会影响曾经打开的文件，不会影响预先设置好的重定向
		// 
		// 5. 普通命令的执行
		if(!n) normalexcute(argv);
		//printf("echo: %s\n", commandline);

		//debug
		//for(int i=0; argv[i]; i++)
		//{
		//	printf("[%d]: %s\n", i, argv[i]);
		//}
	}
	return 0;
}
