#include <iostream>
#include <event2/event.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <thread>
#include <string>
#include <event2/thread.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>
#include <event2/http.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#include "XThreadPool.h"
#include "XTask.h"
#include "XServerTask.h"
#include "XRpcTask.h"
#include "global.h"
#include "XServiceProxy.h"
static XThreadPool* pool;
extern XServiceProxy* proxy;
static  void ListenCbMainFun(int sock,struct sockaddr* addr,int socklen,void* ptr){
    std::cout<<"222"<<addr->sa_data<<std::endl;
    auto serverTask = (XServerTask *)ptr;
    auto task = new XRpcTask();
    task->clientIp = serverTask->clientIp;
    task->clientPort = serverTask->clientPort;
    task->sock = sock;
    pool->Dispatch(task);
}

void listener_cb1(struct evconnlistener *e,
                  evutil_socket_t s,
                  struct sockaddr *a,
                  int socklen,
                  void *arg){
    std::cout<<"3333"<<std::endl;

}

int main() {
    if(signal(SIGPIPE,SIG_IGN) == SIG_ERR){
        return 1;
    }
    XThreadPool* xThreadPool =XThreadPoolFactory::create();
    //XThreadPool* xThreadPool =new XThreadPool();
    xThreadPool->Init(8);


    //proxy = XServiceProxy::Get();
    pool = xThreadPool;



    XThreadPool* serverPool =XThreadPoolFactory::create();
    //XThreadPool* serverPool = XThreadPool::Get();
    serverPool->Init(1);
    auto task = new XServerTask();
    task->serverPort = 7777;
    task->listenCb =ListenCbMainFun;
    serverPool->Dispatch(task);
    serverPool->Wait();
    /*while (true){
        sleep(1);
    }*/


}
