#include "translation.h"

BackpatchListItem* MakeList(int i)
{
	BackpatchListItem* new_item = new BackpatchListItem();
	new_item->quad = i;
	new_item->next = NULL;
	return new_item;
}

bool IsInList(BackpatchListItem* lst, int i)
{
	for (BackpatchListItem* tmp = lst; tmp != NULL; tmp = tmp->next)
	{
		if (tmp->quad == i)
			return true;
	}
	return false;
}

BackpatchListItem* Merge(BackpatchListItem* p, BackpatchListItem* q)
{
	BackpatchListItem* tmp;
	BackpatchListItem* p_tail;
	if (p == NULL)
		return q;

	for (tmp = p; tmp->next != NULL; tmp = tmp->next);
	p_tail = tmp;
	for (tmp = q; tmp != NULL; tmp = tmp->next)
	{
		if (IsInList(p, tmp->quad))
			continue;
		else
		{
			p_tail->next = tmp;
			p_tail = tmp;
			//p_tail->next = NULL;
		}
	}
	return p;
}

void BackPatch(BackpatchListItem* p, int i)
{
	char asm_line_buffer[MAX_CHR_PER_ASM_LINE];
	BackpatchListItem* tmp;
	for (tmp = p; tmp != NULL; tmp = tmp->next)
	{
		sprintf(asm_line_buffer, code.asmcode[tmp->quad], i);
		strcpy(code.asmcode[tmp->quad], asm_line_buffer);
	}
	
	BackpatchListItem* prev = p;
	tmp = p->next;
	while (tmp)
	{
		delete prev;
		prev = tmp;
		tmp = tmp->next;		
	}

	delete prev;
}
