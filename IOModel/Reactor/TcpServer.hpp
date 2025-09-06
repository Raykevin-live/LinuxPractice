#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include "Socket.hpp"
#include "NoCopy.hpp"
#include "Log.hpp"
#include "Epoller.hpp"
#include "Comm.hpp"

uint32_t EVENT_IN = (EPOLLIN | EPOLLET);
uint32_t EVENT_OUT = (EPOLLOUT | EPOLLET);

class Connection;
class TcpServer;
using func_t = std::function<void(std::shared_ptr<Connection>)>;

class Connection
{
public:
    Connection(int sock, std::shared_ptr<TcpServer> tcp_ptr)
        : _sock(sock), _tcp_server_ptr(tcp_ptr)
    {
    }
    void SetHandler(func_t recv_cb, func_t send_cb, func_t except_cb)
    {
        _recv_cb = recv_cb;
        _send_cb = send_cb;
        _except_cb = except_cb;
    }
    int SockFd(){return _sock;}
    ~Connection()
    {
    }

private:
    int _sock;
    std::string _inbuffer;  // 输入缓冲区, 无法处理二进制流
    std::string _outbuffer; // 输出缓冲区
public:
    func_t _recv_cb; // callback
    func_t _send_cb;
    func_t _except_cb;

    // 添加一个回指指针
    std::shared_ptr<TcpServer> _tcp_server_ptr;
};

class TcpServer : public NoCopy
{
    static const int num = 64;

public:
    TcpServer(uint16_t port)
        : _port(port), _epoller_ptr(new Epoller), _listensock_ptr(new Sock), _quit(true)
    {
    }
    void Init()
    {
        _listensock_ptr->Socket();
        SetNonBlockOrDie(_listensock_ptr->Fd());
        _listensock_ptr->Bind(_port);
        _listensock_ptr->Listen();
        lg(Info, "create listen socket success, fd: %d", _listensock_ptr->Fd());
        AddConnection(_listensock_ptr->Fd(), EVENT_IN, 
                    std::bind(&TcpServer::Accepter, this, std::placeholders::_1), 
                    nullptr, nullptr);
    }
    void AddConnection(int sock, uint32_t event, func_t recv_cb, func_t send_cb, func_t except_cb)
    {
        // 1. 给listensocket 也建立一个connection对象，将listensock添加到Connection中，
        // 同时 listensock和Connection放入_connections中进行管理
        std::shared_ptr<Connection> new_connection = std::make_shared<Connection>(sock, std::shared_ptr<TcpServer>(this));
        new_connection->SetHandler(recv_cb, send_cb, except_cb);

        // 2. 添加到unordered_map
        _connections.emplace(sock, new_connection);

        // 3. 添加对应的事件，除了要加到内核中，fd、event
        _epoller_ptr->EpollerUpdate(EPOLL_CTL_ADD, sock, event);
    }
    bool IsConnectionSafe(int fd)
    {
        const auto iter = _connections.find(fd);
        if(iter == _connections.end())
        {
            return false;
        }
        return true;
    }
    void Dispatcher(int timeout)
    {
        int n = _epoller_ptr->EpollerWait(revs, num, timeout);
        for (int i = 0; i < n; ++i)
        {
            uint32_t event = revs[i].events;
            int sock = revs[i].data.fd;
            // 统一把事件异常转化为读写问题，直接在读写的地方处理异常即可
             if(event & EPOLLERR)
            {
                // 错误
                event | (EPOLLIN | EPOLLOUT);
            }
            if(event & EPOLLHUP)
            {
                // 挂断
                event | (EPOLLIN | EPOLLOUT);
            }
            if((event & EVENT_IN) && IsConnectionSafe(sock))
            {
                // 读事件就绪
                if(_connections[sock]->_recv_cb)
                {
                    _connections[sock]->_recv_cb(_connections[sock]);
                }
            }
            if((event & EVENT_OUT) && IsConnectionSafe(sock))
            {
                // 写事件就绪
                if(_connections[sock]->_send_cb)
                {
                    _connections[sock]->_send_cb(_connections[sock]);
                }
            }
           
        }
    }
    void Accepter(std::shared_ptr<Connection> conn)
    {
        while(true)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);

            int sock = ::accept(conn->SockFd(), (struct sockaddr*)&peer, &len);
            if(sock > 0)
            {
                AddConnection(sock, EVENT_IN, nullptr, nullptr, nullptr); // TODO:
            }
            else if(sock == 0)
            {
                if(errno == EWOULDBLOCK)
                {
                    break;
                }
                else if(errno == EINTR)
                {
                    continue;
                }
                else 
                {
                    break;
                }
            }
        }
    }
    void Loop()
    {
        _quit = false;

        // AddConnection();

        while (!_quit)
        {
            Dispatcher(3000);
            DebugPrint();
        }
        _quit = true;
    }
    void DebugPrint()
    {
        std::cout<< "_connection fd list: "<<std::endl;
        for(const auto& connection : _connections)
        {
            std::cout<< connection.second->SockFd()<<" ";
        }
        std::cout<<std::endl;
    }
    ~TcpServer()
    {
    }

private:
    std::shared_ptr<Epoller> _epoller_ptr;
    std::shared_ptr<Sock> _listensock_ptr;
    std::unordered_map<int, std::shared_ptr<Connection>> _connections; // 管理Connection
    struct epoll_event revs[num];
    uint16_t _port;
    bool _quit;
};