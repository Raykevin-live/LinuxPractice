#pragma once

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <cstring>
#include <cstdlib>

#include "log.hpp"
using namespace std;

// 共享内存的大小一般建议为4096的整数倍
// 假如申请4097，实际上申请的是4096*2的大小
const int size = 4097;
const string pathname = "/home/cf";
const int proj_id = 0x66666;
Log log;

key_t GetKey(){
    key_t k = ftok(pathname.c_str(), proj_id);
    if(k<0){
        log(Fatal, "ftok error: %s", strerror(errno));
        exit(1);
    }
    log(Info, "ftok success, key is %d", k);
    return k;
}
int GetshareMemHelper(int flag){
    int key = GetKey();
    int shmid = shmget(key, ::size, flag);
    if(shmid < 0){
        log(Fatal, "create share memory error: %s", strerror(errno));
        exit(2);
    }
    log(Info, "create share memory success, shmid: %d", shmid);
    return shmid;
}

int CreateShm(){
    return GetshareMemHelper(IPC_CREAT|IPC_EXCL|0666);
}

int GetShm(){
    return GetshareMemHelper(IPC_CREAT);
}