#include "UdpServer.hpp"
#include <memory>
#include <string>

void Usage(std::string proc){
    std::cout<<"\n\rUsage: "<<proc<<" pror[1024+]\n"<<std::endl;
}

std::string Handler(const std::string &str){
    std::string res = "Server get a message: ";
    res += str;
    return res;
}
int main(int argc, char* argv[]){
    if(argc != 2){
        Usage(argv[0]);
        exit(0);
    }
    
    uint16_t port = std::stoi(argv[1]);
    std::unique_ptr<UdpServer> svr(new UdpServer(port));

    svr->Init();
    svr->Run(Handler);

    return 0;
}