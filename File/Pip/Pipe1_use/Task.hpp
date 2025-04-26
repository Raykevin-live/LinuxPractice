#pragma once

#include <iostream>
#include <vector>

typedef void (*_task)();

void task1(){
    std::cout<<" lol 刷新日志 "<<std::endl;
}
void task2(){
    std::cout<<" lol 清理野怪 "<<std::endl;
}
void task3(){
    std::cout<<" lol 收割残血 "<<std::endl;
}
void task4(){
    std::cout<<" lol 推掉水晶 "<<std::endl;
}

void LoadTask(std::vector<_task>* tasks){
    tasks->push_back(task1);
    tasks->push_back(task2);
    tasks->push_back(task3);
    tasks->push_back(task4);
}