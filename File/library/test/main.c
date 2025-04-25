#include "mymath.h"
#include "mylog.h"
#include "myprint.h"

int main()
{
	int n = div(10, 0);
	printf("10/0=%d, errno=%d\n", n, myerrno);
	print();
	Log("Linux test log!");
	return 0;
}
