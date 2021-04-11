
#ifndef XCOM_XMSG_H
#define XCOM_XMSG_H
#pragma pack(1)//设定为 1 字节对齐

enum MsgType{
    MSG_NONE = 0,
    MSG_GETDIR,
    MSG_MAX_TYPE,
    MSG_DIRLIST
};
struct XMsgHead{
    MsgType type;
    int size;
};
struct XMicoService{
    char server_name[50];
    char type[5] = {0};
    char ip[20];
    char port[10];
};
struct XMsg: public XMsgHead{
    char* data = 0;
    int recvied = 0;//已经接收的长度
};
#endif //XCOM_XMSG_H
