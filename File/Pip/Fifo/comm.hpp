#pragma once

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>

#define FIFO_FILE "./myfifo"
#define MODE 0664

enum{
    FIFO_CREAT_ERR =1, 
    FIFO_DELETE_ERR , 
    FIFO_OPEN_ERR
};

class Init{
public:
    Init(){
        // 创建信道
        int n = mkfifo(FIFO_FILE, MODE);
        if(n==-1){
            perror("mkfile");
            exit(FIFO_CREAT_ERR);
        }
    }
    ~Init(){
        //关闭信道
        int m = unlink(FIFO_FILE);
        if(m==-1){
            perror("unlink");
            exit(FIFO_DELETE_ERR);
        }
    }
    
};