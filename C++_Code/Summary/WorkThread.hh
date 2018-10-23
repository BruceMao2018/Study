#ifndef __WORKTHREAD__HH
#define __WORKTHREAD__HH

#include "Job.hh"
#include "Error_Info.hh"
#include "Log.hh"
#include "pool.hh"
using namespace std;

void *ThreadFunction(void*);

class WorkThread
{
public:
	WorkThread(ThreadPool *);
	WorkThread(ThreadPool *, CLog *);
	~WorkThread();
	void Start();//create threads here !
	void Run();//run it in a loop, call back to ThreadPool::HandleTasks();
	//void Terminal();
	bool GetExit(){return m_ExitFlag;}
	void SetExit(bool flag){m_ExitFlag = flag;}
	void ThreadLock() {pthread_mutex_lock(&m_WorkThread_mutex);}
	void ThreadUnLock() {pthread_mutex_unlock(&m_WorkThread_mutex);}
private:
	pthread_mutex_t m_WorkThread_mutex;
	bool m_ExitFlag;

	ThreadPool *m_Pool;
	CLog *m_Log;
};
#endif
