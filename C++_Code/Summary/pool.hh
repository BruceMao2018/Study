#ifndef __POOL_HH
#define __POOL_HH

#include <iostream>
#include <pthread.h>
#include <list>
#include <vector>
#include <deque>
#include <stdexcept>
#include <stdlib.h>
#include "Job.hh"
#include "Log.hh"
#include "Configure.hh"
#include "ListenThread.hh"
using namespace std;

void *MonitorFunction(void *p);


class ThreadPool
{
public:
	ThreadPool(CLog *log, Config *conf);
	~ThreadPool();

	void CreateListenThreads(int NumOfListen = 5);
	void CreateTaskThreads(int NumOfThreads);

	bool AddTask(BaseJob *job);
	bool AddTask(const char *job);
	void CreateMonitorThreads();
	void Monitor();
	void HandleTasks();
	void HandleListen();

	void TerminalAll(){m_TaskEnd = true;}


	Config *m_Conf;//set it as public, so, the class ListenThread can access it
private:
	pthread_t ThreadId[20];
	pthread_mutex_t job_mutex;
	pthread_mutex_t pool_mutex;

	deque <BaseJob *> m_Tasks;
	deque <string> m_Test_Tasks;
	//pthread_cond_t cond;
	int m_MaxThreads;
	int m_CurThreads;
	int m_InitThreads;
	//bool full;
	bool m_TaskEnd;
	int m_DesThreads;
	long int m_MaxTaskAllow;

	CLog *m_Log;
};
#endif
