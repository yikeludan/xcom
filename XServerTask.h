//
// Created by 82017 on 2021-03-16.
//

#ifndef XCOM_XSERVERTASK_H
#define XCOM_XSERVERTASK_H
#include "XTask.h"

typedef void(*ListenCbFun)(int sock,struct sockaddr* addr,int socklen,void* ptr);
class XServerTask: public XTask{
public:
    virtual bool Init();
    ListenCbFun listenCb = 0;
    XServerTask();
    ~XServerTask();
    char* clientIp = 0;
    int clientPort = 0;
    int serverPort;
};
#endif //XCOM_XSERVERTASK_H
