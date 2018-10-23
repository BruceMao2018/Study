#include<stdio.h>
#include<signal.h>
#include <unistd.h>
int main()
{
	signal(SIGINT,SIG_DFL);
	int i;
	for( i = 0; i<10;++i)
	{
		printf("hello world\n");
		sleep(1);
	}
	return 0;
}
