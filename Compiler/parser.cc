#include <stdio.h>
#include "common.h"
#include "hash_table.h"
#include "parser.h"

void Parser::Init()
{
	grammer_fp_ = fopen("grammer.txt", "r");
	production_table_tail_ = 0;
	
	char ll;
	char rr[kStringMaxLen];
	memset(rr, 0, sizeof(rr));
	while (fscanf(grammer_fp_, "%c,%s", &ll, &rr) == 2)
	{
		Production* pr = new Production(ll, rr);
		production_table_[production_table_tail_++] = pr;
		fgetc(grammer_fp_);
		memset(rr, 0, sizeof(rr));
	}
	fclose(grammer_fp_);

	for (int i = 0; i < GetArrayLen(char_set); i++)
		char_table_.Insert(&char_set[i], i);
	for (int i = 0; i < GetArrayLen(variable_set); i++)
		variable_table_.Insert(&variable_set[i], i);

}
int Parser::Decode(HashTable<char*, int> table, char* ch)
{
	return table.Find(ch);
}

void Parser::FillActionTable()
{
	//1<<31 + shift_to_state = action
	// shift to state 4 ->  1<<31 + 110B = 0x80000004
	action_table_[0]['a'] = 0x80000003;
	action_table_[0]['b'] = 0x80000004;
	action_table_[1]['#'] = Acc;
	action_table_[2]['a'] = 0x80000003;
	action_table_[2]['b'] = 0x80000004;
	action_table_[3]['a'] = 0x80000003;
	action_table_[3]['b'] = 0x80000004;

	action_table_[4]['a'] = 0x0003;
	action_table_[4]['b'] = 0x0003;
	action_table_[4]['#'] = 0x0003;

	action_table_[5]['a'] = 1;
	action_table_[5]['b'] = 1;
	action_table_[5]['#'] = 1;

	action_table_[6]['a'] = 2;
	action_table_[6]['b'] = 2;
	action_table_[6]['#'] = 2;
}
void Parser::FillGotoTable()
{
	goto_table_[0]['S'] = 1;
	goto_table_[0]['B'] = 2;
	goto_table_[2]['B'] = 5;
	goto_table_[3]['B'] = 6;
}
int ShouldShift(int action)
{
	//shift: 1<<31 + shift_to_state = action
	//reduce: no of the production to use

	if (action & (1 << 31))		
		return 1;
	else
		return 0;
}
void Parser::Startup()
{
	char test_case[kStringMaxLen] = "bab#\0";
	char* p = test_case;
	int action, state_goto;
	Production* production_to_use;
	state_stack_.Push(0);
	while (*p != '\0' && state_stack_.Top() != Acc && state_stack_.Top() != Fail)
	{
		action = action_table_[state_stack_.Top()][*p];
		if (action == Acc)
		{
			state_stack_.Push(Acc);
			break;
		}		
		if (action & (1 << 31))
		{
			state_stack_.Push(action - (1 << 31));
			char_stack_.Push((int)*p);		
			printf("Shift in state %d and char %c \n", state_stack_.Top(), *p);
			p++;
		}
		else if (action >= 0 )
		{
			production_to_use = production_table_[action - 1];			
			char *tmp = production_to_use->r_part;
			while (*tmp != '\0' && !char_stack_.Empty())
			{
				char_stack_.Pop();
				state_stack_.Pop();
				tmp++;
			}
			char_stack_.Push((int)(production_to_use->l_part));		//suppose lpart is always char?
			state_goto = goto_table_[state_stack_.Top()][(int)char_stack_.Top()];	// always operateable?
			state_stack_.Push(state_goto);
			printf("Reduce to char %c, go to state %d\n", (int)char_stack_.Top(), state_stack_.Top());
		}
		else
		{
			ErrorHandle();
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
