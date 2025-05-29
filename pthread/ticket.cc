#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include "LockGuard.hpp"

using namespace std;

#define NUM 4

class threadData{
public:
    threadData(int number, pthread_mutex_t* lock){
        threadname = "thread" + to_string(number);
        this->lock = lock;
    }
public:
    string threadname;
    pthread_mutex_t *lock;
};

int tickets = 1000; //多线程模拟一轮抢票
void* getTicket(void* args){
    threadData* td = static_cast<threadData*>(args);
    const char *name = td->threadname.c_str();
    
    while(true){
        // pthread_mutex_lock(td->lock);// 加锁
        LockGuard lockguard(td->lock); // 使用RAII的锁
        if(tickets > 0){
            usleep(1000);
            printf("%s , get a ticket: %d\n", name, tickets);
            --tickets;
            // pthread_mutex_unlock(td->lock);//解锁
        }
        else{
            // pthread_mutex_unlock(td->lock);//解锁
            break;
        }
    }
    printf("%s ... quit\n", name);
    return nullptr;
}

int main(){
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, nullptr);

    vector<pthread_t> tids;
    vector<threadData*> thread_datas;
    for(int i=1; i<=NUM; i++){
        pthread_t tid;
        threadData* td = new threadData(i, &lock);
        thread_datas.push_back(td);
        pthread_create(&tid, nullptr, getTicket, thread_datas[i-1]);
        tids.push_back(tid);
    }

    for(auto& thread : tids){
        pthread_join(thread, nullptr);
    }
    return 0;
}