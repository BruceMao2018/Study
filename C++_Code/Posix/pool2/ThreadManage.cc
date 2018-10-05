#include "ThreadManage.hh"

CThreadManage::CThreadManage()
{
	m_NumOfThreads = 10;//default value is 10
	m_pool = new CThreadPool(m_NumOfThreads);
}

CThreadManage::CThreadManage(int num)
{
	m_NumOfThreads = num;
	m_pool = new CThreadPool(m_NumOfThreads);
}
CThreadManage::CThreadManage(Config *conf, CLog *log)
{
	m_Log = log;
	m_pool = new CThreadPool(conf, log);
}

void CThreadManage::Run(CJob *job)
{
	m_pool->Run(job);
}

void CThreadManage::TerminalAll(void)
{
	m_pool->TerminalAll();
}

CThreadManage::~CThreadManage()
{
	if(m_pool != NULL)
	delete m_pool;
}
