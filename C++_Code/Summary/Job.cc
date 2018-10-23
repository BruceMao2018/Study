#include "Job.hh"
#include <iostream>
using namespace std;
BaseJob::BaseJob(CLog *log)
{
	m_Log = log;
	//TRACE( "BaseJob construct" );
}

BaseJob::~BaseJob()
{
	//TRACE( "BaseJob Deconstruct" );
}

void BaseJob::Run()
{
	TRACE( "BaseJob::Run, never to be here !" );
}
