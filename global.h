//
// Created by 82017 on 2021-03-18.
//
#ifndef XCOM_GLOBAL_H
#define XCOM_GLOBAL_H
#include <mutex>
#include <vector>
#include <string>
#include <map>
#include <event2/bufferevent.h>
#include <event2/event.h>

#include <string.h>

//static std::map<std::string,std::vector<std::string>> serverMap1;
extern std::map<std::string,std::vector<std::string>> ipMap;
extern std::mutex global_mutex;
extern std::map<std::string,std::vector<std::string>> clientNameMap;
extern std::map<std::string,bufferevent* > bevMap;



extern uint32_t isFirstSerMap;
namespace Tools{
    int sp(const std::string&s,std::vector<std::string> &list,void* ptr,struct bufferevent* bev);
}

#endif //XCOM_GLOBAL_H
