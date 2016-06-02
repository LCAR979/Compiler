#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>
#include "global.h"
#include "general_stack.h"

class Parser
{
private:
	int production_table_[kMaxProductionNum][kMaxProductionRightLen];
	int drive_table_[kMaxStateNum][kMaxVarNum];

	void FillProductionTable();
	void FillDriveTable();
public:
	void Init();
	void Startup();
	void Close();
};

#endif