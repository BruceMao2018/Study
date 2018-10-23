#ifndef __ERROR_INFO__HH
#define __ERROR_INFO__HH
#include <stdexcept>
#include <stdlib.h>
using namespace std;

class Error_Info : public runtime_error
{
public:
	explicit Error_Info(const string &s):runtime_error(s){}
};
#endif
