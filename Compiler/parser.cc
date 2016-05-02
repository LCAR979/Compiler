#include <stdio.h>
#include "common.h"
#include "hash_table.h"
#include "parser.h"

void Parser::Init()
{
	FillProductionTable();
	FillDriveTable();
}
void Parser::FillProductionTable ()
{
	FILE* fp = fopen("res\\grammer_number_transed.txt", "r");
	if (fp == NULL)
	{
		fprintf(stderr, "Error opening grammer file.\n");
		exit(-1);
	}
	int num;
	int row_index = -1, col_index = -1;
	memset(production_table_, 0, sizeof(production_table_));
	while (fscanf(fp, "%d", &num)== 1)
	{
		col_index = -1;
		production_table_[++row_index][++col_index] = num;
		while (fscanf(fp, "%d", &num) == 1)
			production_table_[row_index][++col_index] = num;
		fgetc(fp);
	}
	fclose(fp);
}

void Parser::FillDriveTable()
{
	// shift -> positive numbers, reduce -> negative numbers
	FILE* fp = fopen("res\\drive_table.txt", "r");
	if (fp == NULL)
	{
		fprintf(stderr, "Error opening action file.\n");
		exit(-1);
	}
	int i, j, val;
	memset(drive_table_, Fail, sizeof(drive_table_));
	while (fscanf(fp, "%d,%d,%d", &i, &j, &val) == 3)
		drive_table_[i][j] = val;
	fclose(fp);
}

/*
grammer production # encodes form 0,
shift -> positive 
reduce -> negative
state from 0
*/

char* GetLiteral(int index)
{
	if (index > T_REAL_TYPE && index < V_start)
		return var_list[index - T_REAL_TYPE - 1];
	else if (index >= V_start)
		return var_list[index - T_REAL_TYPE];
	//+1 is because there cannot see T_FINAL in compiler_workbench
	//V_start is 71 here, while 70 for compiler_workbench
	//Compiler workbench does leave a index deliberately for the symbol of #/$/final of input string
	//between the terminal part and nonterminal part
	else
		return keyword_list[index];
}
void Parser::Startup(std::vector<TokenItem*>& token_list)
{
	FILE* outfp = fopen("output\\reduction.txt", "w");
	int token_list_index = 0;
	TokenType crt_token = token_list[token_list_index]->token_type;
	int action, state_goto;
	state_stack_.Push(0);
	TokenItem* final = new TokenItem(T_FINAL);
	token_list.push_back(final);
	while (crt_token && state_stack_.Top() != Acc && state_stack_.Top() != Fail)
	{
		action = drive_table_[state_stack_.Top()][crt_token];
		if (action == Acc)
		{
			printf("Acc!\n");
			break;
		}
		if (action == Fail)
		{
			state_stack_.Push(Fail);
			fprintf(stderr, "Error parsing grammer file\n");
			break;
		}
		if (action > 0)
		{
			state_stack_.Push(action);
			var_stack_.Push(crt_token);		
			//printf("Shift in state %d and var %d \n", state_stack_.Top(), crt_token);
			if (token_list_index < token_list.size() - 1)
				crt_token = token_list[++token_list_index]->token_type;
		}
		else if (action < 0 )
		{			
			action *= -1;
			fprintf(outfp, "No. %d\t", action);
			fprintf(outfp, "%s => ", GetLiteral(production_table_[action][0]));
			int tmp = 1;
			while (production_table_[action][tmp] != 0 && !var_stack_.Empty() && !state_stack_.Empty())
			{
				fprintf(outfp, " %s ", GetLiteral(production_table_[action][tmp]));
				var_stack_.Pop();
				state_stack_.Pop();
				tmp++;
			}	
			fprintf(outfp, "\n");
			var_stack_.Push(production_table_[action][0]);		//suppose lpart is always char?
			state_goto = drive_table_[state_stack_.Top()][var_stack_.Top()];	// always operateable?
			if (state_goto == Fail)
			{
				fprintf(stderr, "Trapped in error state \n");
				break;
			}			
			state_stack_.Push(state_goto);
		}
		else
		{
			ErrorHandle();
			break;
		}
	}
	delete final;
	fclose(outfp);
}
void Parser::ErrorHandle()
{
	printf("Syntax error!\n");
}
