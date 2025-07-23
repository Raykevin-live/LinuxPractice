#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void Usage(const std::string &proc){
    std::cout<<"\n\rUsage: "<<proc<<" serverip serverport\n"<<std::endl;
}
int main(int argc, char* argv[]){
    if(argc != 3){
        Usage(argv[0]);
        exit(1);
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        std::cerr<<"socket error"<<std::endl;
        return 1;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    inet_pton(AF_INET, serverip.c_str(), &(server.sin_addr));
    // server.sin_addr()
    //tcp 客户端要不要bind，需要绑定，但是不用显示绑定，UDP是在第一次发送数据时确定，TCP在。。。
    // 由系统随机指明端口号，进行绑定, 也就不需要listen
    int n = connect(sockfd, (struct sockaddr*)&server, sizeof(server));
    if(n < 0){
        std::cerr<<"connet error..."<<std::endl;
        return 2;
    }
    std::string message;
    while(true){
        std::cout<<"Please Enter# ";
        std::getline(std::cin, message);
        write(sockfd, message.c_str(), message.size());

        char inbuffer[4096];
        int n = read(sockfd, inbuffer, sizeof(inbuffer));
        if(n > 0){
            inbuffer[n] = 0;
            std::cout<<inbuffer<<std::endl;
        }
    }
    close(sockfd);
    return 0;
}