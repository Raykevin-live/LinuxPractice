#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>

using namespace std;

void Usage(std::string proc)
{
    std::cout << "\n\rUsage: " << proc << " serverip" << " port[1024+]\n"
              << std::endl;
}

// .udpclient serverip serverport

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(0);
    }
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        cout << "socket error" << endl;
        return 1;
    }

    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    // 信息初始化
    sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(serverip.c_str());
    server.sin_port = htons(serverport); // 注意这里主机转网络
    server.sin_family = AF_INET;
    socklen_t len = sizeof(server);
    bzero(&server, sizeof(server));
    // client 要绑定吗？需要的，但是是由OS自由随机选择；
    // 一个端口号只能被一个进程绑定，对server是如此，对client也是如此。
    // 因为客户端的端口是多少并不重要，只要能保证主机上的唯一性就可以。
    string messages;
    char buffer[1024];
    while (true)
    {
        // 1.数据
        cout << "Please enter@ ";
        std::getline(cin, messages);
        // 发送消息
        sendto(sockfd, messages.c_str(), messages.size(), 0, (sockaddr *)&server, len);
        // 接收消息
        sockaddr_in temp;
        socklen_t len = sizeof(temp);

        ssize_t s = recvfrom(sockfd, buffer, 1023, 0, (sockaddr*)&temp, &len);
        if(s > 0){
            buffer[s] = '\0';
            std::cout<<buffer<<std::endl;
        }
    }
    close(sockfd);
    return 0;
}