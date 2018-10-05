#include "pool.hh"
#include "Job.hh"
#include "MyJob.hh"
#include "time.hh"
int main(int argc, char **argv)
{
	string LogFile("/home/mcadmin/Github/C++_Code/Posix/pool1/MyJob.log");
	//string LogFile("/home/sc/MyJob.log");
//Suggest to use "new", not like above way, it will cause coredump.
    CLog *m_Log = new CLog(LogFile);
	ThreadPool *pool = new ThreadPool(m_Log, 200,20);
	try{
	
int loop = 50;
while(loop--)
{
	int n = 5000;
	for(int i = 0; i < n; i++)
	{
		MyJob *a = new MyJob(m_Log);
		a->SetData(i, "I am A");
		pool->AddTask(a);

		MyJob *b = new MyJob(m_Log);
		b->SetData(n+i, "I am B");
		pool->AddTask(b);
	}
	cout << "loop: " << loop << endl;

usleep(1000*1000*2);
}

}catch (Error_Info &e)
{
		TRACE("Error: " << e.what());
}


	TRACE("all threads exit");
	string LocalTime = sysUsecTime();
	TRACE(LocalTime);
	pool->TerminalAll();

	LocalTime = sysUsecTime();
	TRACE(LocalTime);
	usleep(1000*1000*5);

/*
	usleep(1000*1000*5);//wait for task end;
	delete pool;
	pool = NULL;

	usleep(1000*1000*2);//wait for Log task end;
	delete m_Log;
	m_Log = NULL;
*/


	pthread_exit(NULL);

	return 0;
}
