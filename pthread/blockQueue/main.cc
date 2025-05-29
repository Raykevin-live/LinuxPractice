#include "BlockQueue.hpp"
#include "task.hpp"

void *Consumer(void *args){
    BlockQueue<int> *bq = static_cast<BlockQueue<int>*>(args);

    while(true){
        // 消费
       int data = bq->pop();
        std::cout<<"消费了一个数据： "<<data<<endl;
    }
}

void *Productor(void *args){
    BlockQueue<int> *bq = static_cast<BlockQueue<int>*>(args);
    int data = 0;
    while(true){
        sleep(1);
        data++;
        bq->push(data);
        cout<<"生产了一个数据："<<data<<endl;
    }
}

int main(){
    // 可以传任务
    BlockQueue<int> *bq = new BlockQueue<int>();
    pthread_t c, p;
    pthread_create(&c, nullptr, Consumer, bq);
    pthread_create(&p, nullptr, Productor, bq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);
    delete bq;
    return 0;
}