#include "Configure.hh"
#include <string>
#include <iostream>
using namespace std;

Config::Config()
{
	m_MaxThreads = 0;
    m_InitThreads = 0;
    m_AvailLow = 0;
    m_AvailMax = 0;
	m_LogName.empty();
}

Config::~Config()
{
	cout << "Config Desconstruct" << endl;
}
