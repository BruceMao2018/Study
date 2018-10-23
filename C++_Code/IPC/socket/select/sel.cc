#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <errno.h>
#include <sys/socket.h>  
#include <arpa/inet.h>
#include <string.h>  //strerror
#include <stdlib.h>  //exit
#include <stdio.h>
#include <time.h>

using namespace std;
#define DEFAULT_PORT 8001
#define BACKLOG 10
#define MAXLINE 4096

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

int main(int argc, char **argv)
{
	int ser_fd;
    struct sockaddr_in servaddr;
    char    buff[4096];


	if((ser_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		cout << "create socket error: " << strerror(errno) << endl;
		exit(-1);
	}

/*##########in case of 'address already in use' error message######## */
	int yes = 1;
	if(setsockopt(ser_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)))
	{
		cout << "setsockopt failed" << strerror(errno) << endl;
		exit(-1);
	}


/*##########Set server socket to non-blocking######## */
	setSockNoBlock(ser_fd);

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

/*##########Add ser_fd into sets######## */
	fd_set readfds, readfds_bak;
	int maxfd;
	maxfd = ser_fd;
	FD_ZERO(&readfds);
	FD_ZERO(&readfds_bak);
	FD_SET(ser_fd, &readfds);
	FD_SET(ser_fd, &readfds_bak);

	cout << "ser_fd: " << ser_fd << endl;

	int connect_fd =0;
	struct sockaddr_in cliaddr;
	char ipStr[INET_ADDRSTRLEN];//INET_ADDRSTRLEN == 16, means the max lenths of IPv4 address

	while(1)
	{
		readfds = readfds_bak;//reset the readfds since the value will be changed after 'select'
		cout << "maxfd: " << maxfd << endl;

		struct timeval timeout = {2,0};
		cout << "timeout.second: " << timeout.tv_sec << " usec: " << timeout.tv_usec << endl;

		//here we do not need writefds & errorfds
		int res = select(maxfd+1, &readfds, NULL, NULL, &timeout);
		if(-1 == res)
		{
			cout << "select error: " << strerror(errno) << endl;
			exit(-1);
		}
		else if(0 == res)
		{
			cout << "No connections, continue ..." << endl;
			continue ;
		}
		
		//Get data
		for(int i = 0; i <= maxfd; i++)
		{
			if(!FD_ISSET(i, &readfds))
				continue;
			
			if(i == ser_fd)
			{
				socklen_t len = sizeof(cliaddr);//same as sizeof(struct sockaddr_in)
				connect_fd = accept(ser_fd, (struct sockaddr *)&cliaddr, &len);
				if(-1 == connect_fd)
				{
					cout << "accept error: " << strerror(errno) << endl;
					exit(-1);
				}
				cout << "connection from1: " << inet_ntoa(cliaddr.sin_addr) << endl;
				memset(ipStr, 0, sizeof(ipStr));
				cout << "connection from2: " << inet_ntop(AF_INET, &cliaddr.sin_addr, ipStr, INET_ADDRSTRLEN) << endl;

				/*##########Set connect socket to non-blocking######## */
				setSockNoBlock(connect_fd);


				/*##########Add connect socket to select for listening####### */
				cout << "add connect_fd into readfds: " << connect_fd << endl;
				if(connect_fd > maxfd) maxfd = connect_fd;
				FD_SET(connect_fd, &readfds_bak);
			}
			else
			{//in this case, the ISSET(i) == true, it means this is connected fd
				cout << "there is connection coming from client: " << i << endl;
				memset(buff, 0, sizeof(buff));
				int recvSize = recv(i, buff, sizeof(buff), 0);
				if(-1 == recvSize)
				{
					cout << "recv error: " << strerror(errno) << endl;
					exit(-1);
				}
				cout << "recv from fd: " << i << " messages:" << buff;
				//send same message back
				if(send(i, buff, strlen(buff), 0) == -1)
				{
					cout << "send error: " << strerror(errno) << endl;
					exit(-1);
				}
				//close connection
				if(close(i) == -1)
				{
					cout << "close error: " << strerror(errno) << endl;
					exit(-1);
				}
				//remove connected fd from readfds
				FD_CLR(i, &readfds_bak);
				if(maxfd == i)
				{//as maxfd will be removed, so, should decrease for next maxfd
					cout << "remove the maxfd from readfds" << endl;
					maxfd--;
				}
			}
		}
	}
	close(ser_fd);
	
	return 0;
}
