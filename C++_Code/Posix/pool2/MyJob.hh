#ifndef __MYJOB__HH
#define __MYJOB__HH
#include "Job.hh"
#include <iostream>
#include <fstream>
#include "Log.hh"
using namespace std;

class MyJob : public CJob
{
public:
	MyJob();
	MyJob(CLog *log);
	~MyJob();
	void Run();
	void KeyInData(const string &);
	string GetLogName(){return m_LogName;}
	//bool WriteLog(ostream &s){return m_Log->WriteLog(s);};
private:
	string m_data;
	string m_LogName;
	CLog *m_Log;
};

#endif
