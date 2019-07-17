#include"client.h"
#include"public.h"
#include<sys/socket.h>
#include<sys/types.h>

//创建套接字,并链接服务器,成功返回true
bool createClient(int *fd,const char *ip,const char *port)
{
	*fd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==*fd)
	{
		printf("create socket is fail!\n");
		return false;
	}
	struct sockaddr_in serverAdd;
	serverAdd.sin_family=AF_INET;
	serverAdd.sin_port=htons(atoi(port));
	serverAdd.sin_addr.s_addr=inet_addr(ip);

	int ret=connect(*fd,(struct sockaddr*)&serverAdd,
			sizeof(struct sockaddr_in));
	if(-1==ret)
	{
		printf("connect server id fail!\n");
		return false;
	}

	return true;
}

//选择超级.普通用户登录或退出
void userLoginInterface(void)
{
	printf("**************************************\n");
	printf("\t1.管理员\t2.普通用户\t3.退出\n");
	printf("**************************************\n");
}

//发送登录或者退出的请求,登录成功返回用户类型,登录失败返回0
int sendLoginRequest(const int *fd,messageType* msgType)
{
	printf("请输入选项:");
	scanf("%d",&(msgType->textType));
	switch(msgType->textType)
	{
	case ROOTLOGIN:
		enterUserPass(msgType);
		sendAndRecvMsg(fd,msgType);
		break;
	case GENERALLOGIN:
		enterUserPass(msgType);
		sendAndRecvMsg(fd,msgType);
		break;
	case LOGOUT:
		sendAndRecvMsg(fd,msgType);
		break;
	default:
		break;
	}

	return msgType->textType;
}

//超级用户的操作界面
void rootUserInterface(void)
{
	printf("**************************************\n");
	printf("\t1.查询\t2.修改\t3.添加\t4.删除\t5.退出\n");
	printf("**************************************\n");
}

//普通用户的操作界面
void generalUserInterface(void)
{
	printf("**************************************\n");
	printf("\t1.查询\t2.修改\t3.退出\n");
	printf("**************************************\n");
}

//超级用户的操作函数
void rootOperate(const int *fd,messageType *msgType)
{
	printf("请输入选项:");
	scanf("%d",&(msgType->textType));
	dealMoreChar();
	//因为使用一个标志位,所以输入选项应加上登录界面所占的三个数
	msgType->textType+=LOGOUT;

	switch(msgType->textType)
	{
	case ROOTCHECK:
		rootCheck(fd,msgType);
		break;
	case ROOTALTER:
		rootAlter(fd,msgType);
		break;
	case ROOTADD:
		rootAdd(fd,msgType);
		break;
	case ROOTDELETE:
		rootDelete(fd,msgType);
		break;
	case ROOTHISTORY:
		break;
	case ROOTLOGOUT:
		msgType->textType=LOGOUT;
		sendAndRecvMsg(fd,msgType);
		break;
	default:
		break;
	}
}

//普通用户的操作函数,当普通用户登录时会把工号返回,
//只能操作对应工号的内容
void generalOperate(const int *fd,messageType *msgType)
{
	printf("你的工号为:%d\n",msgType->staff_info.workNum);

	printf("请输入选项:");
	scanf("%d",&(msgType->textType));
	dealMoreChar();
	//因为使用一个标志位,所以输入选项应加上登录界面
	//以及root用户所占的九个数
	msgType->textType+=ROOTLOGOUT;

	switch(msgType->textType)
	{
	case GENERALCHECK:
		generalCheck(fd,msgType);
		break;
	case GENERALALTER:
		generalAlter(fd,msgType);
		break;
	case GENERALLOGOUT:
		msgType->textType=LOGOUT;
		sendAndRecvMsg(fd,msgType);
		break;
	default:
		break;
	}

}

//超级用户的查询操作,分为按人名/查找所有人/返回上一页
void rootCheck(const int *fd,messageType *msgType)
{
	printf("请输入工号:");
	scanf("%d",&(msgType->staff_info.workNum));
	
	sendAndRecvMsg(fd,msgType);
	if(OPERATEFAIL==msgType->textType)
	{
		printf("operator is fail");
		return ;
	}
	printWorkInfo(msgType);
}

