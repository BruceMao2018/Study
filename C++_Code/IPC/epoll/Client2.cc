#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h>  
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
using namespace std;
  
#define MAXLINE 4096  
#define DEFAULT_PORT 8001
#define DEFAULT_IP "192.168.1.106"
  
  
//int SendMsgToServer(const char* ipAddr, const int port, const char* msg)
main()
{
    int    sockfd, n,rec_len;
    char    recvline[MAXLINE];
    char    buf[MAXLINE];
    struct sockaddr_in    servaddr;
	char msg[1024];
int count = 5000;
int i = 0;
while(i++ < count)
{
	memset(msg, 0, sizeof(msg));
	strcpy(msg, "this is testing data");
	sprintf(msg, "%s: %d", msg, i);

  
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{  
    	printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);  
	break;
    }  
  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(DEFAULT_PORT);
	//inet_aton(argv[1], &servaddr.sin_addr);
    if(inet_pton(AF_INET, DEFAULT_IP, &servaddr.sin_addr) <= 0)
	{
    	printf("inet_pton error");
	break;
    }
  
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
    	printf("connect error: %s(errno: %d)\n",strerror(errno),errno);  
	break;
    }  
  
	cout << "sending:[" << msg << "], length: " << strlen(msg) << endl;
    if(send(sockfd, msg, strlen(msg), 0) < 0)
    {
    	printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
	break;
    }
usleep(1000);
}

while(1){}
	return 0;
}

/*
int main(int argc, char **argv)
{
	char msg[MAXLINE];
	int i = 0;
	memset(msg, 0, sizeof(msg));

	if(argc != 2)
	{
		cout << "parameter error" << endl;
		return -1;
	}
	//cout << "Keyin the msg:" << endl;
	//getline(cin, line);
	
while(i++ < 100)
{
	sprintf(msg, "%s%d", "This is A", i);
	SendMsgToServer(argv[1], DEFAULT_PORT, msg);
}
	return 0;
}
*/
