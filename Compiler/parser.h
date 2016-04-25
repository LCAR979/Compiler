#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>
#include "common.h"
#include "general_stack.h"

class Parser
{
private:
	GeneralStack<int> var_stack_;
	GeneralStack<int> state_stack_;
	int production_table_[kMaxProductionNum][kMaxProductionRightLen];
	int drive_table_[kStateTypeNum][kVariableTypeNum];

	void FillProductionTable();
	void FillDriveTable();
	void ErrorHandle();
public:
	void Init();
	void Startup(std::vector<TokenItem*>&  token_list);
	void Close();
};

#endif