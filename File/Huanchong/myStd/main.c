#include "Mystdio.h"
#include <unistd.h>

#define myfile "test.txt"
int main()
{
	_FILE *fp = _fopen(myfile, "w");
	if(!fp) return 1;

	const char *msg = "hello world!\n";
	int n = 10;
	while(n--)
	{
		_fwrite(fp, msg, strlen(msg));
		sleep(1);
	}
	// _fflush()
	_fclose(fp);
	return 0;
}
