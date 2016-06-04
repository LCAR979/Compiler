#ifndef __GENERAL_STACK_H__
#define __GENERAL_STACK_H__

static const int kStackCapacity = 1024;

template <typename T>
class GeneralStack
{

public:
	int top = -1;
	T s[kStackCapacity];
	GeneralStack()
	{
		top = -1;
		memset(s, 0, sizeof(s));
	}
	void Push(T item)
	{
		s[++top] = item;
	}
	void Pop()
	{
		top--;
	}
	T Top()
	{
		return s[top];
	}
	bool Empty()
	{
		return top == -1;
	}
	bool Full()
	{
		return top == kStackCapacity - 1;
	}
	int Size()
	{
		return top + 1;
	}
};

#endif