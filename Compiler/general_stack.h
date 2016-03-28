#include "common.h"

template <typename T>
class GeneralStack
{
private:
	T g_stack[kStackMaxSize];
	int index = -1;
public:
	void Push(T &item)
	{
		g_stack[++index] = T;
	}
	void Pop()
	{
		index--;
	}
	T* Top()
	{
		return &g_stack[index];
	}
	bool Empty()
	{
		return index == -1;
	}
	bool Full()
	{
		return index == kStackMaxSize - 1;
	}
};