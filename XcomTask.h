
#ifndef XCOM_XCOMTASK_H
#define XCOM_XCOMTASK_H
#include "XTask.h"
#include "XMsg.h"
#include <string>
#include <vector>
#include <iostream>
class XcomTask: public XTask{
public:
    virtual bool Init();
    virtual void eventCb(short what);
    virtual void readCb();
    virtual void readMsg(const XMsg* xMsg);
    virtual bool writeMsg(const XMsg* xMsg);
    bool writeBuffer(std::string str,std::string backStr);
    XcomTask();
    ~XcomTask();
    struct bufferevent* bev = 0;
    std::string  ip = "";
    int port =0;
    char tempBuf[4096] = {0};
    char readBuf[4096] = {0};
    char* clientIp = 0;
    int clientPort = 0;
    std::vector<std::string> serverList;
    //数据包缓存
    XMsg xMsg;
};
#endif //XCOM_XCOMTASK_H
