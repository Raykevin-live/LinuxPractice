#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

// int: 收到了那一个信号
void myhander(int signo){
    cout<<"process get a signal: "<<signo<<endl;
}

int main(){

    int n = alarm(5);
    signal(SIGALRM, myhander);
    while(1){
        cout<<"proc is running ... "<<endl;
        sleep(1);
    }
    // signal(SIGINT, myhander); // 只需设置一次，往后都有效
    // while(true){
    //     cout<<"I am a man\n";
    //     sleep(1);
    // }

    return 0;
}