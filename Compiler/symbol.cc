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

SymTable::SymTable(SymTable* _prev_sym_table, string _table_name)
{
	prev_sym_table = _prev_sym_table;
	table_name = _table_name;
	offset = 0;
	for (int i = 0; i < kHashTableCapacity; i++)
	{
		this->table[i] = NULL;
	}
}

bool SymTable::EnterSymbol(string name, SymType symType, SymTable* stm_table_pointer)
{
	size_t index = HashBKDR(name);

	for (SymItem* sym_item = table[index]; sym_item != NULL; sym_item = sym_item->next_symbol)
	{
		if (sym_item->name == name)
		{
			return false;
		}
	}

	if (table[index] == NULL)
		table[index] = new SymItem(name, symType, this->offset, stm_table_pointer);
	else
	{
		SymItem* sym = new SymItem(name, symType, this->offset, stm_table_pointer);
		sym->next_symbol = table[index];
		table[index] = sym;
	}
	switch (symType)
	{
	case Int:
		offset += 4;
		break;
	case Real:
		offset += 8;
		break;
	case SymTableType:
		break;
	}
	return true;

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


SymTable* crt_sym_table;