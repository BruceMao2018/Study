#include <signal.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

using namespace std;

void fetch_signal(int signmum);

int main(int argc, char **argv)
{

	struct sigaction newact, oldact;
	
	sigemptyset(&newact.sa_mask);
	sigaddset(&newact.sa_mask, SIGQUIT);//SIGQUIT = ctrl + \
	//sigaddset(&newact.sa_mask, SIGTSTP);
	newact.sa_flags = 0;
	newact.sa_handler = fetch_signal;

	if(sigaction(SIGINT, &newact, &oldact) < 0)
	{
		cout << "sigaction error: " << strerror(errno) << endl;
		exit(-1);
	}

	while(1) {}

	return 0;
}

void fetch_signal(int signum)
{
	cout << "Get signal: " << signum << endl;
	usleep(1000*1000*5);
}
