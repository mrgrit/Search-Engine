#include "global.h"

//url링크 추출기 함수
void parse(char *filename,char *host_name,char *title,char *comment)
{
	char buf[6],c,pathname[200];	
	char *path[200];  //url 임시 저장 버퍼
	FILE *fs1;
	int count=0,j,qcount=0;
	static int tr;  //url 중복확인을 위한 값

	if ((fs1=fopen(filename,"r")) == NULL)
	{
		fprintf(stderr,"파일을 열수가 없습니다 ");
		return;
	}

	while(fread(buf,6,1,fs1)>0){
		if (strncmp(buf,"<a",2)==0||strncmp(buf,"<A",2)==0||strncmp(buf,"<frame",6)==0||strncmp(buf,"<FRAME",6)==0){  //<a태그 찾기
			fseek(fs1,-3,SEEK_CUR);
			while(strncmp(buf,">",1)!=0){
				fread(buf,4,1,fs1);
					if(strncmp(buf,"href",4)==0||strncmp(buf,"HREF",4)==0||strncmp(buf,"src",3)==0||strncmp(buf,"SRC",3)==0){ //href태그 찾기
						while(1){
							c=fgetc(fs1);
							if(c!=' ' && c!='=' && c!='"'&& c != '\'') break; //해당문자 저장하지 않기
						}
						strncat(pathname,&c,1);
						while(1){
							c=fgetc(fs1);
							if(c=='\t'||c=='>'||c==' '||c=='"'||c=='\n'||c=='\''||c==EOF||c=='?'||c==';') break; //해당문자를 맞나면 끝내기
							strncat(pathname,&c,1);
						}
						if(*(pathname+strlen(pathname)-1)!='/'){  //url 구별하기 위해
							strncat(pathname,"/",1); //url끝에 /추가하기
						}
						if(separation(pathname, host_name)>0) //링크 구별하기
						{
							path[count]=(char *)malloc(strlen(pathname)+1);
							tr = 1;
							for (qcount=0;qcount<host.r+1;qcount++){
								if(strncmp(host.name[qcount],pathname,strlen(pathname))==0){
									//큐와 url을 비교 같으면 입력 X
									tr=0;  // path임시버퍼에 입력X
								}
							}
							if (tr) {
								if (count == 0){
									strncpy(path[count],pathname,strlen(pathname)+1);
									//한 사이트내에 중복 url에 있으면 입력 X
									count++;
								}
								else {
							//	tr =1;
									for (j=0;j<count;j++){
										if(strcmp(path[j],pathname)==0){
											tr=0;
										}
									}
									if (tr) { //중복되는 것이 없으면 임시 버퍼에 저장
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
		if (host.r==QUEUE_MAX_SIZE-1){ //큐에 url에 모두 추가되면 더이상 큐에 입력 X
			free(path[j]);
			break;
		}
		else
			add(path[j]); //큐에 url입력
//		printf("path[%d]	 : %s\n",j,path[j]);
		printf("INPUT Queue URL name[%d] : %s\n",host.r,host.name[host.r]);
		free(path[j]);
	}
	fclose (fs1);
}


//내부링크와 외부링크 구별하기 함수
int separation(char *pathname, char *host_name)
{
	char buf[200],buff[200];
	int j,k;
	memset(buf,'\0',sizeof(buf));
	memset(buff,'\0',sizeof(buff));
	strcpy(buf,"");
	strcpy(buff,"");
	if( strncmp(pathname,"http://",7)==0){ // 'http://'이부분 삭제하기
		for(k=0;k<(strlen(pathname)-6);k++){
			buf[k]=*(pathname+7+k);
		}
		for(j=0;buf[j]!='/'&&buf[j]!=':';j++){ // 도메인네임 구별하여 저장하기
			buff[j]=buf[j];
		}
		if(strncmp(buff,host_name,strlen(host_name))!=0){ //현재 사이트와 비교하기
			strcpy(pathname, buff);
			return 1;
		}
	}
	return 0;
}
