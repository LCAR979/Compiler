#ifndef __PRODUCTION_H__ 
#define __PRODUCTION_H__

#include "global.h" 
#include "translation.h"

void start_program(Item* it)	//
{}

void program(Item* it)
{}

void identifier_list_id(Item* it)
{
	it->attr.param_vec.push_back(st.s[st.top].attr.name_addr);
}

void identifier_list_list_id(Item* it)
{
	it->attr.param_vec = st.s[st.top - 2].attr.param_vec;
	it->attr.param_vec.push_back(st.s[st.top].attr.name_addr);
}

void declarations(Item* it)		//
{}

void declarations_null(Item* it)	//
{}

void declaration_declaration_identifier(Item* it)
{}

void declaration_identifier(Item* it)
{}

void type_standard(Item* it)
{}

void type_array(Item* it)
{}

void standard_type_int(Item* it)
{}

void standard_type_real(Item* it)
{}

void subprogram_declarations_sub_declarations(Item* it)
{}

void subprogram_declarations_null(Item* it)
{}

void subprogram_declaration_head_dec_compound(Item* it)
{}

void subprogram_head_function(Item* it)
{}

void subprogram_head_procedure(Item* it)
{}

void arguments_param_list(Item* it)
{}

void arguments_null(Item* it)
{}

void parma_list_id_list(Item* it)
{}

void parma_list_parma_list_id(Item* it)
{}

void compound_statement(Item* it)
{}

void optional_statements_st_list(Item* it)
{}

void optional_statements_null(Item* it)
{}

void statement_list_statement(Item* it)
{}

void statement_list_st_list_statement(Item* it)
{}

void statement_var_assign(Item* it)
{}

void statement_procedure_st(Item* it)
{}

void statement_compound_st(Item* it)
{}

void bool_exp_boolexpitem(Item* it)
{}

void var_id(Item* it)
{}

void var_array_element(Item* it)
{}

void procedure_st_func_call_no_param(Item* it)
{}

void procedure_st_func_call_param(Item* it)
{}

void expressionlist_expitem(Item* it)
{}

void expressionlist_expression_list(Item* it)
{}

void expitem_term(Item* it)
{}

void expitem_signterm(Item* it)
{}

void expitem_addop(Item* it)
{}

void term_factor(Item* it)
{}

void term_mulop(Item* it)
{}

void factor_id(Item* it)
{}

void factor_func_call_param(Item* it)
{}

void factor_num(Item* it)
{}

void factor_exp_item(Item* it)
{}

void pass_op(Item* it)
{
	it->attr.name_addr = GetLiteral(st.s[st.top].attr.type);
}

void num_int(Item* it)
{
	it->attr.int_val = st.s[st.top].attr.int_val;
}

void num_real(Item* it)
{}

/*		bool		*/
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

void bool_exp_not(Item* it)
{
	it->attr.truelist = st.s[st.top].attr.falselist;
	it->attr.falselist = st.s[st.top].attr.truelist;
}

void bool_exp_parentheses(Item* it)
{
	it->attr.truelist = st.s[st.top - 1].attr.truelist;
	it->attr.falselist = st.s[st.top - 1].attr.falselist;
}

void bool_exp_true(Item *it)
{
	it->attr.truelist = MakeList(GetNextQuad());
	CodeLine codeLine(F_GOTO, "", "", "", "-");
	GenCode(codeLine);
}

void bool_exp_false(Item *it)
{
	it->attr.falselist = MakeList(GetNextQuad());
	CodeLine codeLine(F_GOTO, "", "", "", "-");
	GenCode(codeLine);
}

void bool_exp_relop(Item* it)
{
	int next_quad = GetNextQuad();
	it->attr.truelist = MakeList(next_quad);
	it->attr.falselist = MakeList(next_quad + 1);

	char* E1_addr = st.s[st.top - 2].attr.name_addr;
	char* E2_addr = st.s[st.top].attr.name_addr;
	int op_type = st.s[st.top - 1].attr.type;

	CodeLine codeLine(F_IF_E1_RELOP_E2_GOTO, GetLiteral(op_type), E1_addr, E2_addr, "-");
	GenCode(codeLine);
}

/*		control statement	*/
void statement_if_then(Item* it)
{
	BackPatch(st.s[st.top - 3].attr.truelist, st.s[st.top - 1].attr.quad);
	it->attr.nextlist = Merge(st.s[st.top - 3].attr.nextlist, st.s[st.top].attr.nextlist);
}

void statement_if_then_else(Item* it)
{
	BackPatch(st.s[st.top - 7].attr.truelist, st.s[st.top - 5].attr.quad);
	BackPatch(st.s[st.top - 7].attr.falselist, st.s[st.top - 1].attr.quad);
	it->attr.nextlist = Merge(st.s[st.top - 4].attr.nextlist,
		Merge(st.s[st.top - 3].attr.nextlist, st.s[st.top].attr.nextlist));
}

void statement_while(Item* it)
{
	BackPatch(st.s[st.top].attr.nextlist, st.s[st.top - 4].attr.quad);
	BackPatch(st.s[st.top - 3].attr.truelist, st.s[st.top - 1].attr.quad);
	it->attr.nextlist = st.s[st.top - 3].attr.falselist;

	CodeLine codeLine(F_GOTO, "", "", "", IntToStr(st.s[st.top - 4].attr.quad));
	GenCode(codeLine);
}

void statement_for(Item* it)
{
	int M_again = st.s[st.top - 1].attr.again;
	BackPatch(st.s[st.top].attr.nextlist, M_again);
	GenCode(CodeLine(F_GOTO, "", "", "", IntToStr(M_again)));
	it->attr.nextlist = st.s[st.top - 1].attr.nextlist;
}

void M_for(Item * it)
{
	it->attr.addr = st.s[st.top - 8].attr.addr;
	GenCode(CodeLine(F_Z_ASS_X, "", st.s[st.top - 6].attr.name_addr, "", it->attr.name_addr));
	char* T1 = GetNewTmp();
	GenCode(CodeLine(F_Z_ASS_X, "", st.s[st.top - 4].attr.name_addr, "", T1));
	char* T2 = GetNewTmp();
	GenCode(CodeLine(F_Z_ASS_X, "", st.s[st.top - 2].attr.name_addr, "", T2));

	int q = GetNextQuad();
	GenCode(CodeLine(F_GOTO, "", "", "", IntToStr(q + 2)));

	it->attr.again = q + 1;
	GenCode(CodeLine(F_Z_ASS_X_OP_Y, "+", it->attr.name_addr, T2, it->attr.name_addr));
	it->attr.nextlist = MakeList(GetNextQuad());
	GenCode(CodeLine(F_IF_E_GOTO, ">", it->attr.name_addr, T1, "-"));
}

void N_if(Item* it)
{
	it->attr.nextlist = MakeList(GetNextQuad());
	CodeLine codeLine(F_GOTO, "", "", "", "-");
	GenCode(codeLine);
}

void M_quad(Item* it)
{
	it->attr.quad = GetNextQuad();
}


#endif 
