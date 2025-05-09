#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

void PrintPending(sigset_t &pending){
    for(int signo = 31; signo>=1; signo--){
        if(sigismember(&pending, signo)){
            cout<<"1";
        }
        else{
            cout<<"0";
        }
    }
    cout<<endl;
}

void Handler(int signo){
    cout<<"catch a signo: "<<signo<<endl;
}
int main(){
    // 0. 对2号信号进行自定义捕捉
    signal(2, Handler);
    // 1. 先对2号信号进行屏蔽  
    // 1.1 准备数据
    sigset_t bset, oset;
    sigemptyset(&bset);
    sigemptyset(&oset);
    sigaddset(&bset, 2); //这里已经把信号2屏蔽了吗？no
    // 1.2 系统调用，将数据设置进内核
    sigprocmask(SIG_SETMASK, &bset, &oset); //这里已经把信号2屏蔽了吗？yes

    // 2. 重复打印当前进程的pending 0000000000000000000000000
    sigset_t pending;
    int cnt = 0;
    while(true){
        // 2.1 获取
        int n = sigpending(&pending);
        if(n<0)
            continue;
        // 2.2 打印
        PrintPending(pending);
        sleep(1);
        ++cnt;
        // 2.3 解除阻塞
        if(cnt==20){
            cout<<"unblock 2 signo"<<endl;
            sigprocmask(SIG_SETMASK, &oset, nullptr);
            // 如果不进行处理，解除阻塞后进程会退出，是因为2号信号被递达，
            // 它的默认处理就是quit
        }
    }
    // 3. 发送2号 0000000000000000000000010

    // 4. 可以将所有的信号都进行屏蔽，信号不就不会被处理了吗？

    return 0;
}