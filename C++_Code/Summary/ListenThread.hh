#ifndef __LISTENTHREAD__HH
#define __LISTENTHREAD__HH

#include "Job.hh"
#include "Error_Info.hh"
#include "Log.hh"
#include "pool.hh"
using namespace std;

class ThreadPool;

void *ListenFunction(void*);

class ListenThread
{
public:
	//ListenThread(ThreadPool *);
	ListenThread(ThreadPool *pool, CLog *log, int port);
	~ListenThread();
	void Start();//create threads here !
	void Run();//run it in a loop, call back to ThreadPool::HandleTasks();

	void HandleSocket();
	int Create_Bind_Socket(int port);//return the serfd
	int AcceptConnection(int serfd); //return connected fd
	void setSockNoBlock(int fd);
	void HandleError(const char *msg, int serfd = 0, int epfd = 0, int clifd = 0);
	char *ReadData(int fd);
private:

	ThreadPool *m_Pool;
	CLog *m_Log;
	int m_Port;
};
#endif
