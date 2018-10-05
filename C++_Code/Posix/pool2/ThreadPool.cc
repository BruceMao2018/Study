#include "ThreadPool.hh"
#include "Error_Info.hh"
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;

CThreadPool::CThreadPool()
{
	m_AvailNum = 0;
	m_MaxNum = 50;
	m_InitNum = 10;
	m_AvailLow = 5;
	m_AvailMax = 20;
	m_ThreadList.clear();
	m_BusyList.clear();
	m_IdleList.clear();
	m_MonitorExitFlag=false;
	//m_LogName = "/home/mcadmin/Github/C++_Code/Posix/pool2/log";
	pthread_mutex_init(&m_All_mutex, NULL);
	pthread_mutex_init(&m_Busy_mutex, NULL);
	pthread_mutex_init(&m_Idle_mutex, NULL);
	pthread_cond_init(&m_Pool_cond, NULL);
	pthread_mutex_init(&m_Pool_mutex, NULL);

	CreateIdleThread(m_InitNum);
	CreateMonitorThread();
}

CThreadPool::CThreadPool(int num)
{
	m_AvailNum = 0;
	m_MaxNum = 50;
	//m_InitNum = 10;
	m_InitNum = (num>m_MaxNum)?m_MaxNum:num ;
	m_AvailLow = 5;
	m_AvailMax = 20;

	m_ThreadList.clear();
	m_BusyList.clear();
	m_IdleList.clear();
	m_MonitorExitFlag=false;
	//m_LogName = "/home/mcadmin/Github/C++_Code/Posix/pool2/log";
	pthread_mutex_init(&m_All_mutex, NULL);
	pthread_mutex_init(&m_Busy_mutex, NULL);
	pthread_mutex_init(&m_Idle_mutex, NULL);
	pthread_cond_init(&m_Pool_cond, NULL);
	pthread_mutex_init(&m_Pool_mutex, NULL);

	CreateIdleThread(m_InitNum);
	CreateMonitorThread();
}

CThreadPool::CThreadPool(Config *conf, CLog *log)
{
	m_AvailNum = 0;
	m_MaxNum = conf->GetMax();
	m_InitNum = conf->GetInit();
	m_AvailLow = conf->GetAvailLow();
	m_AvailMax = conf->GetAvailMax();
	//m_LogName = conf->GetLogName();
	m_ThreadList.clear();
	m_BusyList.clear();
	m_IdleList.clear();
	m_MonitorExitFlag=false;
	m_Log = log;
	pthread_mutex_init(&m_All_mutex, NULL);
	pthread_mutex_init(&m_Busy_mutex, NULL);
	pthread_mutex_init(&m_Idle_mutex, NULL);
	pthread_cond_init(&m_Pool_cond, NULL);
	pthread_mutex_init(&m_Pool_mutex, NULL);

	CreateIdleThread(m_InitNum);
	CreateMonitorThread();
}

void CThreadPool::CreateIdleThread(int num)
{
	for(int i = 0; i < num; i++)
	{
		CWorkThread *pt = new CWorkThread(this, m_Log);
		pt->Start();
		AppenToIdleList(pt);
		m_AvailNum++;
	}
}

void CThreadPool::AppenToIdleList(CWorkThread *jobthread)
{
	pthread_mutex_lock(&m_All_mutex);
	m_ThreadList.push_back(jobthread);
	pthread_mutex_unlock(&m_All_mutex);
	
//TRACE(" AppenToIdleList: try to lock m_Idle_mutex");
	pthread_mutex_lock(&m_Idle_mutex);
	m_IdleList.push_back(jobthread);
	pthread_mutex_unlock(&m_Idle_mutex);
//TRACE(" AppenToIdleList: unlock m_Idle_mutex");

	pthread_cond_signal(&m_Pool_cond);
}

void CThreadPool::MoveToBusyList(CWorkThread *idlethread)
{
	pthread_mutex_lock(&m_Busy_mutex);
	TRACE(idlethread << "Move To Busy");
	m_BusyList.push_back(idlethread);
	pthread_mutex_unlock(&m_Busy_mutex);

	m_AvailNum--;

//TRACE(" MoveToBusyList: try to lock m_Idle_mutex");
	pthread_mutex_lock(&m_Idle_mutex);
	vector<CWorkThread *>::iterator pos;
	pos = find(m_IdleList.begin(), m_IdleList.end(), idlethread);
	if(pos != m_IdleList.end())
		m_IdleList.erase(pos);
	pthread_mutex_unlock(&m_Idle_mutex);
//TRACE(" MoveToBusyList: unlock m_Idle_mutex");
}

