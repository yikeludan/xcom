#include "global.h"
#include "XcomTask.h"
#include <iostream>
std::mutex global_mutex;
std::map<std::string,std::vector<std::string>> ipMap;
std::map<std::string,std::vector<std::string>> clientNameMap;
std::map<std::string,bufferevent*> bevMap;
uint32_t isFirstSerMap = 0;
int Tools::sp(const std::string &s, std::vector<std::string> &list,void* ptr,struct bufferevent* bev) {

    if(!ipMap.empty()){
        isFirstSerMap  = 1;//第一次进来 同步 注册服务
    }
    //auto task = (XcomTask*) ptr;
    int re = s.find("|");
    int re1 = s.find("%");
    int re2 = s.find("&");
    if(re<0){
        //开始发送正式数据 获取orderName;


    }
    std::string head = s.substr(0,re);
    std::string name = s.substr(re+1,9);
    std::string package = s.substr(re2+1);
    const char* p = package.c_str();
    int pos = 0;
    char src[200] = {0};
    while (*p !='\0'){
        if(*p == ';'){
            src[pos] = '\0';
            std::string sa(src);
            list.push_back(sa);
            memset(src,0,strlen(src)+1);
            p+=1;
            pos = 0;
        }
        strncpy(src+pos,p,1);
        p+=1;
        pos+=1;
    }
    global_mutex.lock();
    head = head+"-"+name;
    ipMap.insert(std::pair<std::string, std::vector<std::string>>(head, list));
    clientNameMap.insert(std::pair<std::string, std::vector<std::string>>(name, list));
    bevMap.insert(std::pair<std::string,bufferevent* >(name, bev));
    global_mutex.unlock();
    if(bevMap.size()>=2){
        auto ptr = (bufferevent *)bevMap["coupoName"];
        std::cout<<"ptr = "<<ptr<<std::endl;
        bufferevent_write(ptr,"ok2",3);
    }
  /*  for (auto &kv : bevMap) {

        std::cout<<"kv1 = "<<kv.first<<std::endl;
    }*/

   return 0;

}