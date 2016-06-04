#include <queue>
#include "global.h"
#include "translation.h"
#include "general_stack.h"

std::queue<char*> param_queue;

int GetNextQuad()
{
	return intercode.crt_quad + 1;
}

void GenCode(CodeLine& codeLine)
{
	intercode.crt_quad++;
	intercode.code.push_back(codeLine);
}

char tmp_var_buf[5];
char* GetNewTmp()
{
	char* fmt = "t%d";
	memset(tmp_var_buf, 0, sizeof(tmp_var_buf));
	sprintf(tmp_var_buf, fmt, tmp_count);
	tmp_count++;
	return tmp_var_buf;
}

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
	if (p == NULL)
		return;

	char code_line_buffer[INTER_CODE_OP_LEN];
	BackpatchListItem* tmp;
	for (tmp = p; tmp != NULL; tmp = tmp->next)
	{
		sprintf(code_line_buffer, "%d", i);
		intercode.code[tmp->quad].result = string(code_line_buffer);
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


