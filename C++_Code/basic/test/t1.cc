#include "t1.hh"
#include <iostream>
using namespace std;

Student::Student()
{
	cout << "default construct of Student" << endl;
}

int Student::getAge() const
{
	return age;
}
