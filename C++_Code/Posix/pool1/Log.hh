#ifndef __LOG__HH
#define __LOG__HH

#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include "Error_Info.hh"


//NOTE NOTE !!!!!!!!!!!!!!!!!!!!!!
//you can not add any mutex inside of TRACE, since the 'a' include some mutex already, it will cause deadlock

#define TRACE(a) \
{\
m_Log->Lock();\
ofstream os(m_Log->GetLogFile().c_str(), ios::app);\
if(!os)\
{\
	cout << "-------------------------------Write Log Error" << endl;\
	 throw Error_Info("Write Log Error");\
}\
os << __FILE__ << ":" << __LINE__ << " thread:[" << pthread_self() << "] "<< a << endl;\
os.close();\
m_Log->UnLock();\
}

using namespace std;
class CLog
{
public:
	CLog();
	CLog(string &);
	~CLog();
	void SetLogName(string &s){m_LogFile = s;}
	//bool WriteLog(ostream &);
	bool Clear();
	string GetLogFile(){return m_LogFile;}
	void Lock(){pthread_mutex_lock(&m_Log_mutex);}
	void UnLock(){pthread_mutex_unlock(&m_Log_mutex);}
protected:
	pthread_mutex_t m_Log_mutex;
private:
	string m_LogFile;
	CLog *m_Log;
};
#endif
