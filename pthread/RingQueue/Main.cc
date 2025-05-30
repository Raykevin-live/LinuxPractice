#include "RingQueue.hpp"
#include <iostream>
#include <unistd.h>
#include <ctime>
#include "task.hpp"
using namespace std;

struct ThreadData{
    RingQueue<int> *rq;
    string threadname;
};
void *Productor(void *args){
    // sleep(3);
    ThreadData* td = static_cast<ThreadData*>(args);
    RingQueue<int>* rq = td->rq;
    string name = td->threadname;
    while(true){
        int data = rand() % 10 + 1;

        rq->Push(data);
        cout<<"Productor data done, data is: "<<data<<", who: "<<name<<endl; //打印需要加锁
        sleep(1);
    }
    return nullptr;
}
void *Consumer(void *args){
    ThreadData* td = static_cast<ThreadData*>(args);
    RingQueue<int>* rq = td->rq;
    string name = td->threadname;

    while(true){
        // 1.消费数据
        int data = 0;
        rq->Pop(&data);
        cout<<"Consumer get data, data is : "<<data<<", who: "<<name<<endl;
        // sleep(1);

        // 2.处理数据
    }
}
int main(){
    RingQueue<int> *rq = new RingQueue<int>(10);

    pthread_t c[5], p[3];
    for(int i=0; i<3; i++){
        ThreadData *td = new ThreadData();
        td->rq = rq;
        td->threadname = "Productor-"+to_string(i);
        pthread_create(p+i, nullptr, Productor, td);
    }
    for(int i=0; i<5; i++){
        ThreadData *td = new ThreadData();
        td->rq = rq;
        td->threadname = "Consumer-"+to_string(i);
        pthread_create(c+i, nullptr, Consumer, td);
    }

    for(int i=0; i<3; i++){
        pthread_join(p[i], nullptr);
    }
    for(int i=0; i<5; i++){
        pthread_join(c[i], nullptr);
    }
    return 0;
}