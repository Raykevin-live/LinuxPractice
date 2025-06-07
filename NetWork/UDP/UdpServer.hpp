#pragma once
#include <iostream>
#include "Log.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <functional>
#include <unordered_map>

using func_t = std::function<std::string(const std::string&, const std::string&, uint16_t)>;
Log lg;

enum{
    SOCKET_ERR = 1,
    BIND_ERR
};

const int size = 1024;

uint16_t defaultport = 8080;
std::string defaultip = "0.0.0.0";
class UdpServer{
public:
    UdpServer(uint16_t port = defaultport, const std::string &ip = defaultip)
        :_port(port), _ip(ip), _isrunning(false)
        {}

    void Init(){
        // 1.创建udp socket
        _socketfd = socket(AF_INET, SOCK_DGRAM, 0); //PF_INET
        if(_socketfd < 0){
            lg(Fatal, "socket create error, sockfd: %d", _socketfd);
            exit(SOCKET_ERR);
        }
        lg(Info, "socket create success, sockfd: %d", _socketfd);
        // 2.绑定 socket
        struct sockaddr_in local;
        bzero(&local, sizeof(local));

        local.sin_family = AF_INET;
        // local.sin_addr.s_addr = inet_addr(_ip.c_str()); //1.字符串->uint32_t；2.uint32_t 必须是网络序列的
        local.sin_port = htons(_port); //保证端口号是网络字节序列，因为该端口号是要给对方发送的
        local.sin_addr.s_addr = INADDR_ANY;
        int n = bind(_socketfd, (const sockaddr*)&local, sizeof(local));
        if(n < 0){
            lg(Fatal, "bind error: %d, err string: %s", errno, strerror(errno));
            exit(BIND_ERR);
        }
        lg(Info, "bind success, addr: %s", _ip.c_str());

    }
    void UserCheck(const std::string& ip, const struct sockaddr_in& client){
        auto iter = user_list.find(ip);
        if(iter==user_list.end()){
            user_list.insert({ip, client});
            std::cout<<"User# ["<<ip<<"] is add.."<<std::endl;
        }
    }
    void Broadcast(const std::string& info){
        for(const auto& user : user_list){
            socklen_t len = sizeof(user.second);
            sendto(_socketfd, info.c_str(), info.size(), 0, (sockaddr*)&user.second, len);
        }
    }
    void Run(func_t func){ // 对代码进行分层
        _isrunning = true;
        char inbuffer[size];
        while(_isrunning){
            sockaddr_in client;
            socklen_t len = sizeof(client);
            ssize_t n = recvfrom(_socketfd, inbuffer, sizeof(inbuffer), 0, (sockaddr*)&client, &len);
            if(n < 0){
                lg(Warning, "recvfrom error, errno: %d, err string: %s", errno, strerror(errno));
                continue;
            }
            inbuffer[n] = '\0';

            uint16_t clientport = ntohs(client.sin_port);
            std::string clientip = inet_ntoa(client.sin_addr); 
            UserCheck(clientip, client);
            // 处理一次数据
            std::string info = func(inbuffer, clientip, clientport);

            Broadcast(info);
            // std::string echo_string = "server echo# "+info;
            // std::string echo_string = func(info, clientip, clientport);// 处理字符串
            // sendto(_socketfd, echo_string.c_str(), echo_string.size(), 0, (sockaddr*)&client, len);
        }

    }
    ~UdpServer(){
        if(_socketfd > 0){
            close(_socketfd);
        }
        _isrunning = false;
    }
private:
    int _socketfd;// 网络文件描述符
    std::string _ip;
    uint16_t _port;// 表明服务器进程的端口号
    bool _isrunning;
    std::unordered_map<std::string, struct sockaddr_in> user_list;
};