#include <iostream>
using namespace std;

class A
{
public:
	friend class B;
    A(int a,int b,int c):_a(a),_b(b),_c(c){ } 
	int _a;

protected:
    int _b;
private:
    int _c;
};

class B:public A
{
public:
    B(int a,int b,int c):A(a,b,c){ }
    void print()
	{
        cout<<_a<<endl<<_b<<endl<<this->_c<<endl;
    }
};
int main()
{
    B b(1,2,3);
    b.print();
    return 0;
}
