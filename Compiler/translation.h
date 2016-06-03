#ifndef __TRANSLATION_H__
#define __TRANSLATION_H__

#include "global.h"

int GetNextQuad();
void GenCode(CodeLine& codeLine);
char* GetNewTmp();

BackpatchListItem* MakeList(int i);
BackpatchListItem* Merge(BackpatchListItem* p, BackpatchListItem* q);
void BackPatch(BackpatchListItem* p, int i);
bool IsInList(BackpatchListItem* lst, int i);

//void statement_begin_slist_end(Item* it);
//void statement_assign(Item* it);
//void slist_slist_quad_s(Item * it);
//void slist_statement(Item *it);

#endif
