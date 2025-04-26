#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstring>

#define N 2
#define NUM 1024

void Writer(pid_t fd){
    std::string s = "hello, I am child";
    pid_t self = getpid();
    int number = 0;
    
    char buffer[NUM];
    while(true){
        sleep(1);
        // 构建发送字符串
        buffer[0] = '\0'; // 字符串清空
        snprintf(buffer, sizeof(buffer), "%s-%d-%d", s.c_str(), self, number++);
        // std::cout<<buffer<<std::endl;
        // 发送/写给父进程
        write(fd, buffer, strlen(buffer));
        // sleep(1);
        // char ch = 'c';
        // write(fd, &ch, 1);
        // number ++;
        // std::cout<<number<<std::endl;
    }
}
void Reader(pid_t fd){
    char buffer[NUM];
    int cnt = 0;
    while(true){
        buffer[0] = '\0';
        ssize_t n = read(fd, buffer, sizeof(buffer)); // sizeof != strlen
        if(n > 0){
            buffer[n] = '\0'; // 0 == '\0'
            std::cout<<"father get a message["<<getpid()<<"]#"<<buffer<<std::endl;
            // sleep(5);
        }
        else if(n ==0 ){ 
            std::cout<<"father read file done...\n";
            break;
        }
        else break;

        cnt++;
        if(cnt>5) break;
    }
}
int main(){
    int pipefd[N];
    int n = pipe(pipefd);
    if(n < 0) return 1;
    // std::cout<<"pipfd[0]: "<<pipfd[0]<<", pipfd[1]: "<<pipfd[1]<<std::endl;

    //child -> w; father -> r
    pid_t id = fork();
    if(id < 0) return 2;
    if(id==0){
        // child
        close(pipefd[0]);
        // IPC code
        Writer(pipefd[1]);

        close(pipefd[1]);
        exit(0);
    }
    //father
    close(pipefd[1]);
    // IPC code
    Reader(pipefd[0]);// 读5s
    close(pipefd[0]);
    std::cout<<"father read file fd: "<<pipefd[0]<<std::endl;
    sleep(5); //观察僵尸
    int status = 0;
    pid_t rid = waitpid(id, &status, 0);
    if(rid < 0) return 3;
    
    std::cout<<"wait child success: "<<rid<<" exit_code: "<< ((status>>8)&0xFF) << " exit_signal: "<<(status&0x7F)<<std::endl;
    sleep(5);
    std::cout<<"father quit\n";
    return 0;
}