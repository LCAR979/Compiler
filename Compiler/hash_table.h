#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "common.h"

template <typename T>
class HashTable
{
private:
	T* hash_vec_[kHashTableSize];
public:
	HashTable();
	bool Insert(char* _name, T* new_node);
	T* Find(char* _name);
	unsigned int HashPJW(char* _name);
	unsigned int HashBKDR(char* _name);
};

template <typename T>
HashTable<T>::HashTable()
{
	for (int i = 0; i < kHashTableSize; i++)
		hash_vec_[i] = NULL;
}

template <typename T>
bool HashTable<T>::Insert(char* _name, T* new_item)
{
	unsigned int hash_val = HashPJW(_name);
	T* head_hash_item = hash_vec_[hash_val];

	if (head_hash_item == NULL)
	{
		hash_vec_[hash_val] = new_item;
		return true;
	}
	if (head_hash_item->next_hash_item != NULL)
	{
		new_item->next_hash_item = head_hash_item->next_hash_item;
	}
	head_hash_item->next_hash_item = new_item;
	return true;
}

template <typename T>
T* HashTable<T>::Find(char* _name)
{
	unsigned int hash_val = HashPJW(_name);
	T* crt_hash_item = hash_vec_[hash_val];
	while (crt_hash_item != NULL)
	{
		if (strcmp(crt_hash_item->name, _name) == 0)
			return crt_hash_item;
		else
			crt_hash_item = crt_hash_item->next_hash_item;
	}
	return NULL;
}

template <typename T>
unsigned int HashTable<T>::HashPJW(char* _name)
{

	char* p;
	unsigned int h = 0, g;
	for (p = _name; *p != '\0'; p++)
	{
		h = (h << 4) + (*p);
		if (g = h & 0xf0000000)
		{
			h = h ^ (g >> 24);
			h = h^g;
		}
	}
	return h % kHashPJWPrime;
}

template <typename T>
unsigned int HashTable<T>::HashBKDR(char* _name)
{
	unsigned int seed = 13131;
	unsigned int hash = 0;
	while (*_name)
	{
		hash = hash * seed + (*_name++);
	}
	return (hash & 0x7fffffff);
}

#endif