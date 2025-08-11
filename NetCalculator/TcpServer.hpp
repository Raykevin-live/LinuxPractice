#pragma once
#include "Log.hpp"
#include "Socket.hpp"
#include <signal.h>
#include <functional>

using func_t = std::function<std::string(std::string &package)>;

class TcpServer{
public:
    TcpServer(uint16_t p, func_t c):port_(p), callback_(c){}

    bool InitServer(){
        listensock_.Socket();
        listensock_.Bind(port_);
        listensock_.Listen();
        lg(Info, "init server ... done");
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

            lg(Info, "accept a new link, sockfd: %d, clinetip: %s, clientPort: %d", sockfd, clientip.c_str(), clinetport);

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

                        lg(Debug, "debug: \n%s", inbuffer_stream.c_str());
                        while(true){ //可以处理多个报文
                            std::string info = callback_(inbuffer_stream);
                            if(info.empty()) break;
                            //write
                            lg(Debug, "debug, response: \n%s", info.c_str());
                            lg(Debug, "debug: \n%s", inbuffer_stream.c_str());
                            write(sockfd, info.c_str(), info.size());
                        }
                        
                    }
                    else if(n==0)break;
                    else break;
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