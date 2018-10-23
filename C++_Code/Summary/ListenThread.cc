#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/resource.h> /*setrlimit */
#include "ListenThread.hh"
using namespace std;

#define BACKLOG 5
#define MAXEVENT 1000
#define MAXLEN 4096

ListenThread::ListenThread(ThreadPool *pool, CLog *log, int port)
{
	m_Pool = pool;
	m_Log = log;
	m_Port = port;
	Start();
}

ListenThread::~ListenThread()
{
	TRACE( "This is ListenThread Destruct ...");
}

void ListenThread::Start()
{
	//usleep(1000*100);
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_t id;
	int ret = 0;
	ret = pthread_create(&id, &attr, ListenFunction, (void*)this);
	if(ret != 0)
	{
		TRACE("Create Thread Error, ret: " << ret);
		throw Error_Info("Create Thread Error");
	}
	pthread_attr_destroy(&attr);
}

void *ListenFunction(void *p)
{
	ListenThread *pt = (ListenThread*)p;
	pt->HandleSocket();
}

void ListenThread::HandleSocket()
{
	//int port = m_Pool->m_Conf->GetListenPort();
	int serfd = Create_Bind_Socket(m_Port);
	if(listen(serfd, BACKLOG) == -1)
	{
		TRACE("listen error:" << serfd);
		close(serfd);
		pthread_exit(NULL);
	}
	
	int epfd;
	struct epoll_event event = {0,};
	struct epoll_event myEvents[MAXEVENT];
	memset(myEvents, 0, sizeof(myEvents));

	if((epfd = epoll_create(MAXEVENT)) < 0)
	{
		TRACE("listen error:" << serfd);
		close(serfd);
		pthread_exit(NULL);
	}

	event.events = EPOLLIN;
	event.data.fd = serfd;
	if((epoll_ctl(epfd, EPOLL_CTL_ADD, serfd, &event)) == -1) 
		HandleError("epoll_ctl error", serfd, epfd);

	while(1)
	{
		int num = 0;
		num = epoll_wait(epfd, myEvents, MAXEVENT, -1);
		if(num == -1) HandleError("epoll wait error", serfd, epfd);

		for( int i = 0; i < num; i++)
		{
			if(myEvents[i].data.fd == serfd)
			{//Get new connection from Client
				int confd = AcceptConnection(serfd);
				setSockNoBlock(confd);
				event.events = EPOLLIN;
				event.data.fd = confd;

				if(epoll_ctl(epfd, EPOLL_CTL_ADD, confd, &event) == -1)
					HandleError("epoll_ctl error", serfd, epfd, confd);
			}
			else if(myEvents[i].events == EPOLLIN)
			{
				char buff[MAXLEN];
				char ResBuff[MAXLEN];
				bzero(buff, sizeof(buff));
				bzero(ResBuff, sizeof(ResBuff));
				strcpy(buff, ReadData(myEvents[i].data.fd));
				if(strncmp(buff, "SOCKETOVER", 10) == 0)
				{
					//connection closed
					if(epoll_ctl(epfd, EPOLL_CTL_DEL, myEvents[i].data.fd, &event) == -1)//delete the fd once get data
						HandleError("epoll_ctl error", serfd, epfd, myEvents[i].data.fd);
					close(myEvents[i].data.fd);
					continue;
				}
		
				TRACE("Get data: [" << buff << "]");
				if(m_Pool->AddTask(buff))
					sprintf(ResBuff, "Your Messages [%s] has been accepted", buff);
				else
					sprintf(ResBuff, "Your Messages [%s] has been rejected", buff);
				if(send(myEvents[i].data.fd, ResBuff, strlen(ResBuff),0) == -1)
					HandleError("socket send error", serfd, epfd, myEvents[i].data.fd);

				//if(epoll_ctl(epfd, EPOLL_CTL_DEL, myEvents[i].data.fd, &event) == -1)//delete the fd once get data
					//HandleError("epoll_ctl error", serfd, epfd, myEvents[i].data.fd);
		
				//close(myEvents[i].data.fd);
			}
		}
	}
	close(serfd);
	close(epfd);
}

int ListenThread::Create_Bind_Socket(int p)
{
	struct sockaddr_in seraddr;
	int port = p;
	int serfd =0;
	
	bzero(&seraddr, sizeof(struct sockaddr_in));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(port);
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY means Get Local IP automaticly

	if((serfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		HandleError("create socket error");
	
	setSockNoBlock(serfd);
    if(bind(serfd, (struct sockaddr*)&seraddr, sizeof(struct sockaddr)) == -1)
		HandleError("bind socket error", serfd);

	return serfd;
}
	
int ListenThread::AcceptConnection(int serfd)
{
	struct sockaddr_in cliaddr;
	memset(&cliaddr, 0, sizeof(struct sockaddr_in));
	socklen_t len = sizeof(struct sockaddr_in);
	int confd = accept(serfd, (struct sockaddr *)&cliaddr, &len);
	if(confd == -1)
		HandleError("accept serfd error:", serfd);

	TRACE("get connection from: " << inet_ntoa(cliaddr.sin_addr));

	return confd;
}

void ListenThread::HandleError(const char *msg, int serfd , int epfd , int clifd)
{
	TRACE("Get error:" << msg << "--" << strerror(errno));
	if(serfd != 0) close(serfd);
	if(epfd != 0) close(epfd);
	if(clifd != 0) close(clifd);
	pthread_exit(NULL);
}
void ListenThread::setSockNoBlock(int fd)
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

char* ListenThread::ReadData(int fd)
{
	char buff[MAXLEN];
	bzero(buff, sizeof(buff));

	int recvSize = recv(fd, buff, sizeof(buff), 0);
	if(-1 == recvSize )
	{
		cout << "recv error: " << strerror(errno) << endl;
		exit(-1);
	}
	else if(0 == recvSize && errno == EAGAIN)
	{
		cout << "data not ready, try again" << endl;
		strcpy(buff, "SOCKETTRYAGAIN");
	}
	else if(0 == recvSize)
	{
		cout << "connection closed by peer, fd=" << fd << endl;
		strcpy(buff, "SOCKETOVER");
		//exit(-1);//we can not exit, since we need to handle other Accept
	}
	else
	{
		struct sockaddr_in cliaddr;
		socklen_t addrlen;
		memset(&cliaddr, 0, sizeof(cliaddr));
		getpeername(fd, (struct sockaddr *)&cliaddr, &addrlen);
		TRACE("recv total " << recvSize << " size msg from fd: " << fd << " IP:" << inet_ntoa(cliaddr.sin_addr) << " Port:" << ntohs(cliaddr.sin_port) << " messages:[" << buff << "]");
		return buff;
	}
	return buff;
}
