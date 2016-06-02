#ifndef __TRANSLATION_H__
#define __TRANSLATION_H__

#include "global.h"

typedef struct _Production
{
	int left;
	void(*f)(Item*);
}Production;

typedef struct _Code
{
	int label[MAX_ASM_LINE];
	char asmcode[MAX_ASM_LINE][MAX_CHR_PER_ASM_LINE];

	int crt_quad;
}Code;

Code code;

BackpatchListItem* MakeList(int i);
BackpatchListItem* Merge(BackpatchListItem* p, BackpatchListItem* q);
void BackPatch(BackpatchListItem* p, int i);
bool IsInList(BackpatchListItem* lst, int i);

#endif