CWorkThread* CThreadPool::GetIdleThread()
{
//TRACE(" GetIdleThread: try to lock m_Idle_mutex");
	pthread_mutex_lock(&m_Idle_mutex);
	while(m_IdleList.size() == 0)
	{
		TRACE("No Idle threads, wait ...");
		pthread_cond_wait(&m_Pool_cond, &m_Idle_mutex);
		TRACE("Idle cond is wakeup, running ...");
	}
	CWorkThread *pt = (CWorkThread*)m_IdleList.front();
	pthread_mutex_unlock(&m_Idle_mutex);
//TRACE(" GetIdleThread: unlock m_Idle_mutex");

	return pt;
}

void CThreadPool::MoveToIdleList(CWorkThread *busythread)
{
//TRACE(" MoveToIdleList: try to lock m_Idle_mutex");
	pthread_mutex_lock(&m_Idle_mutex);
	TRACE("Move To Idle");
	m_IdleList.push_back(busythread);
	pthread_mutex_unlock(&m_Idle_mutex);
//TRACE(" MoveToIdleList: unlock m_Idle_mutex");

	m_AvailNum++;

	pthread_mutex_lock(&m_Busy_mutex);
	vector<CWorkThread *>::iterator pos;
	pos = find(m_BusyList.begin(), m_BusyList.end(), busythread);
	if(pos != m_BusyList.end())
		m_BusyList.erase(pos);
	pthread_mutex_unlock(&m_Busy_mutex);

	pthread_cond_signal(&m_Pool_cond);
}

void CThreadPool::Run(CJob *job)
{
TRACE("Try to lock m_Pool_mutex");
	pthread_mutex_lock(&m_Pool_mutex);
	while(GetBusyNum() == m_MaxNum)
	{
		TRACE("can not get resource to hanlde new task, wait for Idle thread, BusyNum: " << GetBusyNum() << " IdleNum: " << GetIdleNum());
		pthread_cond_wait(&m_Pool_cond, &m_Pool_mutex);
	}

	if(m_AvailNum < m_AvailLow) //add more threads
	{
		int num = m_AvailMax - m_AvailNum;
		if(GetAllNum() + num > m_MaxNum)
			num = m_MaxNum - GetAllNum();
		CreateIdleThread(num);
	}

	CWorkThread *pt = GetIdleThread();
	if(pt != NULL)
	{
		MoveToBusyList(pt);
TRACE("Try to AddNewJob");
		pt->AddNewJob(job);
TRACE("End to AddNewJob");
	}
	else
		throw Error_Info("ThreadPool Run error !");

	pthread_mutex_unlock(&m_Pool_mutex);
TRACE("unlock m_Pool_mutex");

}

void CThreadPool::TerminalAll()
{
	TRACE("ThreadPool:: Terminal all threads");
	SetMonitorExitFlag(true);
	for(int i = 0; i < GetAllNum(); i++)
		m_ThreadList[i]->Terminal();
}
int CThreadPool::GetBusyNum()
{
	pthread_mutex_lock(&m_Busy_mutex);
	int num = m_BusyList.size();
	pthread_mutex_unlock(&m_Busy_mutex);
	
	return num;
}

int CThreadPool::GetIdleNum()
{
	//TRACE(" try to lock m_Idle_mutex");
	pthread_mutex_lock(&m_Idle_mutex);
	int num = m_IdleList.size();
	pthread_mutex_unlock(&m_Idle_mutex);
	//TRACE(" unlock m_Idle_mutex");
	
	return num;
}
int CThreadPool::GetAllNum()
{
	pthread_mutex_lock(&m_All_mutex);
	int num = m_ThreadList.size();
	pthread_mutex_unlock(&m_All_mutex);
	
	return num;
}

void *MonitorThread(void *p)
{
	CThreadPool *pool = (CThreadPool*)p;
	pool->Monitor();
}

void CThreadPool::CreateMonitorThread()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_t id;
	if(pthread_create(&id, &attr, MonitorThread, (void*)this))
	{
		TRACE("Create Monitor Thread Error");
		throw Error_Info("Create Monitor Thread Error");
	}
	pthread_attr_destroy(&attr);
}

void CThreadPool::Monitor()
{
	usleep(1000*1000*1);
	while(!GetMonitorExitFlag())
	{
		int busy = GetBusyNum();

		char logtxt[256*10];
		memset(logtxt, 0, sizeof(logtxt));
		sprintf(logtxt, "%s, %d, %s, %d, %s, %d", "----------Total Threads: ", GetAllNum(), " Busy: ", busy, "Idle: ", GetIdleNum());
		TRACE(logtxt);
		memset(logtxt, 0, sizeof(logtxt));
		sprintf(logtxt, "%s", "Busy List:");
		for (int i = 0; i < busy; i++)
		{
			sprintf(logtxt, " %s %s", logtxt, m_BusyList[i]);
			//TRACE(m_BusyList[i]);
		}
		TRACE(logtxt);
		TRACE("---------------------------------------------------------------------");
		usleep(1000*1000*1);
	}
	TRACE("Monitor thread exit");
}

CThreadPool::~CThreadPool()
{
	TerminalAll();
	Need to add :
	pthread_mutex_destroy();
	pthread_cond_destroy(&cond)
}
