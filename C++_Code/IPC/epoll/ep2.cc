#include <iostream>
#include <sys/epoll.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/resource.h> /*setrlimit */

using namespace std;

#define BACKLOG 10
#define MAXEVENT 5000
#define MAXLEN 4096

int Create_Bind_socket(char *p);
int AcceptConnection(int serfd);
void HandleError(const char *msg, int serfd = 0, int epfd = 0, int clifd = 0);
void setSockNoBlock(int fd);
char *ReadData(int fd);

int main(int argc, char **argv)
{
	int serfd;
	int epfd;

	if(argc != 2)
	{
		cout << "Parameter error" << endl;
		exit(-1);
	}

/*
	struct rlimit rt;

    rt.rlim_max = rt.rlim_cur = MAXEVENT;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1) 
    {
        perror("setrlimit error");
        return -1;
    }
*/



	//argv[1] - port No.
	serfd = Create_Bind_socket(argv[1]);
	if(listen(serfd, BACKLOG) == -1) HandleError("Listen Error", serfd);

	struct epoll_event myEvent = {0,};
	struct epoll_event myEvents[MAXEVENT];
	memset(myEvents, 0, sizeof(myEvents));
	if((epfd = epoll_create(MAXEVENT)) < 0) HandleError("epoll_create error", serfd);

	myEvent.events = EPOLLIN;
	myEvent.data.fd = serfd;

	if((epoll_ctl(epfd, EPOLL_CTL_ADD, serfd, &myEvent)) == -1) HandleError("epoll_ctl error", serfd, epfd);

	struct timeval timeout = {2,0};
	while(1)
	{
		int num = 0;
		num = epoll_wait(epfd, myEvents, MAXEVENT, -1);
		if(num == -1) HandleError("epoll wait error", serfd, epfd);

		for( int i = 0; i < num; i++)
		{
			//bzero(myEvent, sizeof(myEvent));

			if(myEvents[i].data.fd == serfd)
			{//Get new connection from Client
				int confd = AcceptConnection(serfd);
				setSockNoBlock(confd);
				myEvent.events = EPOLLIN;
				myEvent.data.fd = confd;

				if(epoll_ctl(epfd, EPOLL_CTL_ADD, confd, &myEvent) == -1) HandleError("epoll_ctl error", serfd, epfd, confd);
			}
			else if(myEvents[i].events == EPOLLIN)
			{
				char buff[MAXLEN];
				bzero(buff, sizeof(buff));
				strcpy(buff, ReadData(myEvents[i].data.fd));
				cout << "Get data: [" << buff << "]" << endl;
				//send same data back
				//	...
				//	...

				epoll_ctl(epfd, EPOLL_CTL_DEL, myEvents[i].data.fd, &myEvent);//delete the fd once get data
				close(myEvents[i].data.fd);
			}
		}
	}
	close(serfd);
	close(epfd);
}

int Create_Bind_socket(char *p)
{
	struct sockaddr_in seraddr;
	int port = atoi(p);
	int serfd;
	
	bzero(&seraddr, sizeof(struct sockaddr_in));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(port);
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY means Get Local IP automaticly

	if((serfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		cout << "create socket error, " << strerror(errno) << endl;
		exit(-1);
	}
	
	setSockNoBlock(serfd);
    if(bind(serfd, (struct sockaddr*)&seraddr, sizeof(struct sockaddr)) == -1)
	{
		cout << "bind socket error, " << strerror(errno) << endl;
		exit(-1);
	}

	return serfd;
}
	
int AcceptConnection(int serfd)
{
	struct sockaddr_in cliaddr;
	memset(&cliaddr, 0, sizeof(struct sockaddr_in));
	socklen_t len = sizeof(struct sockaddr_in);
	int confd = accept(serfd, (struct sockaddr *)&cliaddr, &len);
	if(confd == -1)
	{
		cout << "accept error:" << strerror(errno) << endl;
		exit(-1);
	}

	cout << "get connection from: " << inet_ntoa(cliaddr.sin_addr) << endl;

	return confd;
}

void HandleError(const char *msg, int serfd , int epfd , int clifd)
{
	cout << "Get error: " << msg << strerror(errno) << endl;
	if(serfd != 0) close(serfd);
	if(epfd != 0) close(epfd);
	if(clifd != 0) close(clifd);
	exit(-1);
}
void setSockNoBlock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
	if(flags < 0)
	{
		cout << "F_GETFL error: " << strerror(errno) << endl;
		exit(-1);
	}
    if(fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		cout << "fcntl SETFL error: " << strerror(errno) << endl;
		exit(-1);
	}
}

char *ReadData(int fd)
{
	char buff[MAXLEN];
	bzero(buff, sizeof(buff));

	int recvSize = recv(fd, buff, sizeof(buff), 0);
	if(-1 == recvSize)
	{
		cout << "recv error: " << strerror(errno) << endl;
		exit(-1);
	}
	else if(0 == recvSize)
	{
		cout << "connection closed by peer, fd=" << fd << endl;
		buff[0] = 0;
	}
	else
	{
		struct sockaddr_in cliaddr;
		socklen_t addrlen;
		memset(&cliaddr, 0, sizeof(cliaddr));
		getpeername(fd, (struct sockaddr *)&cliaddr, &addrlen);
		cout << "recv total " << recvSize << " size msg from fd: " << fd << " IP:" << inet_ntoa(cliaddr.sin_addr) << " Port:" << ntohs(cliaddr.sin_port) << " messages:[" << buff << "]" << endl;
		return buff;
	}
}
