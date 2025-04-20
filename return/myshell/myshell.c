#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define LEFT "["
#define RIGHT "]"
#define LABLE "#"
#define DELIM " "
#define LINE_SIZE 1024
#define ARGC_SIZE 30
#define EXIT_CODE 44

int lastcode = 0;
extern char **environ;	
char commandline[LINE_SIZE];
char *argv[ARGC_SIZE];
char pwd[LINE_SIZE];
char myenv[LINE_SIZE];
int quit = 0;

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
void interact(char *cline, int size)
{
	getpwd();
	printf(LEFT"%s@%s:%s"RIGHT""LABLE" ", getusername(), gethostname(), pwd);
	char *s = fgets(cline, size, stdin);
	assert(s);
	(void)s; // 有些编译器声明了变量不使用会报错
	cline[strlen(cline)-1] = '\0'; // 去掉结尾的 '\n'
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
		// 子进程执行命令
		//execvpe(_argv[0], _argv, environ);
		execvp(_argv[0], _argv); // 子进程也可以拿到环境变量
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
		// 2. 交互问题，获取命令行
		interact(commandline, sizeof(commandline));
		int argc = splitstring(commandline, argv);
		// 3. 字串的分割问题
		if(argc==0) continue;
		// 4. 指令的判断
		//内键命令，本质就是shell内部的一个函数
		int n = buildcommand(argc, argv);
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
