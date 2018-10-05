#ifndef __CREATE_HH
#define __CREATE_HH

#include <iostream>
#include <pthread.h>
#include <stdexcept>
#include <stdlib.h>
#include <string>
using namespace std;

class Hello
{
public:
	static void* say_hello(void *p);
};

class MyError : public runtime_error
{
public:
	explicit MyError(const string &s):runtime_error(s){}
	~ MyError() throw() {}
private:
	string info;
};

#endif
