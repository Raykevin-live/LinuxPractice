#include "BlockQueue.hpp"
#include "task.hpp"

void *Consumer(void *args){
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task>*>(args);

    while(true){
        // 消费
       Task data = bq->pop();
       // 计算
       data.run();
        std::cout<<"处理任务： " <<data.GetTask()<<"结果是："<<data.GetResult()<<endl;
    }
}

void *Productor(void *args){
    int len = opers.size();
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task>*>(args);
    // int data1 = 0;
    while(true){
        int data1 = rand()%10+1;
        usleep(10);
        int data2 = rand()%10 +1 ;
        char op = opers[rand()%len];
        
        Task t(data1, data2, op);
        bq->push(t);
        cout<<"生产了一个任务："<<t.GetTask()<<" thread id: "<<pthread_self()<<endl;
        sleep(1);
    }
}

int main(){
    // 可以传任务
    srand(time(nullptr));
    BlockQueue<Task> *bq = new BlockQueue<Task>();
    pthread_t c[3], p[5];

    for(int i=0; i<3; i++){
        pthread_create(c+i, nullptr, Consumer, bq);
    }
    for(int i=0; i<5; i++){
        pthread_create(p+i, nullptr, Productor, bq);
    }

    for(int i=0; i<3; i++){
        pthread_join(c[i], nullptr);
    }
    for(int i=0; i<5; i++){
        pthread_join(p[i], nullptr);
    }
    delete bq;
    return 0;
}