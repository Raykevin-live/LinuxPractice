#pragma once
#include "Log.hpp"
#include "Socker.hpp"
#include <signal.h>
#include <functional>

using func_t = std::function<std::string(std::string &package)>;

class TcpServer{
public:
    TcpServer(uint16_t p, func_t c):port_(p), callback_(c){}
    {}
    bool InitServer(){
        listensock_.Socket();
        listensock_.Bind(port_);
        listensock_.Listen();
        return true;
    }
    void Start(){
        signal(SIGCHLD, SIG_IGN);
        signal(SIGPIPE, SIG_IGN);
        while(true){
            std::string clientip;
            uint16_t clinetport;
            int sockfd = listensock_.Accept(&clientip, &clinetport);
            if(sockfd < 0) continue;


            // 提供服务
            if(fork()==0){
                listensock_.Close();
                std::string inbuffer_stream;
                // 数据计算
                while(true){
                    char buffer[128];
                    ssize_t n = read(sockfd, buffer, sizeof(buffer));
                    if(n > 0){
                        buffer[n] = 0;
                        inbuffer_stream += buffer;
                        std::string info = callback_(inbuffer_stream);

                        //write
                    }
                }
                exit(0);
            }
            close(sockfd);
        }
    }
    ~TcpServer()
    {}
private:
    uint16_t port_;
    Sock listensock_;
    func_t callback_;
};