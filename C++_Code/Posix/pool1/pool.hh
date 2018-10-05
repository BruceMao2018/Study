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
using namespace std;

void *MonitorFunction(void*p);


class ThreadPool
{
public:
	ThreadPool(CLog *log, int Max, int Min);
	~ThreadPool();
	//int GetMaxThreads() {return MaxThreads;}
	//int GetMinThreads() {return MinThreads;}
	//int GetCurThreads() {return CurThreads;}
	void CreateThreads(int NumOfThreads);
	bool AddTask(BaseJob *job);
	void CreateMonitorThreads();
	void Monitor();
	void HandleTasks();
	int DesThreads;
	deque <BaseJob *> tasks;

	void TerminalAll(){TaskEnd = true;}


private:
	pthread_t ThreadId[20];
	pthread_mutex_t job_mutex;
	pthread_mutex_t pool_mutex;
	//pthread_cond_t cond;
	int MaxThreads;
	int CurThreads;
	int MinThreads;
	//bool full;
	bool TaskEnd;
	long int MaxTaskAllow;

	CLog *m_Log;
};

#endif
