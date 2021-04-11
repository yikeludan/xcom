cmake_minimum_required(VERSION 3.10)
project(xcomRpc)

set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11 -pthread")
link_directories(
        /usr/local/lib
        /usr/lib
)
INCLUDE_DIRECTORIES(
        /usr/local/include/
        /usr/include
)

add_executable(xcomRpc main.cpp XTask.h XTask.cpp XThreadPool.cpp
        XThreadPool.h XThread.h XThread.cpp XServerTask.h XServerTask.cpp
        XcomTask.h XcomTask.cpp
        XRpcTask.h XRpcTask.cpp
        XMsg.h XServer.h XServer.cpp XServerHandle.h XServerHandle.cpp XServiceProxy.h XServiceProxy.cpp global.h XTools.h global.cpp)
target_link_libraries(xcomRpc event)