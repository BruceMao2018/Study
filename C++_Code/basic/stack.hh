#ifndef _STACK_HH_
#define _STACK_HH_

template <class T, int SIZE = 50>
class Stack
{
public:
	Stack();
	void push(const T &item);
	T pop();
	void clear();
	T &peek() const;
	bool isEmpty() const;
	bool isFull() const;
	int getSize() const { return top;}
private:
	T Array[SIZE];
	int top;
	

};

#endif
