#include <iostream>
#include <signal.h>
#include <cstring>

using namespace std;

// 问题1：pending位图，什么时候从1->0，执行信号的捕捉方法之前，先清0，再调用
void PrintPending(){
    sigset_t set;
    sigpending(&set);
    for(int signo=31; signo>=1; signo--){
        if(sigismember(&set, signo)) cout<<"1";
        else cout<<"0";
    }
    cout<<"\n";
}

void handler(int signo){
    cout<<"catch a signal, signal number: "<<signo<<endl;
    while(true){
        PrintPending();
        sleep(1);
    }
}

int main(){
    struct sigaction act, oact;
    memset(&act, 0, sizeof(act));
    memset(&oact, 0, sizeof(oact));

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, 3);
    sigaddset(&act.sa_mask, 4);
    
    act.sa_handler = handler;
    sigaction(2, &act, &oact);

    while(true){
        cout<<"I am a process: "<<getpid()<<endl;
        sleep(1);
    }
    return 0;
}