#ifndef XCOM_XSERVICEPROXY_H
#define XCOM_XSERVICEPROXY_H
#include <map>
#include <vector>
#include <string>
#include "XMsg.h"
#include "XRpcTask.h"

class XServiceProxy{
public:
    XServiceProxy();
    ~XServiceProxy();
    //初始化微服务列表
    bool Init();

    //初始化微服务列表
    std::map<std::string,std::vector<XMicoService>> micoMap;
    //std::map<std::string,std::vector<XcomTask>> tempmicoMap;
};
#endif //XCOM_XSERVICEPROXY_H
