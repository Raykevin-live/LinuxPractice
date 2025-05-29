#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
using namespace std;

class Mutex{
public:
    Mutex(pthread_mutex_t* lock){
        this->_lock = lock;
    }
    void Lock(){
        pthread_mutex_lock(_lock);
    }

    void Unlock(){
        pthread_mutex_unlock(_lock);
    }
    ~Mutex(){
    }
private:
    pthread_mutex_t *_lock;
};

class LockGuard{
public:
    LockGuard(pthread_mutex_t* lock):_mutex(Mutex(lock)){
        _mutex.Lock();
    }
    ~LockGuard(){
        _mutex.Unlock();
    }
private:
    Mutex _mutex;
};