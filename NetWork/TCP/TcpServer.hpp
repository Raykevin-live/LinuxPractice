#pragma once
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>
#include "Log.hpp"

const int defaultfd = -1;
const std::string defaultip = "0.0.0.0";
Log lg;

enum{
    SocketError = 2,
    BindError
};

class TcpServer
{
public:
    TcpServer(uint16_t port, const std::string &ip) : sockfd_(defaultfd), port_(port), ip_(ip)
    {
    }
    void InitServer()
    {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd_ < 0){
            lg(Fatal, "create socket, errno: %d, errnostring: %s", errno, strerror(errno));
            exit(SocketError);
        }
        lg(Info, "create socket success, sockfd: %d", sockfd_);

        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port_);
        inet_aton(ip_.c_str(), &(local.sin_addr));

        if(bind(sockfd_, (sockaddr*)(&local), sizeof(local)) < 0){
            lg(Fatal, "bind error, errno: %d, errstring: %s", errno, strerror(errno));
            exit(BindError);
        }
    }
    void Start()
    {
    }
    ~TcpServer()
    {
    }

private:
    int sockfd_;
    uint16_t port_;
    std::string ip_;
};