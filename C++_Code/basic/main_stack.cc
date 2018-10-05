#include "stack.hh"
#include "stack.cc"//the template can not be seperated in 2 different files
#include <iostream>
using namespace std;

int main()
{
	Stack<int> s1;

	if(s1.isEmpty())
		cout << "s1 is empty" << endl;

	return 0;
}
