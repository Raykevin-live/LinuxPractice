#include <iostream>
#include <memory>
#include "TcpServer.hpp"

int main()
{
	std::unique_ptr<TcpServer> tcp_svr(new TcpServer(8888));
	tcp_svr->Init();
	tcp_svr->Loop();

	return 0;
}
