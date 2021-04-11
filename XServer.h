
#ifndef XCOMCLI_XSERVER_H
#define XCOMCLI_XSERVER_H
#include "XTask.h"
#include "XServerHandle.h"
#include "XThreadPool.h"
class XServer: public XTask{
public:
    XServer();
    ~XServer();
    //每次进来一个连接 创建一个对象 加入到线程池
    virtual XServerHandle* createHandle() = 0;

    //服务初始化 由线程池调用
    bool Init();

    //开始运行服务 接收连接任务 加入线程池
    bool Start();

    void listener_cb(int sock,struct sockaddr* addr,int socklen);
    //接收用户连接线程池
    XThreadPool* xThreadListenPool = 0;

    XThreadPool* xThreadClientPool = 0;

};
#endif //XCOMCLI_XSERVER_H
