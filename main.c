#include<stdio.h>
#include<pthread.h>
#include<stdbool.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#include"server.h"

#define PATH_DATA "test.db"
int main(int argc, const char *argv[])
{
	if(argc!=3)
	{
		printf("please enter:%s serverip serverport\n",argv[0]);
		return -1;
	}

	if(sqlite3_open(PATH_DATA,&db)!=SQLITE_OK)
	{
		printf("%s\n",sqlite3_errmsg(db));
		return -1;
	}

	int serverFd,clientFd;
	if(!createServer(&serverFd,argv[1],argv[2]))
	{
		return -1;
	}

	struct sockaddr_in clientAdd;
	socklen_t addLength=sizeof(struct sockaddr_in);
	pthread_t pthreadNum;

	while(1)
	{
		clientFd=accept(serverFd,(struct sockaddr*)&clientAdd,&addLength);
		if(-1==clientFd)
		{
			perror("accept");
			continue;
		}
		pthread_create(&pthreadNum,NULL,pthreadOperate,(void *)clientFd);
		
	}
	return 0;
}
