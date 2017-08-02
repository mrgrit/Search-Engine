all : 
	gcc -o main main.c parse.c mysql.c queue.c -lpthread -lmysqlclient -I/usr/local/mysql/include/mysql -L/usr/local/mysql/lib/mysql

clean :
	rm -f main
