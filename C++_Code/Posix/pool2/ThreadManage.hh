#ifndef __THREAD_MANAGE__HH
#define __THREAD_MANAGE__HH
#include "Job.hh"
#include "ThreadPool.hh"
#include "Configure.hh"
#include "Log.hh"

class CThreadManage
{
public:
	CThreadManage();
	CThreadManage(int num);
	CThreadManage(Config *conf, CLog *log);
	void Run(CJob *job);
	void TerminalAll(void);
	virtual ~CThreadManage();
	//bool WriteLog(ostream &s){return m_Log->WriteLog(s);};
private:
	CThreadPool *m_pool;
	int m_NumOfThreads;
	CLog *m_Log;
};

#endif
