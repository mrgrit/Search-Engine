#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <pthread.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <mysql.h>
#include <errno.h>

#define MAX_STRING_LENGTH	100
#define QUEUE_MAX_SIZE		300

//// 선형 큐 정의 ////
typedef struct _host {
	char name[QUEUE_MAX_SIZE][200];
	int f;
	int r;
} host1;

host1 host;

extern char title[250],comment[100000];

extern void add(char *hostname);
extern void del(char title[],char comment[]);
extern void init();

extern int separation(char *pathname, char *host_name);
extern void parse(char *filename,char *host_name,char *title,char *comment);
extern void search_thread(char *host_name,char *title,char *comment);
extern int mysql_data_insert(char *url, char *title, char *comment);
extern char list(char *filename,char *title,char *comment);
extern void again_search(char title[], char comment[]);
extern void again(char title[], char comment[]);
extern void s_thread(void *u_name);
