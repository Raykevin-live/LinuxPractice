#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <mutex>

using namespace std;

struct ThreadInfo{
    pthread_t tid;
    string name;
};

static const int defaultnum = 5;

template<class T>
class ThreadPool{
public:
    void Lock(){
        pthread_mutex_lock(&mutex_);
    }
    void Unlock(){
        pthread_mutex_unlock(&mutex_);
    }
    void WakeUp(){
        pthread_cond_signal(&cond_);
    }
    void Sleep(){
        pthread_cond_wait(&cond_, &mutex_);
    }
    bool IsQueueEmpty(){
        return tasks_.empty();
    }
    string GetThreadName(pthread_t tid){
        for(const auto& ti : threads_){
            if(ti.tid == tid) return ti.name;
        }
        return "";
    }
public:
    
    static void* HandlerTask(void* args){
        ThreadPool<T>* tp = static_cast<ThreadPool<T>*>(args);
        string name = tp->GetThreadName(pthread_self());
        while(true){
            tp->Lock();
            while(tp->IsQueueEmpty()){
                tp->Sleep();
            }
            T t = tp->tasks_.front();
            tp->tasks_.pop();
            // sleep(1);
            // cout<<"new thread wait task..."<<endl;
            tp->Unlock();
            t();
            // cout<<name<<" run, result: "<<t.GetResult()<<endl;
        }
    }

    void Start(){
        int num = threads_.size();
        for(int i=0; i<num; i++){
            threads_[i].name = "thread-"+to_string(i+1);
            pthread_create(&(threads_[i].tid), nullptr, HandlerTask, this);
        }
    }
    void Push(const T &t){
        Lock();
        tasks_.push(t);
        WakeUp();
        Unlock();
    }
    T Pop(){
        T t = tasks_.front();
        tasks_.pop();
        return t;
    }

    static ThreadPool<T>* GetInstance(){
        if(nullptr == tp_){
            pthread_mutex_lock(&lock_);
            if(nullptr==tp_){
                std::cout<<"log: singleton create done first!"<<std::endl;
                tp_ = new ThreadPool<T>();
            }
        }
        return tp_;
    }
    
private:
    vector<ThreadInfo> threads_;
    queue<T> tasks_;

    pthread_mutex_t mutex_;
    pthread_cond_t cond_;

    static ThreadPool<T> *tp_;
    static pthread_mutex_t lock_;

    ThreadPool(int num = defaultnum):threads_(num){
        pthread_mutex_init(&mutex_, nullptr);
        pthread_cond_init(&cond_, nullptr);
    }

    ThreadPool(const ThreadPool<T>& ) = delete;

    ~ThreadPool(){
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }
};

template <class T>
ThreadPool<T> *ThreadPool<T>::tp_ = nullptr;

template <class T>
pthread_mutex_t ThreadPool<T>::lock_ = PTHREAD_MUTEX_INITIALIZER;