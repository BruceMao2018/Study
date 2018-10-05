#include <iostream>
using namespace std;

class A
{
public:
	A() { cout << "construct of A" << endl; }
	int var;
};

class B : virtual public A
{
public:
	B() { cout << "construct of B" << endl; }
};

class C : virtual public A
{
public:
	C() { cout << "construct of C" << endl; }
};

class D : public B, public C
{
public:
	D() { cout << "construct of D" << endl; }
};

int main()
{
	D d;
	d.B::var = 100;
	cout << "d.var : " << d.C::var << endl;

	return 0;
}
