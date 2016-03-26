#ifndef __HASH_H__
#define __HASH_H__

#include <cstdio>
#include <cstdlib>
#include "common.h"

const int kHashPJWPrime = 211;
const int kHashTableSlotNum = kHashPJWPrime;

typedef struct _HashItem
{
	char* name;
	TokenType token_type;
	void* value;
	//void* extend_attr;
	struct _HashItem* next_hash_item;
	_HashItem ()
	{
		name = NULL;
		value = NULL;
		//value = extend_attr = NULL;
		next_hash_item = NULL;
	};
} HashItem;

class HashTable
{
private:
	HashItem* hash_vec_[kHashTableSlotNum];
public:
	HashTable();
	bool Insert(char* _name, TokenType _token_type);
	int Find(char* _name);
	unsigned int HashPJW(char* _name);
	unsigned int HashBKDR(char* _name);
};

#endif