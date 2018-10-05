#include "WorkThread.hh"
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
using namespace std;

WorkThread::WorkThread(ThreadPool *pool)
{
	m_Pool = pool;
	m_ExitFlag = false;
	pthread_mutex_init(&m_WorkThread_mutex, NULL);
	Start();
}

WorkThread::WorkThread(ThreadPool *pool, CLog *log)
{
	m_Pool = pool;
	m_ExitFlag = false;
	m_Log = log;
	pthread_mutex_init(&m_WorkThread_mutex, NULL);
	Start();
}

WorkThread::~WorkThread()
{
	TRACE( "This is WorkThread Destruct ...");
}

void WorkThread::Start()
{
	usleep(1000*100);
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_t id;
	int ret = 0;
	ret = pthread_create(&id, &attr, ThreadFunction, (void*)this);
	if(ret != 0)
	{
		cout << "Create Thread Error, ret: " << ret << endl;
		throw Error_Info("Create Thread Error");
	}
	pthread_attr_destroy(&attr);
}

void *ThreadFunction(void *p)
{
	WorkThread *pt = (WorkThread*)p;
	pt->Run();
}

void WorkThread::Run()
{
	m_Pool->HandleTasks();
	//cout << "delete this here" << endl;
	//delete this;//free the memory here
}
