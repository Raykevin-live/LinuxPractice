#pragma once

#include <iostream>
#include <sys/epoll.h>
#include "Socket.hpp"
#include "Epoller.hpp"

uint32_t EVENT_IN = (EPOLLIN);
uint32_t EVENT_OUT = (EPOLLOUT);

class EpollServer
{
    static const int num = 64;

public:
    EpollServer(uint16_t port)
        : _port(port), _listensocket_ptr(new Sock()), _epoller_ptr(new Epoller)
    {
    }
    void Init()
    {
        _listensocket_ptr->Socket();
        _listensocket_ptr->Bind(_port);
        _listensocket_ptr->Listen();

        lg(Info, "create listen socket success: %d\n", _listensocket_ptr->Fd());
    }
    void Accepter()
    {
        // 获取了一个新连接
        std::string clientip;
        uint16_t clientport;
        int sock = _listensocket_ptr->Accept(&clientip, &clientport);
        if (sock > 0)
        {
            // 获取成功
            // 这里不能直接读取数据，因为连接建立好了，但是不一定发送了数据
            // 需要交给epoll管理
            _epoller_ptr->EpollerUpdate(EPOLL_CTL_ADD, sock, EVENT_IN);
            lg(Info, "get a new link, client info@ %s:%d", clientip.c_str(), clientport);
        }
    }
    void Recver(int fd)
    {
        char buffer[1024];
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1); // FIXME: 收到的数据不一定完整
        if (n > 0)
        {
            buffer[n] = 0;
            std::cout << "get a message: " << buffer << std::endl;

            // write
            std::string echo_str = "server echo $ "+ std::string(buffer);
            write(fd, echo_str.c_str(), echo_str.size());
        }
        else if (n == 0)
        {
            lg(Info, "client quit, me too, close fd is : %d", fd);
    
            _epoller_ptr->EpollerUpdate(EPOLL_CTL_DEL, fd, 0); // 移除fd： 本质是从epoll红黑树中移除
            close(fd); // 细节3：从epoll中移除文件描述符的时候，必须保证文件描述符是存在的，不可以先关闭
        }
        else
        {
            lg(Warning, "recv error: fd is : %d", fd);
            _epoller_ptr->EpollerUpdate(EPOLL_CTL_DEL, fd, 0); 
            close(fd);
        }
    }
    void Dispatcher(struct epoll_event revs[], int num)
    {
        for (int i = 0; i < num; ++i)
        {
            uint32_t event = revs[i].events;
            int fd = revs[i].data.fd;
            if (event & EVENT_IN)
            {
                // 写事件就绪
                if (fd == _listensocket_ptr->Fd())
                {
                    // 新连接到来
                    Accepter();
                }
                else
                {
                    // 其他fd上面的普通读事件就绪了
                    Recver(fd);
                }
            }
            else if (event & EVENT_OUT)
            {
                // 读事件就绪
            }
        }
    }
    bool Start()
    {
        // 将listensocket添加到epoll中 -> 将listensock和他关心的事件，添加到内核epoll模型的红黑树中
        _epoller_ptr->EpollerUpdate(EPOLL_CTL_ADD, _listensocket_ptr->Fd(), EVENT_IN);
        struct epoll_event revs[num];
        for (;;)
        {
            int n = _epoller_ptr->EpollerWait(revs, num);
            if (n > 0) // 有事件就绪
            {
                // lg(Debug, "event happend, fd is %d", revs[0].data.fd);
                Dispatcher(revs, n);
            }
            else if (n == 0) // 超时
            {
                lg(Info, "time out...");
            }
            else
            {
                lg(Error, "epoll wait error...");
            }
        }
    }
    ~EpollServer()
    {
        _listensocket_ptr->Close();
    }

private:
    std::shared_ptr<Sock> _listensocket_ptr;
    std::shared_ptr<Epoller> _epoller_ptr;
    uint16_t _port;
};
