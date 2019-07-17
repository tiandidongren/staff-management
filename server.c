#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>


#include"server.h"
#include"public.h"
//创建服务器端,并绑定以及监听,成功返回true
bool createServer(int *fd,const char *ip,const char *port)
{
	*fd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==*fd)
	{
		perror("socket");
		return false;
	}
	struct sockaddr_in *serverAdd=(struct sockaddr_in*)
		malloc(sizeof(struct sockaddr_in));
    serverAdd->sin_family=AF_INET;                                           
    serverAdd->sin_port=htons(atoi(port));
    serverAdd->sin_addr.s_addr=inet_addr(ip);
 
	int bindRet;
	bindRet=bind(*fd, (struct sockaddr *)serverAdd,\
			sizeof(struct sockaddr_in));
	if(bindRet<0)
	{
		perror("bind");
		return false;
	}
	int listenRet=listen(*fd,10);
	if(listenRet<0)
	{
		perror("listen");
		return false;
	}

	return true;
}

//子线程的处理函数,在函数中为消息分配空间,函数的参数为客户端描述符
void* pthreadOperate(void *clientFd)
{
	int *fd=(int*)clientFd;
	messageType *msg=(messageType*)malloc(sizeof(messageType));
	if(NULL==msg)
	{
		perror("malloc");
		return NULL;
	}
	memset(msg,0,sizeof(messageType));
	printf("messageType,%d\n",sizeof(messageType));

	printf("*fd=%d\n",*fd);
	
	int ret=recv(*fd,(void*)msg,sizeof(messageType),0);
	if(-1==ret)
	{
		perror("recv");
		return NULL;
	}

	while(1)
	{
		switch(msg->textType)
		{
		case ROOTLOGIN:
			rootLogin(fd,msg);
			break;
		case ROOTCHECK:
			rootCheck(fd,msg);
			break;
		case ROOTALTER:
			rootAlter(fd,msg);
			break;
		case ROOTADD:
			rootAdd(fd,msg);
			break;
		case ROOTDELETE:
			rootDelete(fd,msg);
			break;
		case ROOTHISTORY:
			rootHistory(fd,msg);
			break;
		case GENERALLOGIN:
 			generalLogin(fd,msg);
			break;
		case GENERALCHECK:
 			generalCheck(fd,msg);
			break;
		case GENERALALTER:
 			generalAlter(fd,msg);
			break;
		case ROOTLOGOUT:
		case GENERALLOGOUT:
			send(*fd,msg,sizeof(messageType),0);
			break;
		case LOGOUT:
			send(*fd,msg,sizeof(messageType),0);
			pthread_exit(NULL);
			break;
		default:
			break;
		}
		int ret=recv(*fd,msg,sizeof(messageType),0);
		if(-1==ret)
		{
			perror("recv");
			return NULL;
		}
	}

}

//root用户的登录,查询数据库失败,将textType置为LOGINFAIL
void rootLogin(int *clientFd,messageType *msg)
{
	if(loginCheck(msg))
	{
		strcpy(msg->password,"ok");
	}
	else
	{
		strcpy(msg->password,"fail");
		msg->textType=LOGINFAIL;
	}

	send(*clientFd,msg,sizeof(messageType),0);
}

//root用户的查询,失败将textType置为LOGINFAIL
void rootCheck(int *clientFd,messageType *msg)
{
	//按照工号查询
	if(workNumCheck(msg))
	{
		strcpy(msg->password,"ok");
	}
	else
	{
		strcpy(msg->password,"fail");
		msg->textType=OPERATEFAIL;
	}
	send(*clientFd,msg,sizeof(messageType),0);
}

//root用户的修改,失败将textType置为LOGINFAIL
void rootAlter(int *clientFd,messageType *msg)
{
	//按照工号修改
	if(dataAlterPassword(msg))
	{
		strcpy(msg->password,"ok");
	}
	else
	{
		strcpy(msg->password,"fail");
		msg->textType=OPERATEFAIL;
	}
	send(*clientFd,msg,sizeof(messageType),0);
}

//root用户的添加,失败将textType置为LOGINFAIL
void rootAdd(int *clientFd,messageType *msg)
{
	if(dataAdd(msg))
	{
		strcpy(msg->password,"ok");
	}
	else
	{
		strcpy(msg->password,"fail");
		msg->textType=OPERATEFAIL;
	}
	send(*clientFd,msg,sizeof(messageType),0);
}

//root用户的删除,失败将textType置为LOGINFAIL
void rootDelete(int *clientFd,messageType *msg)
{
	//按照工号删除
	if(dataDelete(msg))
	{
		strcpy(msg->password,"ok");
	}
	else
	{
		strcpy(msg->password,"fail");
		msg->textType=OPERATEFAIL;
	}
	send(*clientFd,msg,sizeof(messageType),0);
}

//root用户的历史查询,失败将textType置为LOGINFAIL
void rootHistory(int *clientFd,messageType *msg)
{
}

