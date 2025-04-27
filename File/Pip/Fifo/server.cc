#include "comm.hpp"

using namespace std;

//管理管道文件
int main(){
    // 创建信道
    Init init;
    // sleep(5);
    //打开信道
    int fd = open(FIFO_FILE, O_RDONLY); // 等待写入方打开之后，自己才会打开文件，向后执行；反之，打开写端也是一样的 open会被阻塞
    if(fd < 0){
        perror("open");
        exit(FIFO_OPEN_ERR);
    }

    //开始通信
    while(true){
        char buffer[1024] = {0};
        int x = read(fd, buffer, sizeof(buffer));
        if(x > 0){
            buffer[x] = 0;
            cout<<"client say#"<<buffer<<endl;
        }
        else if(x == 0){
            cout<<"client quit, me too"<<endl;
            break;
        }
        else break;
    }

    close(fd);
    return 0;
}