#include "MyJob.hh"
MyJob::MyJob()
{
	m_data = "";
	m_LogName = "tmp.log";
	m_Log = NULL;
}

MyJob::MyJob(CLog *log)
{
	m_data = "";
	m_LogName = log->GetLogFile();
	m_Log = log;
}

void MyJob::KeyInData(const string &s)
{
	m_data = s;
}

void MyJob::Run()
{
	TRACE("Start to work, data: " << m_data << "...");
	usleep(1000*1);
	TRACE("Done, data: " << m_data << "...");
}

MyJob::~MyJob()
{
}
