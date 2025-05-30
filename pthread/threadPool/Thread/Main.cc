#include "Thread.hpp"
#include <unistd.h>
#include <vector>
void Print(){
    while(true){
        printf("haha, 我是一个封装线程...\n");
        sleep(1);
    }
}

int main(){
    // Thread t(Print);
    // t.Run();
    // t.Join();
    std::vector<Thread> threads;

    for(int i=0; i<10; i++){
        threads.push_back(Thread(Print));
    }

    for(auto& t: threads){
        t.Run();
    }

    for(auto& t : threads){
        t.Join();
    }
    return 0;
}