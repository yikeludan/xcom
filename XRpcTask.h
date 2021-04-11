//
// Created by 82017 on 2021-03-17.
//

#ifndef XCOM_XRPCTASK_H
#define XCOM_XRPCTASK_H
#include "XcomTask.h"
#include "XServiceProxy.h"
class XRpcTask: public XcomTask{
public:
    XRpcTask();
    ~XRpcTask();
    //XServiceProxy* xServiceProxy;
    //XServiceProxy* xServiceProxy =0;
};
#endif //XCOM_XRPCTASK_H
