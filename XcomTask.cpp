#include "XcomTask.h"
#include "XRpcTask.h"
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include "global.h"
#include <map>
#include <vector>
#include<arpa/inet.h>
void XcomTask::readMsg(const XMsg *xMsg) {
    std::cout<<"msg = "<<xMsg->size<<" "<<xMsg->data<<std::endl;

}
bool XcomTask::writeBuffer(std::string str,std::string backStr) {
    std::cout<<"thread id = "<<this->thread_id<<"  msg = "<<str<<std::endl;
    memset(this->tempBuf,0,sizeof(this->tempBuf));
    bufferevent_write(this->bev,backStr.c_str(),backStr.size());
}
bool XcomTask::writeMsg(const XMsg *xMsg) {
    if(!xMsg||!xMsg->data||xMsg->size <=0){
        return false;
    }
    //写入消息头
    int headSize = bufferevent_write(this->bev,xMsg,sizeof(XMsgHead));
    if(headSize <=0){
        std::cout<<"headSize write error"<<std::endl;
        return false;
    }
    //写入消息体
    int bodySize = bufferevent_write(this->bev,xMsg->data,xMsg->size);
    if(bodySize <=0){
        std::cout<<"bodySize write error"<<std::endl;
        return false;
    }
    return true;
}
void readcb(struct bufferevent *bev, void *ctx){
    auto task = (XcomTask*)ctx;
    task->readCb();
}

void writecb(struct bufferevent *bev, void *ctx){

}


 void eventcb(struct bufferevent *bev, short what, void *ctx){
    std::cout<<"eventcb go"<<std::endl;
    auto task = (XcomTask*)ctx;
    task->eventCb(what);


}
void XcomTask::readCb() {

    int len = bufferevent_read(this->bev,this->tempBuf,sizeof(this->tempBuf));
    if(len <= 0){
        std::cout<<"thread id = "<<this->thread_id<<"  msg = error"<<std::endl;
        return;
    }
    std::string str = this->tempBuf;
    int re = Tools::sp(str,this->serverList,this,this->bev);
    if(re == 0){
        //接收到客户端传来的服务列表
        std::string backStr = "ok1";
        this->writeBuffer(str,backStr);
        return;
    }
    if(re == 1){
        //接收到客户端的请求参数 转发给对应服务客户端
    }


    std::cout<<"thread id = "<<this->thread_id<<"  msg = "<<str<<std::endl;
    memset(this->tempBuf,0,sizeof(this->tempBuf));
    bufferevent_write(this->bev,"ok1",3);
    return;


    //接收Xmsg 消息 先接收头部消息
    //for 循环 确保边缘触发是能读取到所有数据的
    for(;;){
        if(!this->xMsg.data){
            int headSize = sizeof(XMsgHead);
            char buf[4096] = {0};
            int len = bufferevent_read(this->bev,&this->xMsg,sizeof(headSize));
            if(len <=0) return;
            if(len != headSize){
                std::cout<<"head size error"<<std::endl;
                return;
            }
            //验证消息头的有效性
            if(this->xMsg.size<=0){
                std::cout<<"head size error1"<<std::endl;
                return;
            }
            this->xMsg.data = new char[this->xMsg.size];
        }
        int readSize = this->xMsg.size-this->xMsg.recvied;
        if(readSize<=0){
            delete this->xMsg.data;
            memset(&this->xMsg,0, sizeof(this->xMsg));
            std::cout<<"readSize size error1"<<std::endl;
            return;
        }
        int len = bufferevent_read(this->bev,this->xMsg.data+this->xMsg.recvied,this->xMsg.size);
        if(len<=0){
            std::cout<<"len size error1"<<std::endl;
            return;
        }
        this->xMsg.recvied+=len;
        if(this->xMsg.recvied == this->xMsg.size){
            //读取结束 处理消息
            readMsg(&this->xMsg);
           // std::cout<<"msg = "<<this->xMsg.data<<std::endl;
            delete this->xMsg.data;
            memset(&this->xMsg,0, sizeof(this->xMsg));
           // return;
        }
    }

    //std::cout<<"thread id = "<<this->thread_id<<"   read buf ="<<this->tempBuf<<std::endl;
}
void XcomTask::eventCb(short what) {
    if(what & BEV_EVENT_CONNECTED){
        std::cout<<"fgg"<<std::endl;

    }
    if(what & BEV_EVENT_EOF){
        std::cout<<"EXIT"<<std::endl;
        bufferevent_free(this->bev);
    }

    if(what & BEV_EVENT_ERROR || what & BEV_EVENT_TIMEOUT){
        std::cout<<"EXIT2"<<std::endl;
        bufferevent_free(this->bev);
    }
}
bool XcomTask::Init() {
    this->bev = bufferevent_socket_new(this->base,this->sock,BEV_OPT_CLOSE_ON_FREE);
    if(!this->bev){
        std::cout<<"error bev"<<std::endl;
        return false;
    }
    bufferevent_setcb(this->bev,readcb,writecb,eventcb,this);
    bufferevent_enable(this->bev,EV_READ|EV_WRITE);
    timeval t1 = {60,0};
    bufferevent_set_timeouts(this->bev,&t1,&t1);
   // this->writeBuffer(NULL,this->clientIp);
}
XcomTask::XcomTask() {

}
XcomTask::~XcomTask() {

}
