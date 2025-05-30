#pragma once
#include <iostream>
#include <pthread.h>
#include <string>
#include <ctime>

typedef void(*callback_t)();
static int num = 1;

class Thread{
public:
    static void* Rountine(void* args){
        Thread* thread = static_cast<Thread*>(args);
        thread->Entry();
        return nullptr;
    }
public:
    Thread(callback_t cb):tid(0),start_timestamp(0),isrunning(false),_cb(cb){
    }
    void Run(){
        name = "thread-" + std::to_string(num++);
        start_timestamp = time(nullptr);
        isrunning = true;
        pthread_create(&tid, nullptr, Rountine, this);
    }
    void Join(){
        pthread_join(tid, nullptr);
        isrunning = false;
    }
    std::string Name(){
        return name;
    }
    uint64_t StartTimeStamp(){
        return start_timestamp;
    }
    bool IsRunning(){
        return isrunning;
    }
    void Entry(){
        _cb();
    }
    ~Thread(){

    }
private:
    pthread_t tid;
    std::string name;
    uint64_t start_timestamp;
    bool isrunning;

    callback_t _cb;
};