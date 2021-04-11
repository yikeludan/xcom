//
// Created by 82017 on 2021-03-20.
//

#ifndef XCOM_XTOOLS_H
#define XCOM_XTOOLS_H
#include <vector>
#include <string>
#include <string.h>
namespace Tools{
    void sp(const string&s,vector<string> &list){
        int re = s.find("|");
        int re1 = s.find("%");
        int re2 = s.find("&");
        std::string name = s.substr(re+1,re1-1);
        std::string package = s.substr(re2+1);
        const char* p = package.c_str();
        int pos = 0;
        char src[1024] = {0};
        while (*p !='\0'){
            if(*p == ';'){
                src[pos] = '\0';
                string sa(src);
                list.push_back(sa);
                memset(src,0,strlen(src)+1);
                p+=1;
                pos = 0;
            }
            strncpy(src+pos,p,1);
            p+=1;
            pos+=1;
        }
    }

}
#endif //XCOM_XTOOLS_H
