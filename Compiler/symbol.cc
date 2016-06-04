#include "symbol.h"

size_t SymTable::HashBKDR(string hash_key)
{
	string key = hash_key;
	unsigned int seed = 13131;
	unsigned int hash = 0;
	for (size_t i = 0; i < key.length(); i++)
	{
		hash = hash * seed + key.at(i);
	}
	return (hash & 0x7fffffff) % kHashTableCapacity;
}

SymTable::SymTable(SymTable* _prev_sym_table)
{
	prev_sym_table = _prev_sym_table;
	offset = 0;
	memset(table, 0, sizeof(table));
}

bool SymTable::EnterSymbol(string name, SymType symType, SymTable* _locate_sym_table)
{
	if (CanEnterSymbol(name))
	{
		size_t index = HashBKDR(name);
		if (table[index] == NULL)
			table[index] = new SymItem(name, symType, this->offset, _locate_sym_table);
		else
		{
			SymItem* sym = new SymItem(name, symType, this->offset, _locate_sym_table);
			sym->next_symbol = table[index];
			table[index] = sym;
		}
		return true;
	}
	return false;
}


bool SymTable::IsSymbolDefined(string name)
{
	size_t index = HashBKDR(name);
	for (SymTable* sym_table = this; sym_table != NULL; sym_table = sym_table->prev_sym_table)
	{
		for (SymItem* sym_item = sym_table->table[index]; sym_item != NULL; sym_item = sym_item->next_symbol)
		{
			if (sym_item->name == name)
			{
				return true;
			}
		}
	}
	return false;
}

bool SymTable::CanEnterSymbol(string name)
{
	size_t index = HashBKDR(name);
	for (SymItem* sym_item = table[index]; sym_item != NULL; sym_item = sym_item->next_symbol)
	{
		if (sym_item->name == name)
		{
			return false;
		}
	}
	return true;
}

SymTable* program_sym_table = new SymTable(NULL);
SymTable* crt_sym_table = program_sym_table;