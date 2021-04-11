#include "XThread.h"
#include <thread>
#include <iostream>
#include <event2/event.h>
#include <sys/types.h>
#include <unistd.h>
#include "XTask.h"
static void NotifyCb(evutil_socket_t sock, short witch, void * ptr){
    XThread* t = (XThread*)ptr;
    //水平触发 只要没要接收完全 就会进来
    t->notify(sock,witch);
}

void XThread::Start() {
    //给每个线程创建读写管道和event_Base
    this->Setup();
    //启动线程
    std::thread sh(&XThread::Main, this);
    //清理 分离线程 断开与主线程的联系 独立运行
    sh.detach();
}
void XThread::Active() {
    int len = write(this->notify_send_fd,"c",1);
    if(len<=0){
        std::cout<<this->id<<"active fail"<<std::endl;
        return;
    }
}
void XThread::Main() {
    std::cout<<this->id<<"线程启动"<<std::endl;
    while (!this->is_exit){
        //一次处理多条消息
        event_base_loop(this->base,EVLOOP_NONBLOCK);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    //这个函数有时会收不到消息 换成另一个 非阻塞的
   // event_base_dispatch(this->base);
    event_base_free(this->base);
}
bool XThread::Setup() {
    int fds[2];
    int res = pipe(fds);
    if(res<0){
        std::cout<<"pipe fail"<<this->id<<std::endl;
        return false;
    }
    //读取fd[0] 绑定event事件中 写入fd[1]保存
    this->notify_send_fd = fds[1];
    //创建libevent上下文(无锁)
    event_config* config = event_config_new();
    event_config_set_flag(config,EVENT_BASE_FLAG_NOLOCK);
    this->base = event_base_new_with_config(config);
    event_config_free(config);
    if(!this->base){
        std::cout<<"BASE FAIL"<<std::endl;
        event_base_free(this->base);
        return false;
    }
    // 添加管道监听事件 用于激活线程执行任务
    event* ev = event_new(this->base, fds[0], EV_READ | EV_PERSIST, NotifyCb, this);
    event_add(ev,0);
    return true;
}
void XThread::notify(int sock, short witch) {
    //水平触发 只要没要接收完全 就会进来
    char buf[2] = {0};
    int len = read(sock,buf,1);
    if(len<=0){
        return;
    }
    std::cout<<this->id<<"=="<<buf<<std::endl;

    //获取任务 并初始化任务
    XTask* task = NULL;
    this->tasks_mutex.lock();
    if(this->tasks.empty()){
        this->tasks_mutex.unlock();
        return;
    }
    task = tasks.front();//先进先出
    tasks.pop_front();//再删除此task 从列表里
    task->thread_id = this->id;
    this->tasks_mutex.unlock();
    task->Init();
}
void XThread::addTask(XTask *task) {
    if(!task){
        return;
    }
    task->base = this->base;
    this->tasks_mutex.lock();
    this->tasks.push_back(task);

    //添加到微服务临时列表
    this->tasks_mutex.unlock();
}
XThread::XThread() {

}
XThread::~XThread() {

}
