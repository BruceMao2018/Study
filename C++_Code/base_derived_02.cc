#include <iostream>
using namespace std;

class A
{
public:
//	friend class B;
    A(int a,int b,int c):_a(a),_b(b),_c(c)
	{
		cout << "A constructor " << endl;
	}
	void show() const
	{
		cout << "A::show" << endl;
	}
	int _a;
protected:
    int _b;
private:
    int _c;
};

class B:public A
{
public:
    B(int a,int b,int c):A(a,b,c)
	{ 
		cout << "B constructor " << endl;
	}
    void print()
	{
        cout<<_a<<endl<<_b<<endl ;
    }
	void show() const
	{
		cout << "B::show" << endl;
	}
};
int main()
{
    B b(1,2,3);
    b.print();
	//b.A::show();
	b.show();

    return 0;
}
