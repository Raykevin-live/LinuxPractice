#pragma once

#include "Socket.hpp"
#include "Log.hpp"
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

constexpr int defaultport = 8080;
class HttpServer;
const std::string wwwroot = "./wwwroot"; //web根目录 
const std::string homepage = "index.html";
const std::string sep = "\r\n";

struct ThreadData{
    int sockfd;
    HttpServer* svr;
    ThreadData(int fd, HttpServer* h):sockfd(fd), svr(h){

    }
};


class HttpRequest{
public:
    void Deserialize(std::string req){
        while(true){
            std::size_t pos = req.find(sep);
            if(pos == std::string::npos) break;
            std::string temp = req.substr(0, pos);
            if(temp.empty()) break;
            req_header.push_back(temp);
            req.erase(0, pos+sep.size()); // 删除处理过的字符
        }
        text = req;
    }
    void Prase(){
        std::stringstream ss(req_header[0]);
        ss>>method>>url>>http_version;

        url_path = wwwroot;
        if(url == "/" || url == "/index.html") {
            url_path += "/";
            url_path += homepage;
        }
        else{
            url_path += url;
        }
        auto pos = url.rfind(".");
        if(pos == std::string::npos) suffix = ".html";
        else{
            suffix = url.substr(pos);
        }
    }
    void DebugPrint(){
        std::cout<<"----------------HttpRequest----------------\n";
        for(auto &line : req_header){
            std::cout<<line<<"\n\n";
        }
        std::cout<<"text: "<<text<<std::endl;
        std::cout<<"----------------HttpRequest End----------------\n";

        std::cout<<"----------------HttpRequest Prase----------------\n";
        std::cout<<"method: "<<method<<std::endl;
        std::cout<<"url: "<<url<<std::endl;
        std::cout<<"http_version: "<<http_version<<std::endl;
        std::cout<<"url_path: "<<url_path<<std::endl;
        std::cout<<"suffix: "<<suffix<<std::endl;

        std::cout<<"----------------HttpRequest Prase End----------------\n";
    }
public:
    std::vector<std::string> req_header;
    std::string text;

    // 解析字段
    std::string method;
    std::string url;
    std::string http_version;

    // url_path
    std::string url_path;
    std::string suffix;
};

class HttpServer{
public:
    HttpServer(int port = defaultport):port_(port)
    {
        content_type.insert({".html", "text/html"});
        content_type.insert({".png", "image/png"});
    }   

    bool Start(){
        listensock_.Socket();
        listensock_.Bind(port_);
        listensock_.Listen();
        for(;;){
            std::string clientip;
            uint16_t clientport;
            int sockfd = listensock_.Accept(&clientip, &clientport);
            if(sockfd < 0) continue;
            lg(Info, "get a new link, sockfd: %d", sockfd);
            pthread_t pid;
            ThreadData* td = new ThreadData(sockfd, this);
            // td->sockfd = sockfd;
            pthread_create(&pid, nullptr, ThreadRun, td);

        }
    }
    static std::string ReadHtmlContent(const std::string& path){
        std::ifstream in(path, std::ios::binary); 
        if(!in.is_open()) return "404";

        /* 要使用二进制读取，处理图片等格式 */
        // 获取长度
        in.seekg(0, std::ios_base::end);
        auto len = in.tellg();
        in.seekg(0, std::ios_base::beg);

        std::string content;
        content.resize(len);
        // in.read((char*)content.c_str(), len);
        // std::string line;
        // while(std::getline(in, line)){ // DONE: 直接按照字符读取的话，无法处理图片等二进制形式的文件
        //     content += line;
        // }
        in.close();
        return content;
    }
    std::string SuffixToDesc(const std::string &suffix){
        auto iter = content_type.find(suffix);
        if(iter==content_type.end()) return content_type[".html"];
        return iter->second;
    }
    void HandlerHttp(int sockfd){
        char buffer[1024];
        ssize_t n = recv(sockfd, buffer, sizeof(buffer)-1, 0);
        if(n > 0){
            buffer[n] = 0;
            std::cout<<buffer;

            // 反序列化响应
            HttpRequest req;
            req.Deserialize(buffer);
            req.Prase();
            req.DebugPrint();

            // 返回响应的过程
            // std::string text = "<html><body><h3>hello world</h3></body></html>";
            // 使用动态html文件载入
            // std::string path = wwwroot + "index.html";
            std::string path = req.url_path;
            std::string text = ReadHtmlContent(path); //读取文件

            std::string response_line = "HTTP/1.0 200 OK\r\n";
            std::string response_header = "Content-Length: ";
            response_header += std::to_string(text.size());
            response_header += "\r\n";
            response_header += "Content-Type: ";
            response_header += SuffixToDesc(req.suffix);
            response_header += "\r\n";
            response_header += "Set-Cookie: name=hhh";
            response_header += "\r\n";

            std::string blank_line = "\r\n";

            std::string response = response_line;
            response += response_header;
            response += blank_line;
            response += text;

            
            send(sockfd, response.c_str(), response.size(), 0);
        }
    }
    static void* ThreadRun(void* args){
        pthread_detach(pthread_self());// 不接收错误码

        ThreadData* td = static_cast<ThreadData*>(args);

        td->svr->HandlerHttp(td->sockfd);
        close(td->sockfd);
        delete td;
        return nullptr;
    }
    ~HttpServer(){}
private:
    Sock listensock_;
    uint16_t port_;
    std::unordered_map<std::string, std::string> content_type;
};