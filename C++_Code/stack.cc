#include "stack.hh"

template <class T, int SIZE>
Stack<T, SIZE>::Stack()
{
	top = -1 ;
}

template <class T, int SIZE>
void Stack<T, SIZE>::push(const T &item)
{
	Array[++top] = item ;
}

template <class T, int SIZE>
T Stack<T, SIZE>::pop()
{
	return Array[top--] ;
}

template <class T, int SIZE>
bool Stack<T, SIZE>::isEmpty() const
{
	if(top == -1)
		return true;
	else
		return false;
}

template <class T, int SIZE>
bool Stack<T, SIZE>::isFull() const
{
	if(top == SIZE)
		return true;
	else
		return false;
}

template <class T, int SIZE>
void Stack<T, SIZE>::clear()
{
	top = -1;
}

template <class T, int SIZE>
T &Stack<T, SIZE>::peek() const
{
	return Array[top];
}
