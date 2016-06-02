#ifndef __GENERAL_STACK_H__
#define __GENERAL_STACK_H__

template <typename T>
class GeneralStack
{
	static const int kStackCapacity = 1024;
private:
	T g_stack[kStackCapacity];
	int index = -1;
public:
	GeneralStack()
	{
		index = -1;
	}
	void Push(T item)
	{
		g_stack[++index] = item;
	}
	void Pop()
	{
		index--;
	}
	T Top()
	{
		return g_stack[index];
	}
	bool Empty()
	{
		return index == -1;
	}
	bool Full()
	{
		return index == kStackCapacity - 1;
	}
	int Size()
	{
		return index + 1;
	}
};

#endif