/*
 * Note:1, do not use refer for push_back() if you need to use pop quickly
 * 	2, Try to use new to get heap memory for a class object, not local variable, since it may cause unexce[ted destrpy
 */
#include "pool.hh"
#include "WorkThread.hh"
ThreadPool::ThreadPool(CLog *log, int max, int min)
{
	m_Log = log;
	TRACE("Max value: " << max);
	//full = false;
	MaxThreads = max;
	MinThreads = min;
	CurThreads = 0;
	TaskEnd = false;
	tasks.clear();
	DesThreads = 0;
	MaxTaskAllow = 10000;

	pthread_mutex_init(&job_mutex, NULL);
	pthread_mutex_init(&pool_mutex, NULL);
	//pthread_cond_init(&cond, NULL);
	CreateThreads(MinThreads);
	CreateMonitorThreads();
};

ThreadPool::~ThreadPool()
{
	pthread_mutex_destroy(&job_mutex);
	pthread_mutex_destroy(&pool_mutex);
	TRACE("----thread pool destroy----");
}

void ThreadPool::CreateThreads(int NumOfThreads)
{
	pthread_mutex_lock(&pool_mutex);
	for( int i = 0; i < NumOfThreads && i < MaxThreads; i++)
	{
		WorkThread *p = new WorkThread(this, m_Log);
		CurThreads++;
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
	if(tasks.size() < MaxTaskAllow)
		tasks.push_back(job);
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
	TRACE( "HandleTasks--CurThreads: " << CurThreads);
	//pt->RunStatus();
	while(1)
	{
		pthread_mutex_lock(&pool_mutex);
		pthread_mutex_lock(&job_mutex);
		if(tasks.size() == 0)
		{
			if(TaskEnd) break;

			TRACE("-----No task, sleep ... ");
			if(DesThreads-- > 0)
			{
				TRACE("Threads exit -- ");
				//pt->DesThreads = false;
				CurThreads--;
				pthread_mutex_unlock(&job_mutex);
				pthread_mutex_unlock(&pool_mutex);
				//pthread_detach(pthread_self());//to release resource, or else, it will not release source
				//pthread_exit(NULL);
				return (void)0;
			}

			TRACE("unlock mutex and wait ...");
			pthread_mutex_unlock(&job_mutex);
			pthread_mutex_unlock(&pool_mutex);
			usleep(1000*500);
			TRACE("continue ...");
			continue;
		} //how about using Job_List &job = tasks.front(), Can not!!!
		TRACE("Handle: MaxThreads: " << MaxThreads << " CurThreads: " << CurThreads << " tasks: " << tasks.size());
		BaseJob *job = tasks.front();

		tasks.pop_front();
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
		if(TaskEnd && tasks.size() < CurThreads) break;//monitor threads exit

		pthread_mutex_lock(&pool_mutex);
		//pthread_mutex_lock(&pt->job_mutex);
		TRACE( "monitor running ...");
		TRACE( "monitor: MaxThreads: " << MaxThreads << " CurThreads: " << CurThreads << " tasks: " << tasks.size());
		if(CurThreads < MaxThreads && tasks.size() > CurThreads)
		{
			TRACE( "--Need to increase threads");
			//pthread_mutex_unlock(&pt->job_mutex);
			int num = tasks.size()/CurThreads ;
			num = num/10;

			if(num > 10)
				num = 10;
			if(num == 0)
				num = 2;

			if(num + CurThreads > MaxThreads)
				num = MaxThreads - CurThreads;	

			pthread_mutex_unlock(&pool_mutex);
			CreateThreads(num);
		}
		else if(CurThreads > MinThreads && tasks.size() == 0)
		{
			TRACE( "CurThreads: " << CurThreads << " set DesThreads to true");
			int num = CurThreads - MinThreads ;
			if(num > 10)
				num = 10;
			DesThreads = num;
			//pthread_mutex_unlock(&pt->job_mutex);
			pthread_mutex_unlock(&pool_mutex);
		}
		else
		{
			TRACE("monitor do nothing" );
			//pthread_mutex_unlock(&pt->job_mutex);
			pthread_mutex_unlock(&pool_mutex);
		}
		usleep(1000*500);
	}
	TRACE("monitor threads exit !");
}
