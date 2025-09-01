// #include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cerrno>
#include <cstring>

using namespace std;

void SetNonBlock(int fd){
    int fl = fcntl(fd, F_GETFL);
    if(fl < 0){
        cerr<<"fcntl"<<endl;
        return;
    }
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
    cout<<"Set "<<fd<< " to nonblock"<<endl;
}
int main(){
    char buffer[1024];
    SetNonBlock(0);
    sleep(1);
    while(true){
        // cout<<"Please Enter# ";
        // fflush(stdout);
        ssize_t n = read(0, buffer, sizeof(buffer)-1);
        if(n>0){
            buffer[n-1] = 0;
            cout<<"echo : "<<buffer<<endl;
        }
        else if(n == 0){
            cout<<"read done"<<endl;
            break;
        }
        else{
            // 1. 设置为非阻塞轮询，如果底层fd数据没有就绪，recv/read/write/send，返回值会以出错码的形式返回
            // 2. 所以可能是真的出错，也可能是底层数未就绪
            // 3. 需要通过errno去区分
            if(errno == EWOULDBLOCK) // errno == 11
            {
                cout<<"0 fd data not ready, try again!"<<endl;
            }
            else{
                cerr<<"read error, n = "<< n 
                << "errno code: "<<errno<< ", errro str: "<<strerror(errno)<<endl;
            }
            
            break;
            sleep(1);
        }
    }
    return 0;
}