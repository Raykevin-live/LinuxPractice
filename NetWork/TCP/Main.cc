#include "TcpServer.hpp"
#include <memory>

void Usage(std::string proc){
    std::cout<<"\n\rUsage: "<<proc<<" pror[1024+]\n"<<std::endl;
}
// .tcpserver 8080
int main(int argc, char *argv[]){
    if(argc != 2){
        Usage(argv[0]);
    }
    uint16_t port = std::stoi(argv[1]);
    std::unique_ptr<TcpServer> tcp_svr(new TcpServer(port));

    tcp_svr->InitServer();
    Daemon(); // 守护进程
    tcp_svr->Start();

    return 0;
}