#ifndef __TRANSLATION_H__
#define __TRANSLATION_H__

#include "global.h"

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

typedef struct _Code
{
	int label[MAX_INTERCODE_LINE];
	char code[MAX_INTERCODE_LINE][MAX_CHR_PER_INTERCODE_LINE];

	int crt_quad;
}Code;

Code intercode;

BackpatchListItem* MakeList(int i);
BackpatchListItem* Merge(BackpatchListItem* p, BackpatchListItem* q);
void BackPatch(BackpatchListItem* p, int i);
bool IsInList(BackpatchListItem* lst, int i);

//backpatching 
void bool_exp_or(Item*);
void bool_exp_and(Item*);
void bool_exp_no(Item*);
void bool_exp_parentheses(Item*);
void bool_exp_relop(Item*);

#endif
