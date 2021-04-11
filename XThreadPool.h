#ifndef XCOM_XTHREADPOOL_H
#define XCOM_XTHREADPOOL_H
#include <event2/event.h>
#include <list>
#include <vector>
#include <mutex>
class XThread;
class XTask;
class XThreadPool{
public:
    static XThreadPool* Get(){
      /*  static XThreadPool p;
        return &p;*/
    }
    //初始化所有线程并启动
    virtual void Init(int threadCount) = 0;

    //分发线程
    virtual void Dispatch(XTask* task) = 0;

    //退出所有线程
    static void exitAllThread();

    static void Wait();

    XThreadPool();
    ~XThreadPool();

private:
    int threadCount = 0;
    int lastThread = -1;
    std::mutex poolLock;
    //线程池
    std::vector<XThread*> threads;
};
class XThreadPoolFactory{
public:
    static XThreadPool* create();
};
#endif //XCOM_XTHREADPOOL_H
