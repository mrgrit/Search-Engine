#include "global.h"


void init()  //����ť �ʱ�ȭ
{
	int i;
	for (i=0; i<QUEUE_MAX_SIZE; i++){
		memset(host.name[i],0x00, 200);
		if (host.name[i]==NULL){
			printf("1\n");
		}
	}
	

	host.f=-1;
	host.r=-1;
}

void add(char *hostname)  //����ť�� ���� ����ֱ�
{  
	
	if (host.r == QUEUE_MAX_SIZE-1)
	{
//	   printf("Queue Overflow\n");
	   return;
	}
	else
		strncpy(host.name[++host.r], hostname, strlen(hostname)+1);
}


void del(char title[],char comment[]) //����ť �����
{ 
	if (host.f == host.r)
	{
		again(title,comment);
//		printf("Queue Underflow\n");
		return;
	}
	else 
		host.f = (host.f+1); 
}

