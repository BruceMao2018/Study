#ifndef __THROW__HH
#define __THROW__HH
#include <string>
using namespace std;

class MyException
{
public:
	MyException(const string &msg):message(msg){}
	const string &getError() {return message;}

private:
	string message;
};

#endif
