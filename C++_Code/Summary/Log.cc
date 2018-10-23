#include "Log.hh"
#include <fstream>
#include <pthread.h>

CLog::CLog()
{
	pthread_mutex_init(&m_Log_mutex, NULL);
	m_LogFile = "/home/mcadmin/Github/C++_Code/Posix/pool2/MyJob.log";
	m_Log = this;
	Clear();
}

CLog::CLog(string &s)
{
	pthread_mutex_init(&m_Log_mutex, NULL);
	m_LogFile = s;
	m_Log = this;
	Clear();
}

CLog::~CLog()
{
	cout << "CLog Desconstruct" << endl;
}

/*
bool CLog::WriteLog(ostream &s)
{
	Lock();
	ofstream os(m_LogFile.c_str(), ios::app);
	if(!os)
	{
		cout << "===================Open Log File Error=============" << endl;
		UnLock();
		return false;
	}
	os << s << endl;
	os.close();
	UnLock();

	return true;
}
*/

bool CLog::Clear()
{
	Lock();
	ofstream os(m_LogFile.c_str());
	if(!os)
	{
		TRACE("===================Open Log File Error=============");
		UnLock();
		return false;
	}
	os.close();
	UnLock();

	return true;
}
