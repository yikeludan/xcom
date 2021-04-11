#include "XServerTask.h"
#include <iostream>
#include <event2/event.h>
#include <event2/listener.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
static void listener_cb1(struct evconnlistener *e,
                  evutil_socket_t s,
                  struct sockaddr *a,
                  int socklen,
                  void *arg){
   // std::cout<<"listen cb"<<std::endl;

    char ip[16]={0};
    //sockaddr_in cli;
   // socklen_t  len = sizeof(cli);
    //evutil_socket_t client = accept(s,(sockaddr*)&cli,&len);
    sockaddr_in* sin = (sockaddr_in*)a;
    evutil_inet_ntop(AF_INET,&sin->sin_addr,ip,sizeof(ip));
    std::cout<<"client ip = "<<ip<<std::endl;
    std::cout<<"client port = "<<sin->sin_port<<std::endl;

    auto task = (XServerTask*)arg;
    task->clientIp = ip;
    task->clientPort = sin->sin_port;
    if(task->listenCb){
        task->listenCb(s,a,socklen,arg);
    } else{
        std::cout<<"listenCb error"<<std::endl;
    }
}
bool XServerTask::Init() {
    //监听端口
    sockaddr_in sin;
    memset(&sin,0,sizeof(sin));
    //设置回调函数
    sin.sin_family  = AF_INET;
    sin.sin_port = htons(this->serverPort);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    evconnlistener* ev =  evconnlistener_new_bind(this->base,
                                                  listener_cb1,//回调函数
                                                  this,//回调函数的参数
                                                  LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,//地址重用(一个socket 可以反复使用)，listener 关闭的时候同时清理socket
                                                  10,//连接lister 队列大小  缓冲队列没有处理的socket
                                                  (sockaddr*)&sin,
                                                  sizeof(sin));


    std::cout<<"ser ev1  suc"<<ev<<std::endl;
    /*if(base){
        event_base_dispatch(base);
    }
    if(ev){
        evconnlistener_free(ev);
    }
    if(base){
        event_base_free(base);
    }*/
    return true;

}
XServerTask::XServerTask() {

}
XServerTask::~XServerTask() {

}

