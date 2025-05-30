#pragma once
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <vector>

const int defaultcap = 5;

template<class T>
class RingQueue{
private:
    void P(sem_t &sem){
        sem_wait(&sem);
    }
    void V(sem_t &sem){
        sem_post(&sem);
    }
    void Lock(pthread_mutex_t& mutex){
        pthread_mutex_lock(&mutex);    
    }
    void Unlock(pthread_mutex_t& mutex){
        pthread_mutex_unlock(&mutex);    
    }
public:
    RingQueue(int cap = defaultcap):_ringqueue(cap), _cap(cap), c_step(0), p_step(0){
        sem_init(&cdata_sem, 0, 0);
        sem_init(&pspace_sem, 0, _cap);
        pthread_mutex_init(&p_mutex, nullptr);
        pthread_mutex_init(&c_mutex, nullptr);
    }
    void Push(const T &in){ //生产
        P(pspace_sem);
        Lock(p_mutex);// 先拿信号量，后加锁
        _ringqueue[p_step] = in;
        // 后移, 维持环形
        p_step++;
        p_step %= _cap;
        Unlock(p_mutex);

        V(cdata_sem);
    }
    void Pop(T* out){ //消费
        P(cdata_sem);
        Lock(c_mutex);
        *out = _ringqueue[c_step];
        // 后移, 维持环形
        c_step++;
        c_step %= _cap;
        
        Unlock(c_mutex);
        V(pspace_sem);
    }
    ~RingQueue(){
        sem_destroy(&cdata_sem);
        sem_destroy(&pspace_sem);
        pthread_mutex_destroy(&c_mutex);
        pthread_mutex_destroy(&p_mutex);
    }
private:
    std::vector<T> _ringqueue;
    int _cap;
    int c_step; //消费者下标
    int p_step; //生产者下标

    sem_t cdata_sem; //消费者关注的数据资源
    sem_t pspace_sem; //生产者关注的空间资源

    pthread_mutex_t c_mutex;
    pthread_mutex_t p_mutex; 
};