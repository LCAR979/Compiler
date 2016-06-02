#include "global.h"
#include "translation.h"
#include "general_stack.h"

int GetNextQuad()
{
	return intercode.crt_quad + 1;
}

void GenCode(char* str)
{
	strcpy(intercode.code[intercode.crt_quad], str);
	intercode.crt_quad++;
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
	char code_line_buffer[MAX_CHR_PER_INTERCODE_LINE];
	BackpatchListItem* tmp;
	for (tmp = p; tmp != NULL; tmp = tmp->next)
	{
		sprintf(code_line_buffer, intercode.code[tmp->quad], i);
		strcpy(intercode.code[tmp->quad], code_line_buffer);
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

void bool_exp_or(Item* it)
{
	BackPatch(st.s[st.top - 3].attr.falselist, st.s[st.top - 1].attr.quad);
	it->attr.truelist = Merge(st.s[st.top - 3].attr.truelist,
		st.s[st.top].attr.truelist);
	it->attr.falselist = st.s[st.top].attr.falselist;
}

void bool_exp_and(Item* it)
{
	BackPatch(st.s[st.top - 3].attr.truelist, st.s[st.top - 1].attr.quad);
	it->attr.truelist = st.s[st.top].attr.truelist;
	it->attr.falselist = Merge(st.s[st.top - 3].attr.falselist,
		st.s[st.top].attr.falselist);
}

void bool_exp_no(Item* it)
{
	it->attr.truelist = st.s[st.top].attr.falselist;
	it->attr.falselist = st.s[st.top].attr.truelist;
}
void bool_exp_parentheses(Item* it)
{
	it->attr.truelist = st.s[st.top - 1].attr.truelist;
	it->attr.falselist = st.s[st.top - 1].attr.falselist;
}

void bool_exp_relop(Item* it)
{
	int next_quad = GetNextQuad();
	it->attr.truelist = MakeList(next_quad);
	it->attr.falselist = MakeList(next_quad + 1);

	char* E1_addr = st.s[st.top - 2].attr.token.val.name_addr;
	char* E2_addr = st.s[st.top].attr.token.val.name_addr;
	int op_index = st.s[st.top].attr.token.type;

	char buf[MAX_CHR_PER_INTERCODE_LINE];
	char *format = "if %s %s %s goto %s";
	sprintf(buf, format, E1_addr, GetLiteral(op_index), E2_addr, "%d");
	GenCode(buf);
	GenCode("goto %d");
}

void bool_exp_true(Item *it)
{
	it->attr.truelist = MakeList(GetNextQuad());
	GenCode("goto %d");
}

void bool_exp_false(Item *it)
{
	it->attr.falselist = MakeList(GetNextQuad());
	GenCode("goto %d");
}

void type_int(Item* it)
{
	it->attr.token.type = T_INT;
	it->attr.width = 4;
}

void type_real(Item* it)
{
	it->attr.token.type = T_REAL;
	it->attr.width = 8;
}