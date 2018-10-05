#include "ThreadManage.hh"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "MyJob.hh"
#include "Configure.hh"
#include "time.hh"
using namespace std;

int main(int argc, char **argv)
{
	Config *conf = new Config();
	conf->SetMax(50);
	conf->SetInit(20);
	conf->SetAvaLow(5);
	conf->SetAvaMax(25);
	conf->SetLogName("/home/mcadmin/Github/C++_Code/Posix/pool2/log");
	
	string FileName("/home/mcadmin/Github/C++_Code/Posix/pool2/MyJob.log");
	CLog *m_Log = new CLog(FileName);

	CThreadManage *manage = new CThreadManage(conf, m_Log);

	//usleep(1000*1000*1);
	string LocalTime = sysUsecTime();
	TRACE(LocalTime);

try{
int n = 1000;
while(n--)
{
int num = 5000;
for(int i = 0; i < num; i++)
{
	MyJob *myjob1 = new MyJob(m_Log);
	char buf[100];
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%s %d", "This is Job: ", i);
	string s1(buf);
	myjob1->KeyInData(s1);
	manage->Run(myjob1);
}
	

	usleep(1000*100);

for(int i = num; i < num+num; i++)
{
	MyJob *myjob1 = new MyJob(m_Log);
	char buf[100];
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%s %d", "This is Job: ", i);
	string s1(buf);
	myjob1->KeyInData(s1);
	manage->Run(myjob1);
}
	usleep(1000*10);
}
}catch (Error_Info &e)
{
	cout << "Error: " << e.what() << endl;
}
	manage->TerminalAll();


	//LocalTime.clear();
	LocalTime = sysUsecTime();
	TRACE(LocalTime);
	

	delete conf;
	conf = NULL;

usleep(1000*1000*1); // we have to wait several seconds, since some threads is still busy with writing log
// otherwise, it will cause core dump
	delete m_Log;
	m_Log = NULL;

	pthread_exit(NULL);

	return 0;
}
