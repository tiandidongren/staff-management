#ifndef PUBLIC_H_
#define PUBLIC_H_

#include<stdbool.h>

#define LOGINFAIL 		0
#define ROOTLOGIN 		1
#define GENERALLOGIN 	2
#define LOGOUT 			3
#define ROOTCHECK 		4
#define ROOTALTER 		5
#define ROOTADD 		6
#define ROOTDELETE 		7
#define ROOTHISTORY 	8
#define ROOTLOGOUT 		9
#define GENERALCHECK 	10
#define GENERALALTER 	11
#define GENERALLOGOUT 	12
#define OPERATEFAIL 	13

#define LENGTH 12  		//用于名字.入职日期以及用户名长度
#define POSLENGTH 120 	//位置长度
#define MSGLENGTH 120 	//消息长度

typedef struct staff
{
	int workNum;
	char name[LENGTH];
	char password[LENGTH];
	int age;
	bool man;
	int salary;
//	char position[POSLENGTH];
//	int phoneNum;
//	int level;
//	char workData[LENGTH];
//	int userType;
//	void *released;
}staff_t;

typedef struct MSG
{
	int userPower;
	int textType;
	char userName[LENGTH];
	char password[MSGLENGTH];
	staff_t staff_info;
}messageType;

#endif
