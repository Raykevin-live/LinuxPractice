#pragma once
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>
#include <pthread.h>

#include "Log.hpp"
#include "ThreadPool.hpp"
#include "Task.hpp"
#include "Daemon.hpp"


const int defaultfd = -1;
const std::string defaultip = "0.0.0.0";
const int backlog = 5; //一般不要设置的太大

enum{
    SocketError = 2,
    BindError,
    ListenError,
    AcceptError
};

class TcpServer; //类声明
class ThreadData{
public:
    ThreadData(int fd, const std::string &ip, const uint16_t &p, TcpServer* t):sockfd(fd), clientip(ip), clientport(p), tsvr(t){}
public:
    int sockfd;
    std::string clientip;
    uint16_t clientport;
    TcpServer *tsvr; //传送this指针，否则类中使用Service函数会出错
};
class TcpServer
{
public:
    TcpServer(uint16_t port, const std::string &ip = defaultip) : listensock_(defaultfd), port_(port), ip_(ip)
    {
    }
    void InitServer()
    {
        listensock_ = socket(AF_INET, SOCK_STREAM, 0);
        if(listensock_ < 0){
            lg(Fatal, "create socket, errno: %d, errnostring: %s", errno, strerror(errno));
            exit(SocketError);
        }
        lg(Info, "create socket success, sockfd: %d", listensock_);

        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port_);
        inet_aton(ip_.c_str(), &(local.sin_addr));

        if(bind(listensock_, (sockaddr*)(&local), sizeof(local)) < 0){
            lg(Fatal, "bind error, errno: %d, errstring: %s", errno, strerror(errno));
            exit(BindError);
        }
        // TCP是面向连接的，服务器一般是“被动的”，服务器一直处于一种一直在等待连接到来的状态
        lg(Info, "bind socket success, socket: %d", listensock_);
        if(listen(listensock_, backlog) < 0){
            lg(Fatal, "listen error, errno: %d, errstring: %s", errno, strerror(errno));
            exit(ListenError);
        }
        lg(Info, "listen socket success, socket: %d", listensock_);

    }
    static void *Routine(void* arg){//静态，消去第一个参数
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData*>(arg);
        td->tsvr->Service(td->sockfd, td->clientip, td->clientport);//加入TcpServer指针
        delete td;
        return nullptr;
    }
    void Start()
    {
        Daemon();
        ThreadPool<Task>::GetInstance()->Start();
        
        lg(Info, "tcpServer is running ...");
        sleep(1);
        for(;;){
            // 1.获取新连接
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            int sockfd = accept(listensock_, (struct sockaddr*)&client, &len);
            if(sockfd < 0 ){
                lg(Warning, "accept error, errno: %d, errstring: %s", errno, strerror(errno));
                continue;
            }
            uint16_t clientport = ntohs(client.sin_port);
            char clientip[32];// 这两个参数是写给操作系统的，会自动转换大小端
            inet_ntop(AF_INET, &(client.sin_addr), clientip, sizeof(clientip));//还有这个参数
            //2.根据新连接来进行通信
            lg(Info, "get a new link ..., sockfd: %d, client ip: %s, client port: %d", sockfd, clientip, clientport);
            
            // version1：单进程版
            // Service(sockfd, clientip, clientport);
            // close(sockfd);

            // version2: 多进程版
            // pid_t id = fork();
            // if(id == 0){
            //     close(listensock_);// 这里可以直接关掉
            //     if(fork() > 0) exit(0);//再fork一个子进程出来, 由于子进程直接退出，所以孙子进程变孤儿由系统领养并自动释放
            //     //child，后面由孙子进程提供服务
            //     Service(sockfd, clientip, clientport);
            //     close(sockfd);
            //     exit(0);
            // }
            // close(sockfd);
            // // father
            // pid_t rid = waitpid(id, nullptr, 0); // 为了避免父进程需要等待子进程结束才执行（阻塞等待，但是设置为非阻塞轮询也不是很好的一个方案）
            // (void)rid; //防止报未使用错误

            //version3：多线程版
            // ThreadData *td = new ThreadData(sockfd, clientip, clientport, this);
            // pthread_t tid;
            // pthread_create(&tid, nullptr, Routine, td);

            // Version4: 线程池版
            Task t(sockfd, clientip, clientport);
            ThreadPool<Task>::GetInstance()->Push(t);

        }

    }
    void Service(int sockfd, const std::string &clientip, const uint16_t &clientport){
        // 测试代码
        char buffer[4096];
        while(true){
            ssize_t n = read(sockfd, buffer, sizeof(buffer));
            if(n > 0){
                buffer[n] = 0;
                std::cout<<"client say# "<<buffer<<std::endl;
                std::string echo_string = "tcpserver echo# ";
                echo_string += buffer;

                write(sockfd, echo_string.c_str(), echo_string.size());
            }
            else if(n == 0){
                lg(Info, "%s:%d quit, server close sockfd: %d", sockfd);
                break;
            }
            else{
                lg(Warning, "read error, sockfd: %d, client ip: %s, client port: %d", sockfd, clientip.c_str(), clientport);
                break;
            }
        }
    }

    ~TcpServer()
    {

    }

private:
    int listensock_;
    uint16_t port_;
    std::string ip_;
};