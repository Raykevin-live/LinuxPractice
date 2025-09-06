#pragma once
#include "NoCopy.hpp"
#include "Log.hpp"
#include <sys/epoll.h>
#include <cstring>
class Epoller : public NoCopy
{
    static const int size = 128;
    static const int timeout = -1;
public:
    Epoller()
    {
        _epfd = epoll_create(size);
        if (_epfd == -1)
        {
            lg(Error, "epoll_create error: %s", strerror(errno));
        }
        else
        {
            lg(Info, "epoll_create success: %d", _epfd);
        }
    }
    int EpollerWait(struct epoll_event revents[], int num, int timeout)
    {
        int n = epoll_wait(_epfd, revents, num, timeout);
        return n;
    }
    int EpollerUpdate(int oper, int sock, uint32_t event)
    {
        int n = 0;
        if(oper == EPOLL_CTL_DEL)
        {
            n = epoll_ctl(_epfd, oper, sock, nullptr); //最后直接设为nullptr表示不关心即可
            if(n != 0)
            {
                lg(Error, "epoll_ctl error!");
            }
        }
        else
        {
            // EPOLL_CTL_MOD || EPOLL_CTL_ADD
            struct epoll_event ev;
            ev.events = event;
            ev.data.fd = sock; // 方便我们得知是哪一个fd就绪了
            n = epoll_ctl(_epfd, oper, sock, &ev);
            if(n != 0)
            {
                lg(Error, "epoll_ctl add error");
            }
        }
        return n;
    }
    ~Epoller()
    {
        if(_epfd >= 0)
        {
            close(_epfd);
        }
    }

private:
    int _epfd;
};