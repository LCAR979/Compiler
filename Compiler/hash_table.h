#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "common.h"

template <typename Key, typename Val>
class HashTable
{
	template<class Key, class Val>
	struct HashItem
	{
		Key key;
		Val val;
		unsigned int hash_val;
		HashItem(Key key, Val val, unsigned int hash_val) :
			key(key), val(val), hash_val(hash_val){}
	};
private:
	const static int kHashTableCapacity = 4099;	//a prime
	HashItem<Key, Val>*  hash_vec_[kHashTableCapacity] ;
public:
	HashTable();
	unsigned int HashPJW(Key key);
	unsigned int HashBKDR(Key key);
	bool Insert(Key key, Val new_node);
	Val Find(Key key);
	void* FindAddr(Key key);
	unsigned int Hash(Key key);
};

template <typename Key, typename Val>
HashTable<Key, Val>::HashTable()
{
	for (size_t i = 0; i < kHashTableCapacity; i++)
		hash_vec_[i] = NULL;
}

template <typename Key, typename Val>
bool HashTable<Key, Val>::Insert(Key key, Val val)
{
	unsigned int hash_val = Hash(key), index = hash_val;
	HashItem<Key, Val>* new_item = new HashItem<Key, Val>(key, val, hash_val);
	if (NULL == Find(key))
	{
		while (hash_vec_[index] != NULL && hash_vec_[index]->hash_val != hash_val)
			index++;
		hash_vec_[index] = new_item;
		return true;
	}	
	return false;
}

template <typename Key, typename Val>
Val HashTable<Key, Val>::Find(Key key) 
{
	size_t hash_val = Hash(key), index = hash_val;
	while (hash_vec_[index] != NULL){
		if (hash_vec_[index]->hash_val == hash_val)
		{
			return (hash_vec_[index]->val);
		}
		index++;
	}
	return (Val)NULL;
}
template <typename Key, typename Val>
void* HashTable<Key, Val>::FindAddr(Key key)
{
	size_t hash_val = Hash(key), index = hash_val;
	while (hash_vec_[index] != NULL){
		if (hash_vec_[index]->hash_val == hash_val)
		{
			return &(hash_vec_[index]);
		}
		index++;
	}
	return NULL;
}
template <typename Key, typename Val>
unsigned int HashTable<Key, Val>::Hash(Key key)
{
	return HashBKDR(key);
}
template <typename Key, typename Val>
unsigned int HashTable<Key, Val>::HashPJW(Key key)
{

	char* p;
	unsigned int h = 0, g;
	for (p = key; *p != '\0'; p++)
	{
		h = (h << 4) + (*p);
		if (g = h & 0xf0000000)
		{
			h = h ^ (g >> 24);
			h = h^g;
		}
	}
	return h % kHashTableCapacity;
}

template <typename Key, typename Val>
unsigned int HashTable<Key, Val>::HashBKDR(Key key)
{
	unsigned int seed = 13131;
	unsigned int hash = 0;
	while (*key)
	{
		hash = hash * seed + (*key++);
	}
	return (hash & 0x7fffffff)  % kHashTableCapacity;
}
#endif