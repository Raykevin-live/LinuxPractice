#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
using namespace std;

std::string toHex(pthread_t tid){
    char hex[64];
    snprintf(hex, sizeof(hex), "%p", tid);
    return hex;
}
void *threadRoutine(void* args){
    while(true){
        cout<<"thread id: "<<toHex(pthread_self())<<endl;
        sleep(1);
    }
}
int main(){
    pthread_t tid;
    pthread_create(&tid, nullptr, threadRoutine, (void*)"thread 1");
    cout<<"main thread create thread done, new thread id: "<<toHex(tid)<<endl;
    pthread_join(tid, nullptr);

    return 0;
}
// class Request{
// public:
//     Request(int start, int end, const string& threadname)
//         :_start(start), _end(end), _threadname(threadname)
//         {}
// public:
//     int _start;
//     int _end;
//     string _threadname;
// };
// class Response{
// public:
//     Response(int result, int exitcode):_result(result), _exitcode(exitcode)
//         {}
// public:
//     int _result; //计算结果
//     int _exitcode; //计算结果是否可靠

// };
// void *sumCount(void *args){ //线程的参数和返回值不仅仅可以用来传递一般参数，也可以用来传递对象
//     Request *rq = static_cast<Request*>(args);
//     Response *rsq = new Response(0, 0);
//     for(int i=rq->_start; i<rq->_end; i++){
//         rsq->_result += i;
//     }
//     delete rq;
//     return rsq;
// }
// int main(){
//     pthread_t tid;
//     Request* rq = new Request(1,100, "thread 1");
//     pthread_create(&tid, nullptr, sumCount, rq);

//     void* ret;
//     pthread_join(tid, &ret);
//     Response *rsp = static_cast<Response*>(ret);
//     cout<<"rsp->result: "<<rsp->_result<<", exitcode: "<<rsp->_exitcode<<endl;
//     delete rsp;
//     return 0;
// }
// 可以同时调度函数
// void show(const string& name){
//     cout<<name<<"say#"<<" hello world"<<endl;
// }
// int g_val = 1;

// void* threadRoutine(void* args){
//     // while(true){
//     //     cout<<"new thread, pid: "<<getpid()<<endl;
//     //     show("[new thread]");
//     //     sleep(1);
//     // }
//     const char* name = (const char*)args;
//     int cnt = 5;
//     while(true){
//         printf("%s, pid: %d, g_val: %d, &g_val: %p\n", name, getpid(), g_val, &g_val);
//         cnt--;
//         sleep(1);
//         if(cnt==0) break;
//     }
//     pthread_exit((void*)1);
//     // return (void*)1; 
//     // exit(11);
// }
// int main(){
//     pthread_t tid;
//     pthread_create(&tid, nullptr, threadRoutine, (void*)"Thread 1");

//     sleep(1);
//     pthread_cancel(tid);
//     // return 0;

//     // while(true){
//     //     cout<<"main thread, pid: "<<getpid()<<endl;
//     //     show("[main thread]");
//     //     sleep(1);
//     // }
//     void* retval;
//     pthread_join(tid, &retval);

//     cout<<"main thread quit ..., ret: "<<(long long int)retval<<endl;

//     return 0;
// }