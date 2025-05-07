#include "comm.hpp"


int main(){
    sleep(3);
    int shmid = GetShm();
    log(Debug, "get shm done, shmid is %d", shmid);

    sleep(5);

    char* shmaddr = (char*)shmat(shmid, nullptr, 0);
    log(Debug, "attach shm done, shmaddr: 0x%x", shmaddr);
    

    // ipc code 在这里

    sleep(3);
    shmdt(shmaddr);
    log(Debug, "detach shm done, shmaddr: 0x%x", shmaddr);

    return 0;
}