#include "global.h"

//url��ũ ����� �Լ�
void parse(char *filename,char *host_name,char *title,char *comment)
{
	char buf[6],c,pathname[200];	
	char *path[200];  //url �ӽ� ���� ����
	FILE *fs1;
	int count=0,j,qcount=0;
	static int tr;  //url �ߺ�Ȯ���� ���� ��

	if ((fs1=fopen(filename,"r")) == NULL)
	{
		fprintf(stderr,"������ ������ �����ϴ� ");
		return;
	}

	while(fread(buf,6,1,fs1)>0){
		if (strncmp(buf,"<a",2)==0||strncmp(buf,"<A",2)==0||strncmp(buf,"<frame",6)==0||strncmp(buf,"<FRAME",6)==0){  //<a�±� ã��
			fseek(fs1,-3,SEEK_CUR);
			while(strncmp(buf,">",1)!=0){
				fread(buf,4,1,fs1);
					if(strncmp(buf,"href",4)==0||strncmp(buf,"HREF",4)==0||strncmp(buf,"src",3)==0||strncmp(buf,"SRC",3)==0){ //href�±� ã��
						while(1){
							c=fgetc(fs1);
							if(c!=' ' && c!='=' && c!='"'&& c != '\'') break; //�ش繮�� �������� �ʱ�
						}
						strncat(pathname,&c,1);
						while(1){
							c=fgetc(fs1);
							if(c=='\t'||c=='>'||c==' '||c=='"'||c=='\n'||c=='\''||c==EOF||c=='?'||c==';') break; //�ش繮�ڸ� �³��� ������
							strncat(pathname,&c,1);
						}
						if(*(pathname+strlen(pathname)-1)!='/'){  //url �����ϱ� ����
							strncat(pathname,"/",1); //url���� /�߰��ϱ�
						}
						if(separation(pathname, host_name)>0) //��ũ �����ϱ�
						{
							path[count]=(char *)malloc(strlen(pathname)+1);
							tr = 1;
							for (qcount=0;qcount<host.r+1;qcount++){
								if(strncmp(host.name[qcount],pathname,strlen(pathname))==0){
									//ť�� url�� �� ������ �Է� X
									tr=0;  // path�ӽù��ۿ� �Է�X
								}
							}
							if (tr) {
								if (count == 0){
									strncpy(path[count],pathname,strlen(pathname)+1);
									//�� ����Ʈ���� �ߺ� url�� ������ �Է� X
									count++;
								}
								else {
							//	tr =1;
									for (j=0;j<count;j++){
										if(strcmp(path[j],pathname)==0){
											tr=0;
										}
									}
									if (tr) { //�ߺ��Ǵ� ���� ������ �ӽ� ���ۿ� ����
										strncpy(path[count], pathname, strlen(pathname)+1);
										count++;
									}
								}
							}
						}
					}
					fseek(fs1,-3,SEEK_CUR);
			}
		}
		memset(pathname,'\0',sizeof(pathname));
		strcpy(pathname,"");
		fseek(fs1,-5,SEEK_CUR);
	}
	for (j=0;j<count;j++)
	{
		if (host.r==QUEUE_MAX_SIZE-1){ //ť�� url�� ��� �߰��Ǹ� ���̻� ť�� �Է� X
			free(path[j]);
			break;
		}
		else
			add(path[j]); //ť�� url�Է�
//		printf("path[%d]	 : %s\n",j,path[j]);
		printf("INPUT Queue URL name[%d] : %s\n",host.r,host.name[host.r]);
		free(path[j]);
	}
	fclose (fs1);
}


//���θ�ũ�� �ܺθ�ũ �����ϱ� �Լ�
int separation(char *pathname, char *host_name)
{
	char buf[200],buff[200];
	int j,k;
	memset(buf,'\0',sizeof(buf));
	memset(buff,'\0',sizeof(buff));
	strcpy(buf,"");
	strcpy(buff,"");
	if( strncmp(pathname,"http://",7)==0){ // 'http://'�̺κ� �����ϱ�
		for(k=0;k<(strlen(pathname)-6);k++){
			buf[k]=*(pathname+7+k);
		}
		for(j=0;buf[j]!='/'&&buf[j]!=':';j++){ // �����γ��� �����Ͽ� �����ϱ�
			buff[j]=buf[j];
		}
		if(strncmp(buff,host_name,strlen(host_name))!=0){ //���� ����Ʈ�� ���ϱ�
			strcpy(pathname, buff);
			return 1;
		}
	}
	return 0;
}
