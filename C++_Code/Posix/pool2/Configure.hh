#ifndef __CONFIGURE__HH
#define __CONFIGURE__HH
#include <string>
using namespace std;

class Config
{
public:
	Config();
	~Config();
	void SetMax(int max){m_MaxThreads = max;}
	void SetInit(int init){m_InitThreads = init;}
	void SetAvaLow(int low){m_AvailLow = low;}
	void SetAvaMax(int max){m_AvailMax = max;}
	void SetLogName(const string &s){m_LogName = s;}
	int GetMax(){return m_MaxThreads;}
	int GetInit(){return m_InitThreads;}
	int GetAvailLow(){return m_AvailLow;}
	int GetAvailMax(){return m_AvailMax;}
	string GetLogName(){return m_LogName;} 

private:
	unsigned int m_MaxThreads;
	unsigned int m_InitThreads;
	unsigned int m_AvailLow;
	unsigned int m_AvailMax;
	string m_LogName;
};
#endif
