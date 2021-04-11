#include "XThreadPool.h"
#include "XThread.h"
#include "XTask.h"
#include <iostream>
#include <mutex>
#include <thread>
static bool is_exit_all = false;
static std::vector<XThread*> all_threads;
static std::mutex all_threads_mutex;
class CXThreadPool: public XThreadPool{
public:
    void Init(int threadCount) {
        this->threadCount = threadCount;
        this->lastThread = -1;
        for(int i = 0;i<this->threadCount;i++){
            XThread* t = new XThread();
            t->id = i+1;

            //std::cout<<i<<" = "<<t<<std::endl;
            t->Start();
            // XThread* t1 = (XThread*)malloc(sizeof(XThread));
            this->threads.push_back(t);

            all_threads_mutex.lock();
            all_threads.push_back(t);
            all_threads_mutex.unlock();
            //this_thread::sleep_for(chrono::seconds(1));
        }
    }
    void Dispatch(XTask *task) {
        if(!task){
            return;
        }
        int tid = (this->lastThread+1)%this->threadCount;
        lastThread = tid;
        XThread* thread = this->threads[tid];
        //线程添加任务
        thread->id = tid;

        thread->addTask(task);
        //激活线程
        thread->Active();
    }

private:
    int threadCount = 0;
    int lastThread = -1;
    //线程池
    std::vector<XThread*> threads;
};
void XThreadPool::Wait() {
    while (!is_exit_all){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
void XThreadPool::exitAllThread() {
    is_exit_all = true;
    all_threads_mutex.lock();
    for(auto t:all_threads){
        t->Exit();
    }
    all_threads_mutex.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
void XThreadPool::Dispatch(XTask *task) {
    if(!task){
        return;
    }
    this->poolLock.lock();
    int tid = (this->lastThread+1)%this->threadCount;
    lastThread = tid;
    this->poolLock.unlock();
    XThread* thread = this->threads[tid];
    //线程添加任务
    thread->id = tid;

    thread->addTask(task);
    //激活线程
    thread->Active();
}
void XThreadPool::Init(int threadCount) {
    this->threadCount = threadCount;
    this->lastThread = -1;
    for(int i = 0;i<this->threadCount;i++){
        XThread* t = new XThread();
        t->id = i+1;

        //std::cout<<i<<" = "<<t<<std::endl;
        t->Start();
        // XThread* t1 = (XThread*)malloc(sizeof(XThread));
        this->threads.push_back(t);
        //this_thread::sleep_for(chrono::seconds(1));
    }
}
XThreadPool* XThreadPoolFactory::create() {
    return new CXThreadPool();
}
XThreadPool::XThreadPool() {

}
XThreadPool::~XThreadPool() {

}
