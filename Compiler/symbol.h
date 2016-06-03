#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "global.h"

const int kHashTableCapacity = 4099;

typedef enum _SymType
{
	Int, Real, String, Char, Array,
}SymType;

typedef struct _SymItem
{
	std::string name;
	SymType type;
	int offset;
	struct _SymItem* next_symbol;

	bool is_array;
	char* array_addr;
	_SymItem(std::string _name, SymType _type, int _offset)
	{
		name = _name;
		type = _type;
		offset = _offset;
	}
} SymItem;

class SymTable
{
public:
	SymTable* prev_sym_table;
	int total_width;
	SymItem* table[kHashTableCapacity];

	size_t HashBKDR(char* hash_key);
	SymTable* MakeSymTable(SymTable* _prev_sym_table);
	bool EnterSymbol(char* name, SymType symType, int offset);
	void AddWidth(int width);
	bool LookupSymbol(char* name);
};

#endif