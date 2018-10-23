#include<signal.h>
#include<stdio.h>
#include<signal.h>
#include <unistd.h>
#include <stdlib.h>
void f(int);

int main()
{         
        void f(int);      
        signal(SIGINT,f); 
        int i;
        for( i = 0; i<10;++i)
        {
                printf("hello world\n");
                sleep(1); 
        }
        return 0;
}   
    
void f(int signum)
{   
        printf("get signal: %d\n", signum);
}
