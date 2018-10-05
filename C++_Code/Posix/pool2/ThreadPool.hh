#ifndef __ThreadPool__HH
#define __ThreadPool__HH
#include "WorkThread.hh"
#include "Job.hh"
#include "Configure.hh"
#include <vector>
#include "Log.hh"

void *MonitorFunction(void *p);

class CThreadPool
{
public:
	CThreadPool();
	CThreadPool(int num);
	CThreadPool(Config *conf, CLog *log);
	virtual ~CThreadPool();

	void Run(CJob *job);
	void TerminalAll(void);

	void AppenToIdleList(CWorkThread *newthread);
	void MoveToBusyList(CWorkThread *idlethread);
	void MoveToIdleList(CWorkThread *busythread);
	void DeleteIdleThread(int num);
	void CreateIdleThread(int num);
	CWorkThread *GetIdleThread(void);

	vector <CWorkThread *> m_ThreadList;
	vector <CWorkThread *> m_BusyList;
	vector <CWorkThread *> m_IdleList;
	pthread_mutex_t m_All_mutex;
	pthread_mutex_t m_Busy_mutex;
	pthread_mutex_t m_Idle_mutex;
	pthread_cond_t m_Pool_cond;
	pthread_mutex_t m_Pool_mutex;

	int GetBusyNum();
	int GetAllNum();
	int GetIdleNum();
	void Monitor();
	void CreateMonitorThread();

	void SetMonitorExitFlag(bool flag){m_MonitorExitFlag=flag;}
	bool GetMonitorExitFlag(){return m_MonitorExitFlag;}
	//string GetLogName(){return m_LogName;}
	//bool WriteLog(ostream &s){return m_Log->WriteLog(s);};
private:
	unsigned int m_MaxNum;
	unsigned int m_AvailLow;
	unsigned int m_AvailMax;
	unsigned int m_InitNum;
	unsigned int m_AvailNum;
	//string m_LogName;

	bool m_MonitorExitFlag;
	
	CLog *m_Log;

};
#endif
