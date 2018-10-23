#include "Configure.hh"
#include <string>
#include <iostream>
using namespace std;

Config::Config()
{
	m_ListenThreads = 2;
	m_MaxTaskThreads = 5;
	m_InitTaskThreads = 2;
	m_ListenPort = 8000;
}

Config::Config(int listen, int max, int init, int port)
{
	m_ListenThreads = listen;
	m_MaxTaskThreads = max;
	m_InitTaskThreads = init;
	m_ListenPort = port;
}

Config::~Config()
{
	cout << "Config Desconstruct" << endl;
}
