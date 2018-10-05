#ifndef __WORKTHREAD__HH
#define __WORKTHREAD__HH

//#include "ThreadPool.hh"
#include "Job.hh"
//#include <pthread.h>
#include "Error_Info.hh"
#include "Log.hh"
using namespace std;

void *ThreadFunction(void*);
class CThreadPool;

class CWorkThread
{
public:
	CWorkThread(CThreadPool *pool);
	CWorkThread(CThreadPool *pool, CLog *log);
	~CWorkThread();
	void Start();//create threads here !
	void Run();//run it in a loop
	void Terminal();
	void AddNewJob(CJob *job);
	CThreadPool *m_Pool;
	CJob *m_Job;
	bool GetExit(){return m_ExitFlag;}
	void SetExit(bool flag){m_ExitFlag = flag;}
	//bool WriteLog(ostream &s){return m_Log->WriteLog(s);};
private:
	pthread_cond_t m_Work_cond;
	pthread_mutex_t m_Work_mutex;

	bool m_ExitFlag;

	CLog *m_Log;
};
#endif
