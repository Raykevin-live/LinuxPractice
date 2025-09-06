#pragma once

#include <iostream>
#include "Socket.hpp"
#include <poll.h>
#include <sys/time.h>

static const uint16_t defaultport = 8080;
static const int fd_max_num = 64;
int defaultfd = -1;
int eventnone = 0;
int timeout = 1;
class PollServer
{
public:
    PollServer(uint16_t port = defaultport)
    {
        _port = defaultport;

        for (int i = 0; i < fd_max_num; ++i)
        {
            rds[i].fd = defaultfd;
            rds[i].events = eventnone;
            rds[i].revents = eventnone;
        }
    }

    bool Init()
    {
        _listensock.Socket();
        _listensock.Bind(_port);
        _listensock.Listen();
        return true;
    }
    void Accepter()
    {
        // 我们的连接事件就绪了
        std::string clientip;
        uint16_t clientport = 0;
        int sock = _listensock.Accept(&clientip, &clientport);
        if (sock < 0)
            return;

        lg(Info, "accept success, %s: %d, sockfd: %d", clientip.c_str(), clientport, sock);
        // 只需要把sock -> fd_array[]中即可
        int pos = 1;
        for (; pos < fd_max_num; ++pos) //第二个循环
        {
            if (rds[pos].fd != defaultfd)
                continue;
            else
                break;
        }
        if (pos == fd_max_num)
        {
            lg(Warning, "server is full, close %d now!", sock);
            close(sock);

            // 可以扩容
        }
        else
        {
            rds[pos].fd = sock; //设置新的sock
            rds[pos].events = POLLIN; //关心读事件
            rds[pos].revents = eventnone; //清空返回事件
            PrintFd();
        }
    }
    void Recver(int fd, int pos)
    {
        char buffer[1024];
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1); // bug 收到的数据不一定完整
        if (n > 0)
        {
            buffer[n] = 0;
            std::cout << "get a message: " << buffer << std::endl;
        }
        else if (n == 0)
        {
            lg(Info, "client quit, me too, close fd is : %d", fd);
            close(fd);
            rds[pos].fd = defaultfd; // 移除fd： 本质是从select中移除
        }
        else
        {
            lg(Warning, "recv error: fd is : %d", fd);
            close(fd);
            rds[pos].fd = defaultfd; // 移除fd： 本质是从select中移除
        }
    }
    void Dispatcher()
    {
        for (int i = 0; i < fd_max_num; ++i)
        {
            int fd = rds[i].fd;
            if (fd == defaultfd)
                continue;

            if (rds[i].events & POLLIN) //读事件就绪了
            {
                if (fd == _listensock.Fd()) // 获取新链接
                {
                    Accepter(); // 链接管理器
                }
                else // 读事件就绪了： 不是listen套接字
                {
                    Recver(fd, i);
                }
            }
        }
    }
    void PrintFd()
    {
        std::cout << "online fd list: ";
        for (int i = 0; i < fd_max_num; ++i)
        {
            if (rds[i].fd == defaultfd)
            {
                continue;
            }
            std::cout << rds[i].fd << " ";
        }
        std::cout << "\n";
    }
    void Start()
    {
        rds[0].fd = _listensock.Fd();
        rds[0].events = POLLIN;
        // revents才会被修改
        int timeout = 3000; // 3s
        for (;;)
        {
            int n = poll(rds, fd_max_num, timeout);
            switch (n)
            {
            case 0:
                std::cout << "time out ..."<<std::endl;
                break;
            case -1:
                std::cerr << "poll error ..." << std::endl;
                break;
            default:
                // 有事件就绪了
                std::cout << "get a new link ..." << std::endl;
                Dispatcher(); //事件派发器
                break;
            }
        }
    }
    ~PollServer()
    {
        _listensock.Close();
    }

private:
    Sock _listensock;
    uint16_t _port;
    struct pollfd rds[fd_max_num];
};