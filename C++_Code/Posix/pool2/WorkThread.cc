#include "WorkThread.hh"
#include "ThreadPool.hh"
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
using namespace std;

CWorkThread::CWorkThread(CThreadPool *pool)
{
	m_Pool = pool;
	m_Job = NULL;
	m_ExitFlag = false;
	pthread_mutex_init(&m_Work_mutex, NULL);
	pthread_cond_init(&m_Work_cond, NULL);
}

CWorkThread::CWorkThread(CThreadPool *pool, CLog *log)
{
	m_Pool = pool;
	m_Job = NULL;
	m_ExitFlag = false;
	m_Log = log;
	pthread_mutex_init(&m_Work_mutex, NULL);
	pthread_cond_init(&m_Work_cond, NULL);
}

CWorkThread::~CWorkThread()
{
	TRACE( "This is CWorkThread Destruct ...");
}

void CWorkThread::Start()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_t id;
	if(pthread_create(&id, &attr, ThreadFunction, (void*)this))
	{
		//TRACE( "Create Thread Error");
		throw Error_Info("Create Thread Error");
	}
	pthread_attr_destroy(&attr);
}

void *ThreadFunction(void *p)
{
	CWorkThread *pt = (CWorkThread*)p;
	pt->Run();
}

void CWorkThread::Run()
{
	for(;;)
	{
		//TRACE( "Thread: " << pthread_self() << " is Running");
		
		pthread_mutex_lock(&m_Work_mutex);
		while(m_Job == NULL && !GetExit())
		{
			//TRACE( "Thread: " << pthread_self() << " go to sleep");
			pthread_cond_wait(&m_Work_cond, &m_Work_mutex);
		}
		if(m_Job == NULL) //it means GetExit = True;
		{
			//throw Error_Info("Terminal Disabled, impossible to here");
			break;
		}

		//TRACE( "Thread: " << pthread_self() << " start to work");
		m_Job->Run();
		pthread_mutex_unlock(&m_Work_mutex);

		delete m_Job;
		m_Job = NULL;
		m_Pool->MoveToIdleList(this);
	}

	TRACE( "exit !!!");
	pthread_exit(NULL);
}

void CWorkThread::Terminal()
{
	pthread_mutex_lock(&m_Work_mutex);
	SetExit(true);
	pthread_cond_signal(&m_Work_cond);//wake up the threads
	pthread_mutex_unlock(&m_Work_mutex);
}

void CWorkThread::AddNewJob(CJob *job)
{
	pthread_mutex_lock(&m_Work_mutex);
	m_Job = job;
	pthread_cond_signal(&m_Work_cond);
	pthread_mutex_unlock(&m_Work_mutex);
}
