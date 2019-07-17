#ifndef SERVER_H_
#define SERVER_H_
#include<stdbool.h>
#include<sqlite3.h>
#include"public.h"
#define SQLLENGTH 100
sqlite3 *db;

//创建服务器端,并绑定以及监听,成功返回true
bool createServer(int *fd,const char *ip,const char *port);
//子线程的处理函数,在函数中为消息分配空间,函数的参数为客户端描述符
void* pthreadOperate(void *clientFd);
//root用户的登录,查询数据库失败,将textType置为LOGINFAIL
void rootLogin( int *clientFd,messageType *msg);
//root用户的查询,失败将textType置为LOGINFAIL
void rootCheck( int *clientFd,messageType *msg);
//root用户的修改,失败将textType置为LOGINFAIL
void rootAlter( int *clientFd,messageType *msg);
//root用户的添加,失败将textType置为LOGINFAIL
void rootAdd( int *clientFd,messageType *msg);
//root用户的删除,失败将textType置为LOGINFAIL
void rootDelete( int *clientFd,messageType *msg);
//root用户的历史查询,失败将textType置为LOGINFAIL
void rootHistory( int *clientFd,messageType *msg);
//普通用户的登录,失败将textType置为LOGINFAIL
void generalLogin( int *clientFd,messageType *msg);
//普通用户的查询,失败将textType置为LOGINFAIL
void generalCheck( int *clientFd,messageType *msg);
//普通用户的修改,失败将textType置为LOGINFAIL
void generalAlter( int *clientFd,messageType *msg);
//登录查询,成功返回true
bool loginCheck(messageType*msg);
//按照工号查询
bool workNumCheck(messageType*msg);
//修改数据
bool dataAlterPassword(messageType*msg);
//添加数据
bool dataAdd(messageType*msg);
//按照工号删除数据
bool dataDelete(messageType*msg);
//回调函数
int callback(void *data,int f_num,char **f_value,char **f_name);

#endif
