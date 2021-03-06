/* File Name: client.c */  
  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <iostream>
#include "time.hh"
using namespace std;
  
#define MAXLINE 4096  
#define DEFAULT_PORT 8001
  
int main(int argc, char** argv)  
{  
    int    sockfd, n,rec_len;  
    char    recvline[4096], sendline[4096];  
    char    buf[MAXLINE];  
    struct sockaddr_in    servaddr;  
  
  
    if(argc != 2)
	{  
    	printf("usage: ./client <ipaddress>\n");  
    	exit(0);  
    }  
  
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
    	printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);  
    	exit(0);  
    }  
  
  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(DEFAULT_PORT);  
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
	{
    	printf("inet_pton error for %s\n",argv[1]);  
    	exit(0);  
    }  
  
  
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
    	printf("connect error: %s(errno: %d)\n",strerror(errno),errno);  
    	exit(0);  
    }  
  
  
	long int max = 50000;
	int i = 0;
cout << "Starting time: " << sysUsecTime() << endl;
while(i < max)
{
	memset(sendline, 0, sizeof(sendline));
	sprintf(sendline, "This is testing data %d", ++i);
    //printf("send msg to server: %s\n", sendline);  
    if(send(sockfd, sendline, strlen(sendline), 0) < 0)  
    {  
    	printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
    	exit(0);  
    }
    if((rec_len = recv(sockfd, buf, MAXLINE,0)) == -1)
	{
       perror("recv error");  
       exit(1);  
    }
    buf[rec_len]  = '\0';  
    //printf("Received : %s\n",buf);
}
cout << "Ending time: " << sysUsecTime() << endl;
    close(sockfd);
    exit(0);  
}