//超级用户的修改操作
void rootAlter(const int *fd,messageType *msgType)
{
	printf("请输入工号:");
	scanf("%d",&(msgType->staff_info.workNum));

	sendAndRecvMsg(fd,msgType);
	if(OPERATEFAIL==msgType->textType)
	{
		printf("operator is fail");
		return ;
	}
	printWorkInfo(msgType);
	printf("修改成功\n");
}

//超级用户的添加操作
void rootAdd(const int *fd,messageType *msgType)
{
	printf("请输入工号:");
	scanf("%d",&(msgType->staff_info.workNum));
	printf("请输入姓名:");
	scanf("%s",msgType->staff_info.name);
	printf("请输入密码:");
	scanf("%s",msgType->staff_info.password);
	printf("请输入年龄:");
	scanf("%d",&(msgType->staff_info.age));
	printf("请输入性别:");
	int man;
	scanf("%d",&man);
	msgType->staff_info.man=man;
	printf("请输入薪水:");
	scanf("%d",&(msgType->staff_info.salary));

	sendAndRecvMsg(fd,msgType);
	if(OPERATEFAIL==msgType->textType)
	{
		printf("operator is fail");
		return ;
	}
	printWorkInfo(msgType);
	printf("添加成功\n");
}

//超级用户的删除操作
void rootDelete(const int *fd,messageType *msgType)
{	
	printf("请输入工号:");
	scanf("%d",&(msgType->staff_info.workNum));

	sendAndRecvMsg(fd,msgType);
	if(OPERATEFAIL==msgType->textType)
	{
		printf("operator is fail");
		return ;
	}
	printWorkInfo(msgType);
	printf("删除成功\n");

}

//超级用户的查询历史操作
void rootHistory(const int *fd,messageType *msgType)
{

}

//普通用户的查询操作
void generalCheck(const int *fd,messageType *msgType)
{	
	sendAndRecvMsg(fd,msgType);
	if(OPERATEFAIL==msgType->textType)
	{
		printf("operator is fail");
		return ;
	}
	printWorkInfo(msgType);
}

//普通用户的修改操作
void generalAlter(const int *fd,messageType *msgType)
{
	printf("请输入密码:");
	scanf("%s",msgType->staff_info.password);

	sendAndRecvMsg(fd,msgType);
	if(OPERATEFAIL==msgType->textType)
	{
		printf("operator is fail");
		return ;
	}
	printWorkInfo(msgType);

}

//处理多余的字符
void dealMoreChar(void)
{
	while(getchar()!='\n')
		continue;
}

//输入用户名和密码
void enterUserPass(messageType *msgType)
{
	printf("请输入用户名(10个字符以内):");
	scanf("%s",msgType->userName);
	msgType->userName[9]='0';
	printf("请输入密码(10个字符以内):");
	scanf("%s",msgType->password);
	msgType->password[9]='0';
	dealMoreChar();
}

//收发消息
void sendAndRecvMsg(const int *fd,messageType*msgType)
{
	int ret=send(*fd,(void *)msgType,sizeof(struct MSG),0);
	if(-1==ret)
	{
		printf("send msg fail!\n");
		msgType->textType=LOGINFAIL;
	}

	ret=recv(*fd,(void *)msgType,sizeof(struct MSG),0);
	if(-1==ret)
	{
		printf("recv msg fail!\n");
		msgType->textType=LOGINFAIL;
	}
}

//打印数据
void printWorkInfo(messageType*msgType)
{
	printf("workNum\tname\tpassword\tage\tman\tsalary\n");
	printf("%d\t%s\t%s\t%d\t%d\t%d\t",msgType->staff_info.workNum,\
			msgType->staff_info.name,msgType->staff_info.password,\
			msgType->staff_info.age,msgType->staff_info.man,\
			msgType->staff_info.salary);
}
