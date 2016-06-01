#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "global.h"

const int kHashTableCapacity = 4099;

typedef struct _SymbolItem
{
	char* name;
	TokenType type;
	int offset;
	_SymbolItem* prv_symbol_in_closure;
	_SymbolItem* next_symbol;

	bool is_array;
	char* array_addr;
	_SymbolItem(char* _name, TokenType _type, int _offset)
	{
		name = _name;
		type = _type;
		offset = _offset;
	}
	//_SymbolItem(char* _name, TokenType _type, int _offset)
	//{
	//	name = _name;
	//	type = _type;
	//	offset = _offset;
	//}
} SymbolItem;

typedef struct _Sym_table
{
	sym_table* prev_sym_table;
	int total_width;
	SymbolItem* table[kHashTableCapacity];
} Sym_table;

size_t HashBKDR(char* hash_key)
{
	char* key = hash_key;
	unsigned int seed = 13131;
	unsigned int hash = 0;
	while (*key)
	{
		hash = hash * seed + (*key++);
	}
	return (hash & 0x7fffffff) % kHashTableCapacity;
}

Sym_table* make_symbol_table(Sym_table* prev_sym_table)
{
	Sym_table* new_sym_table = new (Sym_table);
	new_sym_table->prev_sym_table = prev_sym_table;
	return new_sym_table;
}

bool enter_symbol(Sym_table* sym_table, char* name, TokenType tokenType, int offset)
{
	size_t index = HashBKDR(name);
	if (sym_table->table[index] == NULL)
		sym_table->table[index] = new SymbolItem(name, tokenType, offset);
	else
	{
		SymbolItem* sym =  new SymbolItem(name, tokenType, offset);
		sym->next_symbol = sym_table[index];
		sym_table[index] = sym;
	}
}

void add_width(Sym_table* sym_table, int width)
{
	sym_table->total_width = width;
}



#endif