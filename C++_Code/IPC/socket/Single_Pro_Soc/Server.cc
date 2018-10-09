/* File Name: server.c */  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h>  
#include <iostream>
#include <arpa/inet.h>
using namespace std;

#define DEFAULT_PORT 8001
#define MAXLINE 4096  
#define BACKLOG 10
int main(int argc, char** argv)  
{  
    int    ser_fd, connect_fd;  
    struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;

    char    buff[4096];
    int     n;  
 	if((ser_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{  
    	printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);  
    	exit(0);  
    }  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY means Get Local IP automaticly
    servaddr.sin_port = htons(DEFAULT_PORT);
  
    if(bind(ser_fd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr)) == -1)
	{
    	printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);  
    	exit(0);  
    }
 
    if(listen(ser_fd, BACKLOG) == -1)
	{  
    	printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);  
    	exit(0);  
    }  
    printf("======waiting for client's request======\n");  
    while(1)
	{ 
		memset(&cliaddr, 0, sizeof(cliaddr));
		socklen_t len = sizeof(struct sockaddr);
 		if((connect_fd = accept(ser_fd, (struct sockaddr*)&cliaddr, &len)) == -1)
		{
        	printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
        	//continue;
        	exit(-1);
    	}
		cout << "Get connection from: " << inet_ntoa(cliaddr.sin_addr) << endl;
		memset(buff, 0, sizeof(buff));
  		//n = recv(ser_fd, buff, MAXLINE, MSG_DONTWAIT);
  		//n = recv(connect_fd, buff, MAXLINE, MSG_WAITALL);
  		n = recv(connect_fd, buff, MAXLINE, 0);
    	printf("recv msg from client: %s", buff);
        if(send(connect_fd, "Hello,you are connected!", 26,0) == -1)
        {
			perror("send error");
        	close(connect_fd);
        	exit(0);
    	}
    	close(connect_fd);
    }
    close(ser_fd);  
}
