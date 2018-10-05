#include <iostream>
#include "throw.hh"
#include <stdexcept>

using namespace std;

int main(int argc, char *argv[])
{
	try {
		if (1 == 1) 
		{
			throw out_of_range("out of range");
			throw MyException("first throw");
		}
	}
	catch (MyException &e1){
		cout << "Error(): " << e1.getError() << endl;
	}
	catch (exception &e){
		cout << "Error(): " << e.what() << endl;
	}

	return 0;
}
