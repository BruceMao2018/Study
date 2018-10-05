#ifndef __MYJOB__HH
#define __MYJOB__HH
#include "Job.hh"
#include <string>
using namespace std;
class MyJob : public BaseJob
{
public:
	MyJob(CLog *log):BaseJob(log), index(0), m_Log(log){content="";}
	~MyJob();
	void Run();
	void SetData(int, string);
private:
	int index;
	string content;
	CLog *m_Log;
};

#endif
