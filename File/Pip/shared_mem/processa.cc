#include "comm.hpp"


int main(){
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
    // 一旦有人把数据写入共享内存，其实我们立马就能看到了！！
    // 不需要经过系统调用
    struct shmid_ds shmds;
    while(true){
        cout<<"client say@ "<<shmaddr<<endl;
        sleep(1);
        
        shmctl(shmid, IPC_STAT, &shmds);
        cout<<"shm size: "<<shmds.shm_segsz<<endl;
        cout<<"shm nattch: "<<shmds.shm_nattch<<endl;
        printf("key: 0x%x", shmds.shm_perm.__key);
        cout<<"shm mode: "<<shmds.shm_perm.mode<<endl;
    }

    sleep(20);
    shmctl(shmid, IPC_RMID, nullptr);
    log(Debug, "destory shm done");

    sleep(3);
    log(Debug, "process quit...");
    return 0;
}