#include <iostream>

int main(int argc, char *argv[], char *env[])
{
	std::cout<< "程序"<< argv[0] <<" running..." << std::endl;
	std::cout<<"这是参数列表"<<std::endl;
	for(int i=0; argv[i]; i++)
	{
		std::cout<<i<<" : "<<argv[i]<<std::endl;
	}
	std::cout<<"这是环境变量"<<std::endl;
	for(int i=0; env[i]; i++)
	{
		std::cout<<i<<" : "<<env[i]<<std::endl;
	}
	
	return 0;
}
