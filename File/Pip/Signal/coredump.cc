#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(){
    pid_t id = fork();
    if(id==0){
        //child
        int cnt = 500;
        while(cnt){
            cout<<"I am a child process, pid: "<<getpid()<<" cnt: "<<cnt<<endl;
            sleep(1);
            --cnt;
        }
    }
    // father
    int status = 0;
    pid_t rid = waitpid(id, &status, 0);
    if(rid==id){
        cout<<"child quit info, rid: "<<rid<<" exit_code: "<<((status>>8)&0xFF)
            <<" exit_signal: "<<(status&0x7F)<<" core_dump: "<<((status>>7)&1)<<endl;
    }

    return 0;
}