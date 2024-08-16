#include "ProBar.h"

void  process_bar()
{
	char bar[NUM+1];
	memset(bar, '\0', sizeof(bar));
	
	const char* lable = "|/-\\";
	int i = 0;
	while(i <= NUM)
	{
		printf("Load...[%-100s][%-3d%%][%c]\r", bar, i, lable[i%4]);
		fflush(stdout);
		bar[i++] = '#';
		usleep(10000);
	}
	printf("\n");
}
