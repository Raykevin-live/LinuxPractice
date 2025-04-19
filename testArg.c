#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]){
	int i=0;
	for(;argv[i]; i++){
		printf("argv[%d]: %s\n", i, argv[i]);
	}
	//if(argc!=2){
	//	printf("Usage: %s -v1/-v2/-v3\n", argv[0]);
	//	return 1;
	//}
	//printf("argc = %d\n", argc);
	//if(strcmp(argv[1], "-v1")==0)
	//	printf("这是功能1\n");
	//else if(strcmp(argv[1], "-v2")==0)
	//	printf("这是功能2\n");
	//else if(strcmp(argv[1], "-v3")==0)
	//	printf("这是功能3\n");
	//else{
	//	printf("功能不支持\n");
	//}
	return 0;
}


