#pragma once

#include <pthread.h>
#include <queue>
#include <iostream>
#include <unistd.h>
using namespace std;

// 阻塞队列
template<class T>
class BlockQueue{
    static const int defalutnum = 5;
public:
    BlockQueue(int maxcap = defalutnum):_maxcap(maxcap){
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&c_cond, nullptr);
        pthread_cond_init(&p_cond, nullptr);
        low_water = _maxcap/3;
        high_water = (_maxcap*2)/3;
    }
    // 谁来唤醒
    T pop(){
        pthread_mutex_lock(&_mutex);
        if(_q.size()==0){ //因为判断临界资源是否满足，也是在访问临界资源！判断临界资源是否就绪，是通过在临界资源内部判断的
            pthread_cond_wait(&c_cond, &_mutex);
        }
        T out = _q.front();
        _q.pop();
        if(_q.size()<low_water) pthread_cond_signal(&p_cond);
        pthread_mutex_unlock(&_mutex);
        return out;
    }
    void push(const T& in){
        pthread_mutex_lock(&_mutex);
        while(_q.size()==_maxcap){ //需要防止线程被伪唤醒//////！！！！！改if为while
            pthread_cond_wait(&p_cond, &_mutex); //1.调用的时候，自动释放 2.返回时,重新持有锁
        }
        // 1.队列没满 2.被唤醒
        _q.push(in); //并不是象声词就可以生产，必须得保证生产条件的正确
        if(_q.size()>high_water) pthread_cond_signal(&c_cond);
        // pthread_cond_broadcast(&c_cond);

        pthread_mutex_unlock(&_mutex);
    }
    ~BlockQueue(){
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&c_cond);
        pthread_cond_destroy(&p_cond);
    }
private:
    std::queue<T> _q; //共享资源
    // int _mincap;
    int _maxcap; //极值
    pthread_mutex_t _mutex;
    pthread_cond_t c_cond;
    pthread_cond_t p_cond;
    int low_water;
    int high_water;
};