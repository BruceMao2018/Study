#include "MyJob.hh"
#include <iostream>
using namespace std;

MyJob::~MyJob()
{
	//TRACE( "MYJob Destruct" );
}

void MyJob::SetData(int i, string s)
{
	index = i;
	content = s;
}

void MyJob::Run()
{
	//TRACE("Get task: " << index << " content: " << content );
	usleep(1000*10);
	TRACE( "Task: " << index << "content: " << content << " Done !" );
}
