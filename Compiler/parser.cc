#include <stdio.h>
#include "global.h"
#include "hash_table.h"
#include "parser.h"
#include "translation.h"

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
	for (int i = 0; i < kMaxStateNum; i++){
		for (int j = 0; j < kMaxVarNum; j++)
			drive_table_[i][j] = Fail;
	}

	while (fscanf(fp, "%d,%d,%d", &i, &j, &val) == 3)
		drive_table_[i][j] = val;
	fclose(fp);
}

/*
grammer production # encodes form 0,
shift -> 
reduce -> negative
state from 0
*/


void Parser::Startup()
{
	FILE* outfp = fopen("output\\reduction.txt", "w");

	size_t token_vec_index = 0;
	TokenType crt_token = token_vec[token_vec_index].type;

	int action, state_goto;

	Item it;
	it.state = 0;
	it.attr.type = T_FINAL;
	st.Push(it);

	while (crt_token && st.Top().state != Acc && st.Top().state != Fail)
	{
		action = drive_table_[st.Top().state][crt_token];
		if (action == Acc)
		{
			printf("Acc!\n");
			PrintInterCode();
			break;
		}
		if (action == Fail)
		{
			it.state = Fail;
			st.Push(it);
			fprintf(stderr, "Error parsing grammer file\n");
			break;
		}
		if (action > 0)
		{
			it.state = action;
			it.attr.type = token_vec[token_vec_index].type;
			it.attr.name_addr = token_vec[token_vec_index].val.name_addr;
			it.attr.int_val = token_vec[token_vec_index].val.int_val;
			it.attr.real_val = token_vec[token_vec_index].val.real_val;
			st.Push(it);
	
			//printf("Shift in state %d and var %d \n", st.st.Top(), crt_token);
			if (token_vec_index < token_vec.size() - 1)
				crt_token = token_vec[++token_vec_index].type;
			else
				crt_token = T_FINAL;
		}
		else if (action < 0 )
		{			
			action *= -1;
			fprintf(outfp, "No. %d\t", action);
			fprintf(outfp, "%s => ", GetLiteral(production_table_[action][0]));
			printf("%s => ", GetLiteral(production_table_[action][0]));
			int tmp = 1;
			while (production_table_[action][tmp] != 0 && !st.Empty())
			{
				fprintf(outfp, " %s ", GetLiteral(production_table_[action][tmp]));
				printf(" %s ", GetLiteral(production_table_[action][tmp]));
				st.Pop();
				tmp++;
			}	
			fprintf(outfp, "\n");
			printf("\n");
			
			it.attr.type = (TokenType)production_table_[action][0];	
			state_goto = drive_table_[st.Top().state][it.attr.type];
			it.state = state_goto;
			if (state_goto == Fail)
			{
				ErrorHandle("Trapped in error state \n");
				break;
			}			
			st.Push(it);
			//printf("Shift in state %d and var %d \n", it.state, it.attr.type);
		}
		else
		{
			ErrorHandle("Unknown error");
			break;
		}
	}
	fclose(outfp);
}