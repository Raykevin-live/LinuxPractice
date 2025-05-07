#include "comm.hpp"


int main(){
    // 一旦有了共享内存，挂接到自己的地址空间中，你直接把她当成你自己的内存空间来使用即可！
    // 不需要系统调用
    // 也不需要缓冲区
    sleep(3);
    int shmid = CreateShm();
    log(Debug, "create shm done, shmid is %d", shmid);

    sleep(10);

    char* shmaddr = (char*)shmat(shmid, nullptr, 0);
    log(Debug, "attach shm done, shmaddr: 0x%x", shmaddr);
    sleep(3);
    shmdt(shmaddr);
    log(Debug, "detach shm done, shmaddr: 0x%x", shmaddr);

    // ipc code 在这里

    sleep(20);
    shmctl(shmid, IPC_RMID, nullptr);
    log(Debug, "destory shm done");

    sleep(3);
    log(Debug, "process quit...");
    return 0;
}