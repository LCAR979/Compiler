#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <cstdio>
#include <cstdlib>
#include "common.h"

const int kHashPJWPrime = 211;
const int kHashTableSlotNum = kHashPJWPrime;

typedef struct _SymbolItem
{
	TokenType token_type;
	char* name;
	int* value_addr;
	void* extend_attr;
	struct _SymbolItem* next_hash_item;
	_SymbolItem ()
	{
		name = NULL;
		value_addr = NULL;
		extend_attr = NULL;
		next_hash_item = NULL;
	};
} SymbolItem;

class HashTable
{
private:
	SymbolItem* hash_vec_[kHashTableSlotNum];
public:
	HashTable();
	bool Insert(char* _name, TokenType _token_type);
	int Find(char* _name);
	unsigned int HashPJW(char* _name);
	unsigned int HashBKDR(char* _name);
};

#endif