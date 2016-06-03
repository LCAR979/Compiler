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
		sprintf(code_line_buffer, intercode.code[tmp->quad].result, i);
		strcpy(intercode.code[tmp->quad].result, code_line_buffer);
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




void statement_begin_slist_end(Item* it)
{
	it->attr.nextlist = st.s[st.top - 1].attr.nextlist;
}

void statement_assign(Item* it)
{
	it->attr.nextlist = NULL;
}

void slist_slist_quad_s(Item * it)
{
	BackPatch(st.s[st.top - 3].attr.nextlist, st.s[st.top - 1].attr.quad);
	it->attr.nextlist = st.s[st.top].attr.nextlist;
}

void slist_statement(Item *it)
{
	it->attr.nextlist = st.s[st.top].attr.nextlist;
}

/*		arith		*/	
void exp_item_addop(Item* it)
{
	char *E_addr = GetNewTmp();
	char* E1_addr = st.s[st.top - 2].attr.name_addr;
	char* E2_addr = st.s[st.top].attr.name_addr;
	int op_type = st.s[st.top - 1].attr.type;

	CodeLine codeLine(F_Z_ASS_X_OP_Y, GetLiteral(op_type), E1_addr, E2_addr, E_addr);
	GenCode(codeLine);
}

void exp_item_mulop(Item* it)
{
	char *E_addr = GetNewTmp();
	char* E1_addr = st.s[st.top - 2].attr.name_addr;
	char* E2_addr = st.s[st.top].attr.name_addr;
	int op_type = st.s[st.top - 1].attr.type;

	CodeLine codeLine(F_Z_ASS_X_OP_Y, GetLiteral(op_type), E1_addr, E2_addr, E_addr);
	GenCode(codeLine);
}

void exp_item_sign(Item* it)
{
	char *E_addr = GetNewTmp();
	char* E2_addr = st.s[st.top].attr.name_addr;

	CodeLine codeLine(F_Z_ASS_OP_Y, "", "", E2_addr, E_addr);
	
	if (st.s[st.top - 1].attr.type == T_SUB)
		codeLine.op = "-";

	GenCode(codeLine);
}

void exp_item_parentheses(Item* it)
{
	it->attr.addr = st.s[st.top - 1].attr.addr;
}

void exp_item_id(Item* it)
{

}

/*		call and ret */
void statement_call_id_exp_list(Item* it)
{
	CodeLine codeLine(F_PARAM, "", "", "", "");
	int n = param_queue.size();

	while (!param_queue.empty())
	{
		codeLine.result = param_queue.front();
		GenCode(codeLine);
	}

	codeLine.fmt = F_CALL;
	codeLine.arg1 = st.s[st.top - 3].attr.name_addr;
	codeLine.arg2 = IntToStr(n);
	GenCode(codeLine);
}

void type_int(Item* it)
{
	it->attr.type = T_INT;
	it->attr.width = 4;
}

void type_real(Item* it)
{
	it->attr.type = T_REAL;
	it->attr.width = 8;
}