//普通用户的登录,失败将textType置为LOGINFAIL
void generalLogin(int *clientFd,messageType *msg)
{
	if(loginCheck(msg))
	{
		strcpy(msg->password,"ok");
	}
	else
	{
		strcpy(msg->password,"fail");
		msg->textType=LOGINFAIL;
	}

	send(*clientFd,msg,sizeof(messageType),0);
}

//普通用户的查询,失败将textType置为LOGINFAIL
void generalCheck(int *clientFd,messageType *msg)
{
	if(workNumCheck(msg))
	{
		strcpy(msg->password,"ok");
	}
	else
	{
		strcpy(msg->password,"fail");
		msg->textType=OPERATEFAIL;
	}
	send(*clientFd,msg,sizeof(messageType),0);
}

//普通用户的修改,失败将textType置为LOGINFAIL
void generalAlter(int *clientFd,messageType *msg)
{
	//按照工号修改
	if(dataAlterPassword(msg))
	{
		strcpy(msg->password,"ok");
	}
	else
	{
		strcpy(msg->password,"fail");
		msg->textType=OPERATEFAIL;
	}
	send(*clientFd,msg,sizeof(messageType),0);
}

//登录查询,成功返回true
bool loginCheck(messageType*msg)
{	
	char sql[SQLLENGTH]={0};
	char *errmsg;
	char **rep;
	int n_row;
	int n_column;

	sprintf(sql,"select * from staff where name='%s' and password='%s'",msg->userName,msg->password);
	if(sqlite3_get_table(db,sql,&rep,&n_row,&n_column,&errmsg)!=SQLITE_OK)
	{
		printf("+++++++登录失败+++++++\n");
		return false;
	}
	else
	{
		if(0==n_row)
		{
			printf("登录失败\n");
			return false;
		}
		else
		{
			printf("%s\n",rep[n_column]);
			msg->staff_info.workNum=atoi(rep[n_column]);
		//	printf("%s\t%s\t\n",rep[n_column],rep[n_column+1]);
			printf("登录成功\n");
			return true;
		}
	}
}

//按照工号查询
bool workNumCheck(messageType*msg)
{
	char sql[SQLLENGTH]={0};
	char *errmsg;
	char **rep;
	int n_row;
	int n_column;

	sprintf(sql,"select * from staff where workNum='%d'",msg->staff_info.workNum);
	if(sqlite3_get_table(db,sql,&rep,&n_row,&n_column,&errmsg)!=SQLITE_OK)
	{
		printf("查询失败\n");
		return false;
	}
	else
	{
		if(0==n_row)
		{
			printf("查询失败\n");
			return false;
		}
		else
		{
			msg->staff_info.workNum=atoi(rep[n_column]);
			strcpy(msg->staff_info.name,rep[n_column+1]);
			strcpy(msg->staff_info.password,rep[n_column+2]);
			msg->staff_info.age=atoi(rep[n_column+3]);
			msg->staff_info.man=atoi(rep[n_column+4]);
			msg->staff_info.salary=atoi(rep[n_column+5]);
			return true;
		}
	}

}

//修改数据
bool dataAlterPassword(messageType*msg)
{
	char sql[SQLLENGTH]={0};
	char *errmsg;
	char **rep;
	int n_row;
	int n_column;

	sprintf(sql,"update staff set password='%s' where workNum=%d"
			,msg->staff_info.password,msg->staff_info.workNum);
	if(sqlite3_exec(db,sql,callback,NULL,&errmsg)!=SQLITE_OK)
	{
		printf("修改失败\n");
		return false;
	}
	else
	{
		if(0==n_row)
		{
			printf("修改失败\n");
			return false;
		}
		else
		{
			printf("修改成功\n");
			return true;
		}
	}
}

//添加数据
bool dataAdd(messageType*msg)
{
	char sql[SQLLENGTH]={0};
	char *errmsg;
	char **rep;
	int n_row;
	int n_column;

	sprintf(sql,"insert into staff values(%d,'%s','%s',%d,%d,%d)",
			msg->staff_info.workNum,msg->staff_info.name,
			msg->staff_info.password,msg->staff_info.age,
			msg->staff_info.man,msg->staff_info.salary);
	if(sqlite3_exec(db,sql,callback,NULL,&errmsg)!=SQLITE_OK)
	{
		printf("添加失败\n");
		return false;
	}
	else
	{
		if(0==n_row)
		{
			printf("添加失败\n");
			return false;
		}
		else
		{
			printf("添加成功\n");
			return true;
		}
	}
}

//按照工号删除数据
bool dataDelete(messageType*msg)
{
	char sql[SQLLENGTH]={0};
	char *errmsg;
	char **rep;
	int n_row;
	int n_column;

	sprintf(sql,"delete from staff where workNum=%d"
			,msg->staff_info.workNum);
	if(sqlite3_exec(db,sql,callback,NULL,&errmsg)!=SQLITE_OK)
	{
		printf("删除失败\n");
		return false;
	}
	else
	{
		if(0==n_row)
		{
			printf("删除失败\n");
			return false;
		}
		else
		{
			printf("删除成功\n");
			return true;
		}
	}
}


//回调函数
int callback(void *data,int f_num,char **f_value,char **f_name)
{
	//空
}
