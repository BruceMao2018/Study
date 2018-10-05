#ifndef __JOB__HH
#define __JOB__HH
#include <string>
using namespace std;
class CJob
{
public:
	CJob();
	virtual ~CJob();
	virtual void Run() = 0;
	virtual void KeyInData(const string&) = 0;
private:
	string m_data;
	//CLog *m_Log;
};

#endif
