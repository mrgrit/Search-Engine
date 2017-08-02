#include "global.h"

int cnt = 1; //����ť�� ��ȣ ī��Ʈ
static int thcount = 0; //���� ��ȣ ���� ī��Ʈ
pthread_mutex_t work_mutex;


/////�ش����Ʈ html ����޾ƿ���/////
void search_thread(char *host_name,char *title,char *comment)
{
	int sockfd,fd,nread;
	struct hostent *host_ip;
	struct sockaddr_in destaddr;
	char sendmsg[MAX_STRING_LENGTH];
	char recvmsg[MAX_STRING_LENGTH];
	char filename[40];  //html ���������ϴ� �����̸�
	char buf[30];
	
	thcount += 1;
	memset(filename, 0x00, 40);
	sprintf(filename, "file%d.txt", thcount);

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	memset(&destaddr,0,sizeof(destaddr));
	memset(buf, 0x00, 30);
	strncpy(buf, host_name, strlen(host_name));
	host_ip=gethostbyname(buf);
	
	if (host_ip == NULL) { //���� ����Ʈ üũ
		if (cnt==QUEUE_MAX_SIZE-1){ //ť�� ����ִ� ���
			again(title,comment);
		}
		cnt++;
		again_search(title,comment);
	}

	destaddr.sin_family = AF_INET;
	destaddr.sin_port = htons(80);
	destaddr.sin_addr = *(struct in_addr*)host_ip->h_addr;

	connect(sockfd, (struct sockaddr *)&destaddr, sizeof(destaddr));

	memset(sendmsg,0,MAX_STRING_LENGTH);
	strcpy(sendmsg, "GET http://");
	strcat(sendmsg, host_ip->h_name);
//	strcat(sendmsg, "/index.html");
	strcat(sendmsg, "\n");
	send(sockfd, sendmsg, sizeof(sendmsg), 0);

	if((fd=open(filename,O_WRONLY|O_CREAT|O_TRUNC,0644))==-1)
	{
		perror("file.txt");
		exit(2);
	}

	while(1)
	{
		memset(recvmsg,0,MAX_STRING_LENGTH);
		if((nread=recv(sockfd, recvmsg, sizeof(recvmsg), 0)) <= 0)
		{
			break;
		}
		write(fd,recvmsg,nread);
	}
	close(fd);
	close(sockfd);
	if (host.r != QUEUE_MAX_SIZE-1){
		parse(filename,host_name,title,comment); //��ũ����� �Լ�
	}
	list(filename,title,comment); //Ÿ��Ʋ�� ���� �����Լ�

	//title���� �� �ʿ���� �κ� �ɷ�����
	if (strncasecmp(title,"error",5)!=0 && strncmp(title,"40",2)!=0 && strncmp(title,"Not",3)!=0 && strncmp(title,"30",2)!=0 && strncmp(title,"50",2)!=0 && strncmp(title,"Bad",3)!=0){
		mysql_data_insert(host_name,title,comment); //mysql�� �����ϱ�
	}
	
	unlink(filename); //���� �����
}


//�κ� ��ӽ����ϱ�
void again_search(char title[], char comment[])
{
	char first[200];
	
	for (;cnt<QUEUE_MAX_SIZE;cnt++){
		printf("Search URL name [%d] : %s\n",cnt,host.name[cnt]);
		del(title,comment);
		memset(first,'\0',sizeof(first));
		strncpy(first,host.name[cnt],strlen(host.name[cnt]));
		if ((host.f+1)==host.r){ //ť�� ������� ��� ���ο� ť ����
			init();
			add(first);
			cnt=0;
		}
		search_thread(host.name[cnt],title,comment);
	}

}

//���̻��� url�� ���� ��� ���ο� ����Ʈ �Ҵ� �ϴ� �Լ�
void again(char title[], char comment[])
{
	char name[10][30]={
		"www.linuzine.com",
		"www.sayclub.com",
		"www.hitel.net",
		"www.sbs.co.kr",
		"www.daum.net",
		"www.netian.com",
		"www.kldp.org",
		"www.wowlinux.co.kr",
		"www.linux.co.kr",
		"www.kmu.ac.kr"
	};
	int n;
	time_t t;
	char buf[30];
	time(&t);
	srand(t);

	n=rand()%10;  //���� �߻��� ���� 10�� ����Ʈ �� �ϳ� ����
	memset(buf,'\0',sizeof(buf));
	strcpy(buf,name[n]);
	init();
	add(buf);
	cnt=0;
	search_thread(buf,title,comment);
}


//������ �����Լ�
void s_thread(void *u_name)
{
	char title[250],comment[100000]; //Ÿ��Ʋ�� ���������� ���� �迭����
	memset(title,'\0',sizeof(title));
	memset(comment,'\0',sizeof(comment));
	
	init();
	add(u_name);;
	printf("Search URL name [%d] : %s\n",0,host.name[0]);
	search_thread(host.name[0],title,comment);
	again_search(title,comment);
}

int main(int argc,char *argv[])
{
	char *init_link [] = {
		"www.empas.com",
		"www.naver.com",
		"www.joinc.co.kr",
		"www.dreamwiz.com"
	};
	
	int i;
	pthread_t thread; //������ ����
	int res = pthread_mutex_init(&work_mutex, NULL);;

	for (i=0; i<4; i++) { //������ ����
		pthread_create(&thread,NULL,(void *)&s_thread, (void *)init_link[i]);
		usleep(100);
	}		
	
//	pthread_create(&thread,NULL,(void *)&s_thread, (void *)argv[2]);
//	pthread_create(&thread,NULL,(void *)&s_thread, (void *)argv[3]);
//	pthread_create(&thread,NULL,(void *)&s_thread, (void *)argv[4]);
//	pthread_create(&thread,NULL,(void *)&s_thread, (void *)argv[5]);

	pthread_mutex_lock(&work_mutex);
	pthread_mutex_unlock(&work_mutex);
	pthread_join(thread, NULL);
	
	pthread_mutex_destroy(&work_mutex);
	return 0;
}

