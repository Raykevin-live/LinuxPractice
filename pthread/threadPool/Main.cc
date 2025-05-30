#include <iostream>
#include "ThreadPool.hpp"
#include "Task.hpp"
#include <ctime>

using namespace std;

int main(){
    srand(time(nullptr)^getpid());

    ThreadPool<Task> *tp = new ThreadPool<Task>(5);
    tp->Start();
    int len = opers.size();
    while(true){
        //1.创建任务
        int x = rand()%10 + 1;
        usleep(100);
        int y = rand()%5;
        char op = opers[rand()%len];

        Task t(x, y, op);
        tp->Push(t);
        //2.交给线程池处理
        cout<<"main thread make task: "<<t.GetTask()<<endl;
        sleep(1);
    }
    return 0;
}