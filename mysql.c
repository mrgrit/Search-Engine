#include "global.h"

//mysql�� ���� �����ϴ� �Լ�
int mysql_data_insert(char *url, char* title, char* comment) 
{
	MYSQL         mysql;
	MYSQL_RES     *res;
	MYSQL_ROW     row;
    int           fields;
	int           i,number;
	char          query[100000];
	MYSQL_FIELD   *field;

	mysql_init(&mysql);

	memset(query,'\0',100000);
	if(!mysql_real_connect(&mysql,"localhost","root","search","project",0,(char *)NULL,0))//mysql ������ �����ϴ� �Լ� 
	{
		printf("%s\n",mysql_error(&mysql));
		exit(1);
	}

	sprintf(query, "select url from search where url='%s'", url);
//	query="select url from search where url='http://naver.com'";

	if(mysql_real_query(&mysql,query,strlen(query))) //query �������� query�� ���̸�ŭ sql ������ �����ϴ� �Լ� 
	{
		printf("%s\n",mysql_error(&mysql));
		exit(1);
	}
	
	res = mysql_store_result(&mysql);  //�����κ��� ������ ����� ���� ����� ��ü�� �������� �Լ�  
	
	number=mysql_num_rows(res);

    if(number>0) {
//		printf("�ش� URL �ּҰ� �̹� �ֽ��ϴ�\n");
	}else {
			
		memset(query,'\0',100000);
		sprintf(query, "insert into search values ('', '%s', '%s', '%s')", url,title,comment);

		if( mysql_real_query(&mysql,query,strlen(query))) {
			printf("error no:   %d\n%s\n",mysql_errno(&mysql),mysql_error(&mysql));
			exit(1);
	    }
    }    
	mysql_free_result(res);
	mysql_close(&mysql);
		
	return 0;
}


//html�±׿��� Ÿ��Ʋ�� ������ �и��س��� �Լ�
char list(char *filename,char *title,char *comment)
{
	char buf[50],c;
	FILE *fs1;
	int i;

	if ((fs1=fopen(filename,"r")) == NULL) 
	{
		fprintf(stderr,"������ ������ �����ϴ� ");
		return;
	}
	memset(title,0,sizeof(title));
	memset(comment,0,sizeof(comment));


	//Ÿ��Ʋ �и�
	while(fread(buf,7,1,fs1)>0){
		if (strncmp(buf,"<title>",7)==0||strncmp(buf,"<TITLE>",7)==0){
			while(1){
				if((c=fgetc(fs1))!=' ') break;
			}
			strncat(title,&c,1);
			for (;;){
				if((c=fgetc(fs1))=='<') break;
				if (c!='\n'&&c!='\t'&&c!='\''&&c!=':'){
					strncat(title,&c,1);
				}
			}
		}

		//�������� �и�
		if(strncmp(buf,"<body",5)==0||strncmp(buf,"<BODY",5)==0){
			fseek(fs1,-1,SEEK_CUR);
			while (strncmp(buf,"</body>",7)!=0||strncmp(buf,"</BODY>",7)!=0){
				if ((c=fgetc(fs1))=='>'){
					while ((c=fgetc(fs1))!='<'){
						if (c==EOF) break;
						if (c!='\n'&&c!='\t'&&c!='\''&&c!='"'){
							strncat(comment,&c,1);
						}
					}
					strncat(comment,"\n",1);
				}
				if (!(fread(buf,7,1,fs1)>0)) break;
				fseek(fs1,-7,SEEK_CUR);
			}
		}
		fseek(fs1,-6,SEEK_CUR);
	}
	fclose (fs1);
}
