#ifndef __PARSER_H__
#define __PARSER_H__

#include "common.h"
#include "general_stack.h"
#include "hash_table.h"
typedef struct Production
{
	char l_part;
	char r_part[kMaxProductionRightLen];
	Production(char pa_l_part, char* pa_r_part)
	{
		l_part = pa_l_part;
		strcpy(r_part, pa_r_part);
	}
}Production;

class Parser
{
private:
	FILE* grammer_fp_;
	GeneralStack<char> char_stack_;
	GeneralStack<int> state_stack_;
	HashTable<char*, int> char_table_;
	HashTable<char*, int> variable_table_;
	
	Production* production_table_[kMaxProductionNum];
	int production_table_tail_;
	char ch_;
	int action_table_[kStateTypeNum][kCharTypeNum];
	int goto_table_[kStateTypeNum][kVariableTypeNum];
public:
	void Init();
	void ErrorHandle();
	void Startup();
	void FillActionTable();
	void FillGotoTable();
	int Decode(HashTable<char*, int>table, char* ch);
};

#endif