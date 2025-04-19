#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int print()
{
	fprintf(stdout, "hello fprintf()!\n");
	//exit (10);
	return 0;
}
int main()
{
	printf("我是一个进程!"); //输出的字符会在缓冲区中字符
	print();
	sleep(3);
	_exit(1);



	//printf("我的进程开始了！\n");
	//sleep(1);
	//print();
	//printf("我的程序正常结束了!\n");
	//return 4;
}
