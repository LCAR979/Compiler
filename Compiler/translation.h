#ifndef __TRANSLATION_H__
#define __TRANSLATION_H__

#include "global.h"


typedef enum _Format
{
	F_Z_ASS_X_OP_Y, F_Z_ASS_OP_Y, F_Z_ASS_X,
	F_IF_E1_RELOP_E2_GOTO, F_GOTO, F_IF_E_GOTO,
	F_PARAM, F_CALL, 
}Format;

typedef struct _Production
{
	int left;
	char description[kMaxProductionRightLen];
	void(*f)(Item*);
}Production;

Production production[] = 
{
	{}
};

typedef struct _CodeLine
{
	Format fmt;
	char* arg1 = new char[INTER_CODE_OP_LEN];
	char* arg2 = new char[INTER_CODE_OP_LEN];
	char* op = new char[INTER_CODE_OP_LEN];
	char* result = new char[INTER_CODE_OP_LEN];
	/*char arg2[INTER_CODE_OP_LEN];
	char result[INTER_CODE_OP_LEN];
	char op[INTER_CODE_OP_LEN];*/
	_CodeLine(Format _fmt, char* _op, char* _arg1, char* _arg2, char* _result)
	{
		fmt = _fmt;
		strcpy(arg1, _arg1);
		strcpy(arg2, _arg2);
		strcpy(op, _op);
		strcpy(result, _result);
	}
}CodeLine;

typedef struct _Code
{
	int label[MAX_INTERCODE_LINE];
	std::vector<CodeLine> code;

	int crt_quad;		//the last valid quad index 
	_Code()
	{
		code.clear();
		crt_quad = -1;
	}
}Code;

Code intercode;


int GetNextQuad();
void GenCode(char* str);
char* GetNewTmp();

BackpatchListItem* MakeList(int i);
BackpatchListItem* Merge(BackpatchListItem* p, BackpatchListItem* q);
void BackPatch(BackpatchListItem* p, int i);
bool IsInList(BackpatchListItem* lst, int i);


void bool_exp_or(Item* it);
void bool_exp_and(Item* it);
void bool_exp_not(Item* it);
void bool_exp_parentheses(Item* it);
void bool_exp_relop(Item* it);
void bool_exp_true(Item* it);
void bool_exp_false(Item* it);
void M_quad(Item * it);

void statement_if_then(Item* it);
void statement_if_then_else(Item* it);
void statement_while(Item* it);
void N_if(Item* it);
void statement_begin_slist_end(Item* it);
void statement_assign(Item* it);
void slist_slist_quad_s(Item * it);
void slist_statement(Item *it);


void type_int(Item* it);
void type_int(Item* it);

void PrintInterCode()
{
	FILE *fp = fopen("intercode.txt", "w");
	std::vector<CodeLine>::iterator it;
	for (it = intercode.code.begin(); it != intercode.code.end(); it++)
	{
		switch (it->fmt)
		{
		case F_IF_E1_RELOP_E2_GOTO :
				fprintf(fp, "if %s %s %s then goto %s\n",
				it->arg1, it->op, it->arg2, it->result);
				break;
			case F_IF_E_GOTO:
				fprintf(fp, "if %s then goto %s\n", it->arg1, it->result);
				break;
			case F_GOTO:
				fprintf(fp, "goto %s\n", it->result);
				break;
			case F_PARAM:
				fprintf(fp, "param %s\n", it->result);
				break;
			case F_CALL:
				fprintf(fp, "call %s %s\n", it->arg1, it->arg2);
				break;
			case F_Z_ASS_X:
				fprintf(fp, "%s := %s\n", it->result, it->arg1);
				break;
			case F_Z_ASS_X_OP_Y:
				fprintf(fp, "%s := %s %s %s\n", it->result, it->arg1, it->op, it->arg2);
				break;
			case F_Z_ASS_OP_Y:
				fprintf(fp, "%s := %s %s\n", it->result, it->op, it->arg1);
				break;
		}
	}
}
#endif
