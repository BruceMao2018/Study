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

int main(int argc, char **argv)
{
	int ser_fd;
    struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;
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

/*##########Set socket to be non-blocking######## */
    int flags = fcntl(ser_fd, F_GETFL, 0);
	if(flags < 0)
	{
		cout << "F_GETFL error: " << strerror(errno) << endl;
		exit(-1);
	}
    if(fcntl(ser_fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		cout << "fcntl SETFL error: " << strerror(errno) << endl;
		exit(-1);
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
//set timeout for sending and recving
struct timeval timeout = { 3, 0};
//set timeout for sending
setsockopt(ser_fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(struct timeval));
//set timeout for recving
setsockopt(ser_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval));

	int connect_fd;
	socklen_t len = sizeof(struct sockaddr);
	while(1)
	{
		memset(&cliaddr, 0, sizeof(struct sockaddr_in));
		len = sizeof(struct sockaddr_in);//sizeof(sockaddr_in) == sizeof(sockaddr)
 		connect_fd = accept(ser_fd, (struct sockaddr*)&cliaddr, &len);
		if(connect_fd == -1 && errno == EAGAIN)
		{
			cout << "no client connection coming" << endl;
			usleep(1000*1000*1);
			continue ;
		}
		else if(connect_fd == -1)
		{
			cout << "accept error" << endl;
			exit(-1);
		}

		//Get connection here
		cout << "Get connection from: " << inet_ntoa(cliaddr.sin_addr) << endl;

while(1)
{
		memset(buff, 0, sizeof(buff));
  		//n = recv(ser_fd, buff, MAXLINE, MSG_DONTWAIT);
  		//n = recv(connect_fd, buff, MAXLINE, MSG_WAITALL);
  		int n = recv(connect_fd, buff, MAXLINE, 0);
		if(n <= 0)
		{
			if(0 == n)
			{
				cout << "connection closed by peer" << endl;
				break;//go back accept, not recv
			}
			else if(errno == EINTR) // n < 0 && errno == EINTR
			{
				cout << "Get Interupt" << endl;
				continue;
			}
			else if(errno == EAGAIN) //no data now, you should read it again
			{
				cout << "No data now, let us try it again" << endl;
				continue;
			}
			else //n == -1 && errno != EINTR
			{
				cout << "recv error" << endl;
				exit(-1);
			}
		}

		//Get data from client
    	printf("recv msg from client: %s", buff);
        if(send(connect_fd, "Hello,you are connected!", 26,0) == -1)
        {
			perror("send error");
        	close(connect_fd);
        	exit(0);
    	}
    	close(connect_fd);
		break;//go back accept
}
	}
	close(ser_fd);
	
	return 0;
}
