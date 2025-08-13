#include "Socket.hpp"
#include <iostream>
#include <iostream>
#include <time.h>
#include "Protocol.hpp"
#include <assert.h>

static void Usage(const std::string& proc){
    std::cout<<"\nUsage: "<<proc <<" serverip serverport\n\n"<<std::endl;
}

// .clientcal ip port
int main(int argc, char* argv[]){
    if(argc != 3){
        Usage(argv[0]);
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    Sock sockfd;
    sockfd.Socket();
    bool r = sockfd.Connect(serverip, serverport);
    if(!r){
        return 1;
    }

    srand(time(nullptr)^getpid());
    int cnt = 10;
    const std::string opers = "+-*/%=$#";

    std::string inbuffer_stream;
    while(cnt--){
        std::cout<<"第"<<10-cnt<<"次测试.....\n";
        int x = rand()%100+1;
        usleep(1234);
        int y = rand()%100;
        usleep(2345);
        char op = opers[rand()%opers.size()];

        Request req(x, y, op);
        req.DebugPrint();

        std::string package;
        req.Serialize(&package);

        package = Encode(package);

        /// 模拟多个请求
        std::cout<<"打印最新的发出请求: \n"<<package;
        ssize_t len = write(sockfd.Fd(), package.c_str(), package.size());
        // std::cout<<"打印最新的发出请求: \n"<<package;
        // len = write(sockfd.Fd(), package.c_str(), package.size());
        // std::cout<<"打印最新的发出请求: \n"<<package;
        // len = write(sockfd.Fd(), package.c_str(), package.size());
        // std::cout<<"打印最新的发出请求: \n"<<package;
        // len = write(sockfd.Fd(), package.c_str(), package.size());
        // std::cout<<"打印最新的发出请求: \n"<<package;
        // len = write(sockfd.Fd(), package.c_str(), package.size());

        char buffer[128];
        ssize_t n = read(sockfd.Fd(), buffer, sizeof(buffer)); //无法保证读到完整的报文
        if(n > 0){
            buffer[n] = 0;
            inbuffer_stream += buffer;
            std::string content;
            bool r = Decode(inbuffer_stream, &content);
            assert(r);

            Response resp;
            r = resp.Deserialize(content);
            assert(r);

            resp.DebugPrint();
        }
        sleep(1);
    }
    sockfd.Close();
    return 0;
}