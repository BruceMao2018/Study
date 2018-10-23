#include <stdio.h>  
#include <time.h>  
#include <sys/time.h>  
#include <iostream>
#include <cstring>
using namespace std;

string sysLocalTime()
{
    time_t timesec;
    struct tm *p;

    time(&timesec);
    p = localtime(&timesec);

	char timebuf[256];
	memset(timebuf, 0, sizeof(timebuf));
	sprintf(timebuf, "%d-%02d-%02d %02d:%02d:%02d", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	string MyTime(timebuf);

	return MyTime;
}

string sysUsecTime()
{  
    struct timeval    tv;
    struct timezone tz;  

    struct tm         *p;  

    gettimeofday(&tv, &tz);  
    //printf("tv_sec:%ld\n",tv.tv_sec);  
    //printf("tv_usec:%ld\n",tv.tv_usec);  
    //printf("tz_minuteswest:%d\n",tz.tz_minuteswest);  
    //printf("tz_dsttime:%d\n",tz.tz_dsttime);  

	char timebuf[256];
	memset(timebuf, 0, sizeof(timebuf));	
    p = localtime(&tv.tv_sec);  
    sprintf(timebuf, "%d-%02d-%02d %02d:%02d:%02d.%3ld", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec);
	string UserTime(timebuf);
	return UserTime;
}

/*
int main()
{
	string MyLocalTime = sysLocalTime();
	cout << "MyLocalTime: " << MyLocalTime << endl;
	string UserTime = sysUsecTime();
	cout << "UserTime: " << UserTime << endl;

	return 0;
}
*/
