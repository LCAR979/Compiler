#ifndef __PARSER_H__
#define __PARSER_H__

#include "common.h"
#include "general_stack.h"
#include "hash_table.h"
typedef struct Production
{
	char* l_part;
	char* r_part;
	struct Production* next_hash_item;
}Production;

class Parser
{
private:
	FILE* grammer_fp_;
	GeneralStack<char> char_stack_;
	GeneralStack<int> state_stack_;
	HashTable<Production> l2r_production_table_;

	int crt_state_ = 0;
	char ch_;
public:
	void Init();
	void ErrorHandle();

};

#endif