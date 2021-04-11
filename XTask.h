#ifndef XCOM_XTASK_H
#define XCOM_XTASK_H
class XTask{
public:
    struct event_base* base = 0;
    int sock = 0;
    int thread_id = 0;
    //纯虚函数
    virtual bool Init() = 0;
    XTask();
    ~XTask();

};
#endif //XCOM_XTASK_H
