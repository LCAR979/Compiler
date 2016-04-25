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
void Parser::Startup(std::vector<TokenItem*>& token_list)
{
	int token_list_index = 0;
	TokenType crt_token = token_list[token_list_index]->token_type;
	int action, state_goto;
	state_stack_.Push(0);
	while (crt_token && state_stack_.Top() != Acc && state_stack_.Top() != Fail)
	{
		action = drive_table_[state_stack_.Top()][crt_token];
		if (action == Acc)
		{
			state_stack_.Push(Acc);
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
			printf("Shift in state %d and var %d \n", state_stack_.Top(), crt_token);
			crt_token = token_list[++token_list_index]->token_type;
		}
		else if (action < 0 )
		{			
			action *= -1;
			int tmp = 1;
			while (production_table_[action][tmp] != 0 && !var_stack_.Empty() && !state_stack_.Empty())
			{
				var_stack_.Pop();
				state_stack_.Pop();
				tmp++;
			}
			
			var_stack_.Push(production_table_[action][0]);		//suppose lpart is always char?
			state_goto = drive_table_[state_stack_.Top()][var_stack_.Top()];	// always operateable?
			if (state_goto == -1)
			{
				fprintf(stderr, "Trapped in error state \n");
				break;
			}			
			state_stack_.Push(state_goto);
			printf("Reduced using No. %d production, go to state %d\n", action, state_stack_.Top());
		}
		else
		{
			fprintf(stderr, "Action = 0 \n");
			break;
		}
	}
	if (state_stack_.Top() == Acc)
		printf("Acc!\n");
}
void Parser::ErrorHandle()
{
	printf("Syntax error!\n");
}
