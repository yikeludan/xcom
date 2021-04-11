#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include "XServer.h"
#include "XServerHandle.h"
static void listener_cb1(struct evconnlistener *e,
                         evutil_socket_t s,
                         struct sockaddr *a,
                         int socklen,
                         void *arg){

    auto task = (XServer*)arg;
    task->listener_cb(s,a,socklen);
}
void XServer::listener_cb(int sock, struct sockaddr *addr, int socklen) {
    //创建客户端任务对象

    auto handle = createHandle();

}
bool XServer::Init() {
    //监听端口
    sockaddr_in sin;
    memset(&sin,0,sizeof(sin));
    //设置回调函数
    sin.sin_family  = AF_INET;
    sin.sin_port = htons(7777);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    evconnlistener* ev =  evconnlistener_new_bind(this->base,
                                                  listener_cb1,//回调函数
                                                  this,//回调函数的参数
                                                  LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,//地址重用(一个socket 可以反复使用)，listener 关闭的时候同时清理socket
                                                  10,//连接lister 队列大小  缓冲队列没有处理的socket
                                                  (sockaddr*)&sin,
                                                  sizeof(sin));
    if(!ev){
        std::cout<<"ser ev1  error"<<ev<<std::endl;
        return false;
    }
    std::cout<<"ser ev1  suc"<<ev<<std::endl;
    return true;
}
bool XServer::Start() {
    this->xThreadListenPool->Init(1);
    this->xThreadClientPool->Init(6);
    this->xThreadListenPool->Dispatch(this);
    return true;
}
XServer::XServer() {
    this->xThreadClientPool = XThreadPoolFactory::create();
    this->xThreadListenPool = XThreadPoolFactory::create();
}
XServer::~XServer() {
    delete this->xThreadListenPool;
    this->xThreadListenPool = NULL;
    delete this->xThreadClientPool;
    this->xThreadClientPool = NULL;

}
