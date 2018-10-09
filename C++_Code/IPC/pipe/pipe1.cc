#include <unistd.h>
#include <stdio.h>

#define MAXLINE 1024

int main(void)
{
	int n;
	int fd[2];
	pid_t pid;
	char line[MAXLINE];

	if (pipe(fd) < 0)
    	printf("pipe error\n");
	if ((pid = fork()) < 0)
	{
		printf("fork error\n");
	}
	else if (pid > 0)
	{           /* parent */
		close(fd[0]);
		write(fd[1], "hello world\n", 12);              /* write data to fd[1] */
	}
	else
	{                                        /* child */
		close(fd[1]);
		n = read(fd[0], line, MAXLINE);                 /* read data from fd[0] */
		write(STDOUT_FILENO, line, n);                  /* write data to standard output */
	}

	return (0);
}
