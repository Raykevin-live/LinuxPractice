#include "Task.hpp"
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <ctime>

#include "Task.hpp"
struct channel;

const int processNum = 5;
std::vector<_task> tasks;
std::vector<channel> channels(processNum); // 进程池 //调用无参构造

//先描述
struct channel{
public:
    channel(int cmdfd, pid_t slaverid, std::string& processname)
    :_cmdfd(cmdfd), _slaverid(slaverid), _processname(processname)
    {}
    channel(){}
public:
    int _cmdfd; // 发送任务的文件描述符
    pid_t _slaverid; // 子进程的pid
    std::string _processname; // 子进程的名字 -- 方便我们打印日志
};

void slaver(){ // 读端
    while(true){
        // std::cout<<getpid()<<" - "<<"read fd is : "<<rfd<<std::endl;
        // sleep(1);
        int cmdcode = 0;
        int n = read(0, &cmdcode, sizeof(int));
        if(n==sizeof(int)){
            std::cout <<"slaver say@ get a command: "<< getpid() << " : cmdcode: " <<  cmdcode << std::endl;
            if(cmdcode >= 0 && cmdcode<tasks.size()) tasks[cmdcode]();
        }
        if(n==0) break;
    }
}

// 输入 const &
// 输出 *
// 输入输出 &
void InitProcessPool(std::vector<channel> *channels){
    std::vector<int> oldfds;
    for(int i=0; i<processNum; ++i){
        int pipefd[2];
        int n = pipe(pipefd);
        assert(!n);
        (void)n;

        pid_t id = fork();
        if(id==0){
            // child
            // 清理之前的多余写端
            for(const auto oldfd: oldfds) {
                close(oldfd);
                std::cout<<oldfd<<" ";
            }
            std::cout<<"\n";
            close(pipefd[1]);
            dup2(pipefd[0], 0); //0->pipefd[0]
            close(pipefd[0]);
            slaver();
            std::cout<<"process : "<<getpid()<<" quit\n";
            exit(0);
        }
        //father
        close(pipefd[0]);
        // 添加 channel字段
        std::string name = "process-" + std::to_string(i);
        (*channels)[i] = channel(pipefd[1], id, name);
        oldfds.push_back(pipefd[1]);
        sleep(1);
    }
}

void Debug(const std::vector<channel>& channels){
    //test
    for(const auto &iter : channels){
        std::cout<<iter._cmdfd<<" "<<iter._slaverid<<" "<<iter._processname<<std::endl;
    }
}

void Menu(){
    std::cout<<"###############################"<<std::endl;
    std::cout<<"#### 1.刷新日志  2.清理野怪 ####"<<std::endl;
    std::cout<<"#### 3.收割残血  4.推掉水晶 ####"<<std::endl;
    std::cout<<"####            0.退出     ####"<<std::endl;
    std::cout<<"###############################"<<std::endl;
}
void CtrlSlaver(std::vector<channel>& channels){
    int which = 0;
    while(true){
        int select = 0;
        Menu();
        std::cout<<"请输入选项：";
        std::cin>>select;

        if(select > 4 || select <= 0) break;
        // 1. 选择任务
        int cmdcode = select -1;

        // 2. 选择进程

        std::cout<<"father say: "<<" cmdcode: "<<cmdcode
        <<" already send to: " <<channels[which]._slaverid<<" process name: "
        <<channels[which]._processname<<std::endl;
        // 3. 发送任务
        write(channels[which]._cmdfd, &cmdcode, sizeof(int));

        which++;
        which %= channels.size();

        sleep(1);
    }
}
void QuitProcess(const std::vector<channel>& channels){

    // for(int i=channels.size()-1; i>=0; i--){
    //     close(channels[i]._cmdfd);
    //     waitpid(channels[i]._slaverid, nullptr, 0);
    // }
    for(const auto& iter: channels) {
        close(iter._cmdfd);
        waitpid(iter._slaverid, nullptr, 0);
    }
    // sleep(5);
    // for(const auto& iter : channels) waitpid(iter._slaverid, nullptr, 0);
    // sleep(5);
}
int main(){ 
    srand(time(nullptr)^getpid()^2025);
    LoadTask(&tasks);
    //再组织
    
    // 1. 初始化
    InitProcessPool(&channels);

    // Debug(channels);
    // 2.开始控制子进程
    CtrlSlaver(channels);
    
    // 3.清理收尾
    QuitProcess(channels);
    // sleep(1000);
    return 0;
}