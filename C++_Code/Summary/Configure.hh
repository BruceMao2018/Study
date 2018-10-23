#ifndef __CONFIGURE__HH
#define __CONFIGURE__HH
#include <string>
using namespace std;

class Config
{
public:
	Config();
	Config(int Listen, int max, int init, int port);
	~Config();
	void SetListenThreads(int n){m_ListenThreads = n;}
	void SetListenPort(int port){m_ListenPort = port;}
	void SetMaxTaskThreads(int max){m_MaxTaskThreads = max;}
	void SetInitTaskThreads(int init){m_InitTaskThreads = init;}
	int GetMaxTaskThreads(){return m_MaxTaskThreads;}
	int GetInitTaskThreads(){return m_InitTaskThreads;}
	int GetListenThreads(){return m_ListenThreads;}
	int GetListenPort(){return m_ListenPort;}

private:
	unsigned int m_ListenThreads;
	unsigned int m_ListenPort;
	unsigned int m_MaxTaskThreads;
	unsigned int m_InitTaskThreads;
};
#endif
