/*
 * Note:1, do not use refer for push_back() if you need to use pop quickly
 * 	2, Try to use new to get heap memory for a class object, not local variable, since it may cause unexce[ted destrpy
 */
#include "pool.hh"
#include "WorkThread.hh"
ThreadPool::ThreadPool(CLog *log, Config *conf)
{
	m_Log = log;
	m_Conf = conf;

	m_MaxThreads = m_Conf->GetMaxTaskThreads();
	m_InitThreads = m_Conf->GetInitTaskThreads();
	m_TaskEnd = false;
	m_Tasks.clear();
	m_DesThreads = 0;
	m_MaxTaskAllow = 1000000;

	pthread_mutex_init(&job_mutex, NULL);
	pthread_mutex_init(&pool_mutex, NULL);
	//pthread_cond_init(&cond, NULL);
	CreateTaskThreads(m_InitThreads);
	CreateMonitorThreads();
	CreateListenThreads(m_Conf->GetListenThreads());
};

ThreadPool::~ThreadPool()
{
	pthread_mutex_destroy(&job_mutex);
	pthread_mutex_destroy(&pool_mutex);
	TRACE("----thread pool destroy----");
}

void ThreadPool::CreateTaskThreads(int NumOfThreads)
{
	pthread_mutex_lock(&pool_mutex);
	for( int i = 0; i < NumOfThreads && i < m_MaxThreads; i++)
	{
		WorkThread *p = new WorkThread(this, m_Log);
		m_CurThreads++;
	}
	pthread_mutex_unlock(&pool_mutex);
}

void ThreadPool::CreateMonitorThreads()
{
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	int ret = pthread_create(&tid, &attr, MonitorFunction, (void*)this);
	if(ret != 0)
	{
		throw runtime_error("create threads error");
	}
	pthread_attr_destroy( &attr );
	
};

bool ThreadPool::AddTask(BaseJob *job)
{
	pthread_mutex_lock(&job_mutex);
	if(m_Tasks.size() < m_MaxTaskAllow)
		m_Tasks.push_back(job);
	else
	{
		pthread_mutex_unlock(&job_mutex);
		TRACE("too much of jobs, reject ");
		return false;
	}
	pthread_mutex_unlock(&job_mutex);
	return true;
}

void ThreadPool::HandleTasks()
{
	TRACE( "HandleTasks--CurThreads: " << m_CurThreads);
	//pt->RunStatus();
	while(1)
	{
		pthread_mutex_lock(&pool_mutex);
		pthread_mutex_lock(&job_mutex);
		if(m_Tasks.size() == 0)
		{
			if(m_TaskEnd) break;

			TRACE("-----No task, sleep ... ");
			if(m_DesThreads-- > 0)
			{
				TRACE("Threads exit -- ");
				//pt->DesThreads = false;
				m_CurThreads--;
				pthread_mutex_unlock(&job_mutex);
				pthread_mutex_unlock(&pool_mutex);
				//pthread_detach(pthread_self());//to release resource, or else, it will not release source
				//pthread_exit(NULL);
				return (void)0;
			}

			TRACE("unlock mutex and wait ...");
			pthread_mutex_unlock(&job_mutex);
			pthread_mutex_unlock(&pool_mutex);
			usleep(1000*5);
			TRACE("continue ...");
			continue;
		} //how about using Job_List &job = tasks.front(), Can not!!!
		TRACE("Handle: MaxThreads: " << m_MaxThreads << " CurThreads: " << m_CurThreads << " tasks: " << m_Tasks.size());
		BaseJob *job = m_Tasks.front();

		m_Tasks.pop_front();
		pthread_mutex_unlock(&job_mutex);
		pthread_mutex_unlock(&pool_mutex);
//		TRACE( "Thread: " << pthread_self() << " Index: " << job.index << "  Content: " << job.content );
		//usleep(1000*1000);
		job->Run();
		delete job;
		job = NULL;

	}
	pthread_mutex_unlock(&job_mutex);
	pthread_mutex_unlock(&pool_mutex);
	TRACE(" exit !");
	//pthread_detach(pthread_self());//to release resource, or else, it will not release source
	pthread_exit(NULL);

}

void *MonitorFunction(void*p)
{
	ThreadPool *pt = (ThreadPool*)p;
	pt->Monitor();
}

void ThreadPool::Monitor()
{
	while(1)
	{
		if(m_TaskEnd && m_Tasks.size() < m_CurThreads) break;//monitor threads exit

		pthread_mutex_lock(&pool_mutex);
		//pthread_mutex_lock(&pt->job_mutex);
		TRACE( "monitor running ...");
		TRACE( "monitor: MaxThreads: " << m_MaxThreads << " CurThreads: " << m_CurThreads << " tasks: " << m_Tasks.size() <<"Testing tasks: " << m_Test_Tasks.size());
		cout << "monitor: MaxThreads: " << m_MaxThreads << " CurThreads: " << m_CurThreads << " tasks: " << m_Tasks.size() <<"Testing tasks: " << m_Test_Tasks.size() << endl;
		if(m_CurThreads < m_MaxThreads && m_Tasks.size() > m_CurThreads)
		{
			TRACE( "--Need to increase threads");
			//pthread_mutex_unlock(&pt->job_mutex);
			int num = m_Tasks.size()/m_CurThreads ;
			num = num/10;

			if(num > 10)
				num = 10;
			if(num == 0)
				num = 2;

			if(num + m_CurThreads > m_MaxThreads)
				num = m_MaxThreads - m_CurThreads;	

			pthread_mutex_unlock(&pool_mutex);
			CreateTaskThreads(num);
		}
		else if(m_CurThreads > m_InitThreads && m_Tasks.size() == 0)
		{
			TRACE( "CurThreads: " << m_CurThreads << " set DesThreads to true");
			int num = m_CurThreads - m_InitThreads ;
			if(num > 10)
				num = 10;
			m_DesThreads = num;
			//pthread_mutex_unlock(&pt->job_mutex);
			pthread_mutex_unlock(&pool_mutex);
		}
		else
		{
			TRACE("monitor do nothing" );
			//pthread_mutex_unlock(&pt->job_mutex);
			pthread_mutex_unlock(&pool_mutex);
		}
		usleep(1000*1000*1);
	}
	TRACE("monitor threads exit !");
}

void ThreadPool::CreateListenThreads(int num)
{
	pthread_mutex_lock(&pool_mutex);
	for( int i = 0; i < num; i++)
	{
		ListenThread *p = new ListenThread(this, m_Log, m_Conf->GetListenPort() + i);
	}
	pthread_mutex_unlock(&pool_mutex);
}

bool ThreadPool::AddTask(const char *msg)
{
	string job(msg);
	pthread_mutex_lock(&job_mutex);
	if(m_Test_Tasks.size() < m_MaxTaskAllow)
		m_Test_Tasks.push_back(job);
	else
	{
		pthread_mutex_unlock(&job_mutex);
		TRACE("too much of jobs, reject ");
		return false;
	}
	pthread_mutex_unlock(&job_mutex);
	return true;
}
