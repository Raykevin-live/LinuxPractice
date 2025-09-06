#pragma once

#include <iostream>
#include "Socket.hpp"
#include <sys/select.h>
#include <sys/time.h>

static const uint16_t defaultport = 8080;
static const int fd_max_num = (sizeof(fd_set) * 8);
int defaultfd = -1;

class SelectServer
{
public:
    SelectServer(uint16_t port = defaultport)
    {
        _port = defaultport;

        for (int i = 0; i < fd_max_num; ++i)
        {
            fd_array[i] = defaultfd;
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
        for (; pos < fd_max_num; ++pos)
        {
            if (fd_array[pos] != defaultfd)
                continue;
            else
                break;
        }
        if (pos == fd_max_num)
        {
            lg(Warning, "server is full, close %d now!", sock);
            close(sock);
        }
        else
        {
            fd_array[pos] = sock;
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
            fd_array[pos] = defaultfd; // 移除fd： 本质是从select中移除
        }
        else
        {
            lg(Warning, "recv error: fd is : %d", fd);
            close(fd);
            fd_array[pos] = defaultfd; // 移除fd： 本质是从select中移除
        }
    }
    void Dispatcher(fd_set &rfds)
    {
        for (int i = 0; i < fd_max_num; ++i)
        {
            int fd = fd_array[i];
            if (fd == defaultfd)
                continue;

            if (FD_ISSET(_listensock.Fd(), &rfds))
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
            if (fd_array[i] == defaultfd)
            {
                continue;
            }
            std::cout << fd_array[i] << " ";
        }
        std::cout << "\n";
    }
    void Start()
    {
        int listensock = _listensock.Fd();
        fd_array[0] = listensock;
        for (;;)
        {
            fd_set rfds;
            FD_ZERO(&rfds);
            int maxfd = fd_array[0];
            for (int i = 0; i < fd_max_num; ++i) // 第一次循环
            {
                if (fd_array[i] == defaultfd)
                {
                    continue;
                }
                FD_SET(fd_array[i], &rfds);
                if (maxfd < fd_array[i])
                {
                    maxfd = fd_array[i];
                }
            }
            // 这里不能accept? 因为这样只能一次检测一个端口号

            // FD_SET(listensock, &rfds);

            struct timeval timeout = {1, 0}; // 输入输出，可能要进行周期的重复设置
            // struct timeval timeout = {0, 0}; // 全设为0，表示非阻塞轮询

            // 如果事件就绪，上层不处理，select会一直通知你！
            // 如果select告诉你就绪了，接下来的一次读取，我们读取fd的时候不会被阻塞
            int n = select(maxfd + 1, &rfds, nullptr, nullptr, &timeout);
            // int n = select(listensock+1, &rfds, nullptr, nullptr, nullptr /* 设置为nullptr表示一直轮询直到就绪*/);
            switch (n)
            {
            case 0:
                std::cout << "time out, timeout: " << timeout.tv_sec << "." << timeout.tv_usec << std::endl;
                break;
            case -1:
                std::cerr << "select error" << std::endl;
                break;
            default:
                // 有事件就绪了
                std::cout << "get a new link ..." << std::endl;
                Dispatcher(rfds); //事件派发器
                break;
            }
        }
    }
    ~SelectServer()
    {
        _listensock.Close();
    }

private:
    Sock _listensock;
    uint16_t _port;
    int fd_array[fd_max_num]; ///< 辅助数组
};