#include<stdio.h>
#include<string.h>

#include"client.h"
#include"public.h"


int main(int argc, const char *argv[])
{
	if(argc!=3)
	{
		printf("please enter:%s serverip serverport\n",argv[0]);
		return 0;
	}

	int clientFd;

	if(!createClient(&clientFd,argv[1],argv[2]))
	{
		return 0;
	}

	messageType *news=(messageType*)malloc(sizeof(messageType));
	memset(news,0,sizeof(messageType));

	while(news->textType!=LOGOUT)
	{
		userLoginInterface();
		switch(sendLoginRequest(&clientFd,news))
		{
		case LOGINFAIL:
			break;
		case ROOTLOGIN:
			while(news->textType!=ROOTLOGOUT)
			{
				rootUserInterface();
				rootOperate(&clientFd,news);
			}
			break;
		case GENERALLOGIN:
			while(news->textType!=GENERALLOGOUT)
			{
				generalUserInterface();
				generalOperate(&clientFd,news);
			}
			break;
		case LOGOUT:
			break;
		default:
			printf("error!now exit!");
			news->textType=LOGOUT;
			break;
		}
	}

	free(news);
	news=NULL;
	return 0;
}
