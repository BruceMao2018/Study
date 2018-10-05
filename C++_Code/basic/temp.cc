#ifndef __TEMP_HH
#define __TEMP_HH
#include <iostream>
using namespace std;

class Temp
{
public:
	Temp(int value);
	~Temp();	
friend	Temp operator+(const Temp &t1, const Temp &t2);
private:
	int data;
};

Temp::Temp(int value):data(value)
{
	cout << "Construct of Temp: data= " << data<< endl;
}

Temp::~Temp()
{
	cout << "Desconstruct of Temp: data= " << data << endl;
}

Temp operator+(const Temp &t1, const Temp &t2)
{
	Temp t3(0);
	t3.data=t1.data+t2.data;
	return t3;
}

#endif

void create()
{
	Temp t1(5), t2(10);
	Temp t3(t1+t2);
}

int main()
{
	create();

	return 0;
}
