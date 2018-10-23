#include <iostream>
#include<errno.h>  
#include <signal.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

void fetch_signal(int);

int sigaction(int signum, const struct sigaction *act,
                     struct sigaction *oldact);
int main(int argc, char **argv)
{
	struct sigaction newact, oldact;

	newact.sa_handler = fetch_signal;
	newact.sa_flags = 0;
	sigemptyset(&newact.sa_mask);

	if(sigaction(SIGINT, &newact, &oldact) < 0)
	{
		cout << "sigaction error" << strerror(errno) << endl;
		exit(-1);
	}

//	while(1){}
	usleep(1000*1000*5);
	cout << "process to exit" << endl;

	return 0;
}

void fetch_signal(int signum)
{
	cout << "Get signal: " << signum << endl;
	usleep(1000*1000*15);
}
