#include <string.h>
#include "pool.hh"
#include "Job.hh"
#include "MyJob.hh"
#include "time.hh"
#include "Configure.hh"

char *getLogPath(const char *env);

int main(int argc, char **argv)
{
	string LogFile(getLogPath("PWD"));
	cout << "LogFile:" << LogFile << endl;
	//string LogFile("/home/sc/MyJob.log");
	//Suggest to use "new", not like above way, it will cause coredump.
	CLog *m_Log = new CLog(LogFile);
	Config *m_Conf = new Config(5,100,10,8000);
	try{
		ThreadPool *pool = new ThreadPool(m_Log, m_Conf);
//wait here, main process should be exit by signal()
while(1) {}
		TRACE("all threads going to exit");
		string LocalTime = sysUsecTime();
		TRACE(LocalTime);
		pool->TerminalAll();

		LocalTime = sysUsecTime();
		TRACE(LocalTime);

	}catch (Error_Info &e)
	{
		TRACE("Error: " << e.what());
	}

	pthread_exit(NULL);
	return 0;
}

char *getLogPath(const char *env)
{
	char *path = NULL;
	char logname[1024];
	memset(logname, 0, sizeof(logname));
	if((path = getenv(env)) == NULL)
	{
		cout << "getenv error, exit" << endl;
		exit(-1);
	}

	strcpy(logname, path);
	sprintf(logname, "%s/tmp.txt", logname);

	return logname;
}
