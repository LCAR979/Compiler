#include "symbol.h"

size_t SymTable::HashBKDR(char* hash_key)
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

SymTable* SymTable::MakeSymTable(SymTable* _prev_sym_table)
{
	SymTable* new_sym_table = new SymTable();
	new_sym_table->prev_sym_table = _prev_sym_table;
	return new_sym_table;
}

bool SymTable::EnterSymbol(char* name, SymType symType, int offset)
{
	size_t index = HashBKDR(name);
	if (table[index] == NULL)
		table[index] = new SymItem(std::string(name), symType, offset);
	else
	{
		SymItem* sym = new SymItem(std::string(name), symType, offset);
		sym->next_symbol =table[index];
		table[index] = sym;
	}
	return true;
}

void SymTable::AddWidth(int width)
{
	total_width = width;
}

bool SymTable::LookupSymbol(char* name)
{
	return true;
}

SymTable* program_sym_table = new SymTable();
SymTable* crt_sym_table = program_sym_table;
