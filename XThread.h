#ifndef XCOM_XTHREAD_H
#define XCOM_XTHREAD_H
#include <event2/event.h>
#include <list>
#include <mutex>
class XTask;
class XThread{
public:
    void Start();
    //线程入口函数
    void Main();

    //安装线程和初始化libevent和管道监听
    bool Setup();

    void notify(evutil_socket_t sock, short witch);
    //添加任务  一个线程同时处理多个任务 共用一个event_base
    void addTask(XTask* task);
    //线程激活
    void Active();
    XThread();
    ~XThread();
    //线程编号
    int id;
    int notify_send_fd = 0;
    struct event_base* base = 0;
    unsigned int isBusy = 0;
    bool is_exit = false;
    //任务列表
    std::list<XTask*> tasks;
    //线程安全互斥
    std::mutex tasks_mutex;

    void Exit(){
        is_exit = true;
    }
};
#endif //XCOM_XTHREAD_H
