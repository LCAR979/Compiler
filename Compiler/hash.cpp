#include <cstring>
#include "hash.h"

HashTable::HashTable()
{
	for (int i = 0; i < kHashTableSlotNum; i++)
		hash_vec_[i] = NULL;
}
bool HashTable::Insert(char* _name, TokenType _token_type)
{
	unsigned int hash_val = HashPJW(_name);
	HashItem* head_hash_item = hash_vec_[hash_val];
	HashItem* new_item = new HashItem;
	new_item->name = _name;
	new_item->token_type = _token_type;
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

int HashTable::Find(char* _name)
{
	unsigned int hash_val = HashPJW(_name);
	HashItem* crt_hash_item = hash_vec_[hash_val];
	while (crt_hash_item != NULL)
	{
		if (strcmp(crt_hash_item->name, _name) == 0)
			return crt_hash_item->token_type;
		else
			crt_hash_item = crt_hash_item->next_hash_item;
	}
	return -1;
}

unsigned int HashTable::HashPJW(char* _name)
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


unsigned int HashTable::HashBKDR(char* _name)
{
	unsigned int seed = 13131;
	unsigned int hash = 0;
	while (*_name)
	{
		hash = hash * seed + (*_name++);
	}
	return (hash & 0x7fffffff);
}