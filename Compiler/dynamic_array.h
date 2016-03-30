#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__
#include "common.h"

template <typename T>
class DynamicArray
{
public:
	DynamicArray();
	void Insert(T new_item);
	bool Find(T item) const;
	size_t GetSize() const  { return size_; }
	size_t GetFilled() const { return filled_; }
	T& DynamicArray<T>::operator [](size_t& pos);
private:
	T* d_array_;
	int size_;
	int filled_;	//the num of items filled, the last_index + 1
	static const int init_size_ = 2;
	void Expand();
	void Expand(int size);
};

template <typename T>
DynamicArray<T>::DynamicArray()
{
	size_ = init_size_;
	filled_ = 0;
	d_array_ = (T*)malloc(sizeof(T) * init_size_);
}
template<typename T>
void DynamicArray<T>::Insert(T new_item)
{
	if (filled_ * 2 >= size_)
		Expand();
	d_array_[filled_++] = new_item;
}
template<typename T>
bool DynamicArray<T>::Find(T item) const
{
	size_t index = 0;
	while (index < GetFilled())
	{
		if (d_array_[index] == t)
			return true;
		index++;
	}
	return false;
}
template <typename T>
void DynamicArray<T>::Expand()
{
	T* new_d_array =  (T*)realloc(d_array_, sizeof(T) * size_ * 2);
	d_array_ = new_d_array;
	size_ *= 2;
}
template <typename T>
void DynamicArray<T>::Expand(int size)
{
	T* new_d_array = (T*)realloc(d_array_, sizeof(T) * size);
	d_array_ = new_d_array;
	size_ = size;
}
//template <typename T>
//T& DynamicArray<T>::operator [](size_t& pos)  
//{
//	if (pos < GetFilled())
//		return d_array_[pos];
//	else
//		throw kOutOfRangeException;
//}

#endif