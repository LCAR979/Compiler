#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "global.h"

const int kHashTableCapacity = 4099;

class SymTable
{
	typedef struct _SymItem
	{
		std::string name;
		SymType type;
		int offset;		
		SymTable* sym_table;

		struct _SymItem* next_symbol;
		bool is_array;
		char* array_addr;
		_SymItem(std::string _name, SymType _type, int _offset, SymTable* _sym_table)
		{
			name = _name;
			type = _type;
			offset = _offset;
			sym_table = _sym_table;
		}
	} SymItem;

public:
	SymTable* prev_sym_table;
	int offset;
	SymItem* table[kHashTableCapacity];

	SymTable(SymTable* _prev_sym_table);
	size_t HashBKDR(string hash_key);
	bool EnterSymbol(string name, SymType symType, SymTable* _locate_sym_table);
	bool IsSymbolDefined(string name);
	bool CanEnterSymbol(string name);
};

extern SymTable* program_sym_table;
extern SymTable* crt_sym_table;


#endif