#ifndef __JOB__HH
#define __JOB__HH
#include <string>
#include "Log.hh"
using namespace std;
class BaseJob
{
public:
	BaseJob(CLog *);
	virtual ~BaseJob();
	virtual void Run();
	virtual void SetData(int, string) = 0;
private:
	string data;
	CLog *m_Log;
};

#endif
