#ifndef CLIENT_H_
#define CLIENT_H_
#include"public.h"
//创建套接字,并链接服务器,成功返回true
bool createClient(int *fd,char *ip,char *port);
//选择超级.普通用户登录或退出
void userLoginInterface(void);
//发送登录或者退出的请求,登录成功返回用户类型,登录失败返回0
int sendLoginRequest(const int *fd,messageType* msgType)
//超级用户的操作界面
void rootUserInterface(void);
//普通用户的操作界面
void generalUserInterface(void);
//超级用户的操作函数
void rootOperate(const int *fd,messageType *msgType);
//普通用户的操作函数
void generalOperate(const int *fd,messageType *msgType);
//超级用户的查询操作,分为按人名/查找所有人/返回上一页
void rootCheck(const int *fd,messageType *msgType);
//超级用户的修改操作
void rootAlter(const int *fd,messageType *msgType);
//超级用户的添加操作
void rootAdd(const int *fd,messageType *msgType);
//超级用户的删除操作
void rootDelete(const int *fd,messageType *msgType);
//超级用户的查询历史操作
void rootHistory(const int *fd,messageType *msgType);
//普通用户的查询操作
void generalCheck(const int *fd,messageType *msgType);
//普通用户的修改操作
void generalAlter(const int *fd,messageType *msgType);
//处理多余的字符
void dealMoreChar(void);
//输入用户名和密码
void enterUserPass(messageType*msgType);
//收发消息
void sendAndRecvMsg(const int *fd,messageType*msgType);
//打印数据
void printWorkInfo(messageType*msgType);
#endif
