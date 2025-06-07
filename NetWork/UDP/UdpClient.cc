#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include "Terminal.hpp"

using namespace std;

void Usage(std::string proc)
{
    std::cout << "\n\rUsage: " << proc << " serverip" << " port[1024+]\n"
              << std::endl;
}

struct ThreadData
{
    struct sockaddr_in server;
    int socketfd;
};

void *resv_message(void *args)
{
    // OpenTerminal();
    ThreadData *td = static_cast<ThreadData *>(args);
    char buffer[1024];
    // 接收消息
    sockaddr_in temp;
    socklen_t len = sizeof(temp);
    while(true){
         ssize_t s = recvfrom(td->socketfd, buffer, 1023, 0, (struct sockaddr *)&temp, &len);
        if (s > 0)
        {
            buffer[s] = '\0';
            // std::cout << buffer << std::endl;
            std::cerr << buffer << std::endl;
        }
    }
   
    return nullptr;
}

void *send_message(void *args)
{
    ThreadData *td = static_cast<ThreadData *>(args);
    std::string messages;
    socklen_t len = sizeof(td->server);
    while (true)
    {
        // 1.数据
        cout << "Please enter@ ";
        std::getline(cin, messages);
        // 发送消息
        sendto(td->socketfd, messages.c_str(), messages.size(), 0, (struct sockaddr *)&td->server, len);
    }
    return nullptr;
}
// .udpclient serverip serverport
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(0);
    }

    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    ThreadData td;
    // 信息初始化
    sockaddr_in server;
    bzero(&td.server, sizeof(td.server));

    td.server.sin_addr.s_addr = inet_addr(serverip.c_str());
    td.server.sin_port = htons(serverport); // 注意这里主机转网络
    td.server.sin_family = AF_INET;

    td.socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (td.socketfd < 0)
    {
        cout << "socket error" << endl;
        return 1;
    }

    // 创建线程
    pthread_t resver, sender;
    pthread_create(&resver, nullptr, resv_message, &td);
    pthread_create(&sender, nullptr, send_message, &td);
    pthread_join(resver, nullptr);
    pthread_join(sender, nullptr);

    // client 要绑定吗？需要的，但是是由OS自由随机选择；
    // 一个端口号只能被一个进程绑定，对server是如此，对client也是如此。
    // 因为客户端的端口是多少并不重要，只要能保证主机上的唯一性就可以。

    /* 读写分离（多线程）
    string messages;
    char buffer[1024];
    while (true)
    {

    }
    */

    close(td.socketfd);
    return 0;
}