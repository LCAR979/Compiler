#ifndef __PRODUCTION_H__ 
#define __PRODUCTION_H__

#include "global.h" 
#include "symbol.h"
#include "translation.h"

//start => program ;
void start_program(Item* it){}

/*
program => T_PROGRAM T_ID T_LPAR identifier_list T_RPAR T_SEMICL
declarations M_quad
subprogram_declarations M_quad
compound_statement T_DOT M_quad;
*/
void program(Item* it)
{
	BackPatch(st.s[st.top - 2]->attr.nextlist, st.s[st.top]->attr.quad);
	BackPatch(st.s[st.top - 4]->attr.nextlist, st.s[st.top - 3]->attr.quad);
	BackPatch(st.s[st.top - 6]->attr.nextlist, st.s[st.top - 5]->attr.quad);
}

//identifier_list => T_ID;
void identifier_list_id(Item* it)
{
	it->attr.idn_vec.push_back(st.s[st.top]->attr.name_addr);
}

//identifier_list => identifier_list T_COMMA T_ID ;
void identifier_list_list_id(Item* it)
{
	it->attr.idn_vec = st.s[st.top - 2]->attr.idn_vec;
	it->attr.idn_vec.push_back(st.s[st.top]->attr.name_addr);
}

//declarations => T_VAR declaration T_SEMICL;
void declarations(Item* it){}

//declarations => ;
void declarations_null(Item* it){}

//declaration => declaration T_SEMICL identifier_list T_COLON type 
void declaration_declaration_identifier(Item* it)
{
	std::vector<string> vec = st.s[st.top - 2]->attr.idn_vec;
	for (std::vector<string>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		crt_sym_table->EnterSymbol(*it, st.s[st.top]->attr.type, NULL);
	}
}

//declaration => identifier_list T_COLON type;
void declaration_identifier(Item* it)
{
	std::vector<string> vec = st.s[st.top - 2]->attr.idn_vec;
	for (std::vector<string>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		crt_sym_table->EnterSymbol(*it, st.s[st.top]->attr.type, NULL);
	}
}

//type => standard_type;
void type_standard(Item* it)
{
	it->attr.type = st.s[st.top - 2]->attr.type;
}

//type => T_ARRAY T_LBRKPAR T_INT T_DOUBLE_DOT T_INT T_RBRKPAR T_OF standard_type;
void type_array(Item* it)		//Todo: Type
{
	it->attr.type = st.s[st.top - 2]->attr.type;
}

//standard_type => T_INT_TYPE;
void standard_type_int(Item* it)
{
	it->attr.type = (SymType)Int;
}

//standard_type => T_REAL_TYPE;
void standard_type_real(Item* it)
{
	it->attr.type = (SymType)Real;
}

//subprogram_declarations = > subprogram_declarations  M_quad subprogram_declaration T_SEMICL;
void subprogram_declarations_sub_declarations(Item* it)
{
	BackPatch(st.s[st.top - 3]->attr.nextlist, st.s[st.top - 2]->attr.quad);
	it->attr.nextlist = st.s[st.top - 1]->attr.nextlist;
}

//subprogram_declarations = >  ;
void subprogram_declarations_null(Item* it)
{
	it->attr.nextlist = NULL;
}

/*
subprogram_declaration = > subprogram_head
declarations
compound_statement;
*/
void subprogram_declaration_head_dec_compound(Item* it)
{
	SymTable* ret_to_sym_table = crt_sym_table->prev_sym_table;
	ret_to_sym_table->offset += crt_sym_table->offset;
	crt_sym_table = ret_to_sym_table;
	GenCode(CodeLine(F_RET, "ret", "", "", ""));
	it->attr.nextlist = st.s[st.top]->attr.nextlist;
}

//subprogram_head => T_FUNCTION M_function T_ID arguments T_COLON standard_type T_SEMICL ;
void subprogram_head_function(Item* it)		//Todo
{
	intercode.code[st.s[st.top - 5]->attr.quad].result = st.s[st.top - 4]->attr.name_addr;
}

//subprogram_head => T_PROCEDURE M_procedure T_ID arguments T_SEMICL;
void subprogram_head_procedure(Item* it)  //Todo
{
	intercode.code[st.s[st.top - 3]->attr.quad].result = st.s[st.top - 2]->attr.name_addr;
}

//arguments = > T_LPAR parameter_list T_RPAR;
void arguments_param_list(Item* it){}

//arguments = > ;
void arguments_null(Item* it){}

//parameter_list = > identifier_list T_COLON type ;
void parma_list_id_list(Item* it)
{
	std::vector<string> vec = st.s[st.top - 2]->attr.idn_vec;
	for (std::vector<string>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		crt_sym_table->EnterSymbol(*it, st.s[st.top]->attr.type, NULL);
	}
}

/*
parameter_list = >
parameter_list T_SEMICL identifier_list T_COLON type;
*/
void parma_list_parma_list_id(Item* it)
{
	std::vector<string> vec = st.s[st.top - 2]->attr.idn_vec;
	for (std::vector<string>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		crt_sym_table->EnterSymbol(*it, st.s[st.top]->attr.type, NULL);
	}
}

//compound_statement => T_BEGIN optional_statements T_END ;
void compound_statement(Item* it)
{
	it->attr.nextlist = st.s[st.top - 1]->attr.nextlist;
}

//optional_statements => statement_list;
void optional_statements_st_list(Item* it)
{
	it->attr.nextlist = st.s[st.top]->attr.nextlist;
}

//optional_statements => ;
void optional_statements_null(Item* it){}

//statement_list => statement;
void statement_list_statement(Item* it)
{
	it->attr.nextlist = st.s[st.top]->attr.nextlist;
}

//statement_list = > statement_list T_SEMICL M_quad statement;
void statement_list_st_list_statement(Item* it)
{
	BackPatch(st.s[st.top - 3]->attr.nextlist, st.s[st.top - 1]->attr.quad);
	it->attr.nextlist = st.s[st.top]->attr.nextlist;
}

//statement => variable T_ASS exp_item;
void statement_var_assign(Item* it)
{
	GenCode(CodeLine(F_Z_ASS_X, "", st.s[st.top]->attr.name_addr,
		"", st.s[st.top - 2]->attr.name_addr ));
}

//statement => procedure_statement;		 
void statement_procedure_st(Item* it)
{
	it->attr.nextlist = st.s[st.top]->attr.nextlist;
}

//statement = > compound_statement;
void statement_compound_st(Item* it)
{
	it->attr.nextlist = st.s[st.top]->attr.nextlist;
}

//statement => T_IF bool_exp T_THEN M_quad statement T_SEMICL;
void statement_if_then(Item* it)
{
	BackPatch(st.s[st.top - 4]->attr.truelist, st.s[st.top - 2]->attr.quad);
	it->attr.nextlist = Merge(st.s[st.top - 4]->attr.nextlist, st.s[st.top-1]->attr.nextlist);
}

//statement => T_IF bool_exp T_THEN M_quad statement T_ELSE N_IF M_quad statement;
void statement_if_then_else(Item* it)
{
	BackPatch(st.s[st.top - 7]->attr.truelist, st.s[st.top - 5]->attr.quad);
	BackPatch(st.s[st.top - 7]->attr.falselist, st.s[st.top - 1]->attr.quad);
	it->attr.nextlist = Merge(st.s[st.top - 4]->attr.nextlist,
		Merge(st.s[st.top - 2]->attr.nextlist, st.s[st.top]->attr.nextlist));
}

//N_IF => ;
void N_if(Item* it)
{
	it->attr.nextlist = MakeList(GetNextQuad());
	GenCode(CodeLine(F_GOTO, "", "", "", "%d"));
}

//M_quad = >;
void M_quad(Item* it)
{
	it->attr.quad = GetNextQuad();
}

//statement => T_WHILE M_quad bool_exp T_DO M_quad statement ;
void statement_while(Item* it)
{
	BackPatch(st.s[st.top]->attr.nextlist, st.s[st.top - 4]->attr.quad);
	BackPatch(st.s[st.top - 3]->attr.truelist, st.s[st.top - 1]->attr.quad);
	it->attr.nextlist = st.s[st.top - 3]->attr.falselist;

	CodeLine codeLine(F_GOTO, "", "", "", IntToStr(st.s[st.top - 4]->attr.quad));
	GenCode(codeLine);
}

//statement = > T_FOR variable M_quad T_ASS exp_item T_TO exp_item T_DO M_FOR statement;
void statement_for(Item* it)
{
	int M_again = st.s[st.top - 1]->attr.again;
	BackPatch(st.s[st.top]->attr.nextlist, M_again);
	GenCode(CodeLine(F_GOTO, "", "", "", IntToStr(M_again)));
	it->attr.nextlist = st.s[st.top - 1]->attr.nextlist;
}

//statement = > T_FOR variable M_quad T_ASS exp_item T_TO exp_item T_DO M_FOR statement;
//M_FOR => ;
void M_for(Item * it)
{
	it->attr.name_addr = st.s[st.top - 6]->attr.name_addr;	//variable
	GenCode(CodeLine(F_Z_ASS_X, "", st.s[st.top - 3]->attr.name_addr, "", it->attr.name_addr));
	string T1 = GetNewTmp();
	GenCode(CodeLine(F_Z_ASS_X, "", st.s[st.top - 1]->attr.name_addr, "", T1));

	int q = GetNextQuad();
	GenCode(CodeLine(F_GOTO, "", "", "", IntToStr(q + 2)));

	it->attr.again = q + 1;
	GenCode(CodeLine(F_Z_ASS_X_OP_Y, "+", it->attr.name_addr, "1", it->attr.name_addr));
	it->attr.nextlist = MakeList(GetNextQuad());
	GenCode(CodeLine(F_IF_E1_RELOP_E2_GOTO, ">", it->attr.name_addr, T1, "%d"));
}

//bool_exp => bool_exp_item T_OR M_quad bool_exp;
void bool_exp_or(Item* it)
{
	BackPatch(st.s[st.top - 3]->attr.falselist, st.s[st.top - 1]->attr.quad);
	it->attr.truelist = Merge(st.s[st.top - 3]->attr.truelist,
		st.s[st.top]->attr.truelist);
	it->attr.falselist = st.s[st.top]->attr.falselist;
}

//bool_exp => bool_exp_item T_AND M_quad bool_exp;
void bool_exp_and(Item* it)
{
	BackPatch(st.s[st.top - 3]->attr.truelist, st.s[st.top - 1]->attr.quad);
	it->attr.truelist = st.s[st.top]->attr.truelist;
	it->attr.falselist = Merge(st.s[st.top - 3]->attr.falselist,
		st.s[st.top]->attr.falselist);
}

//bool_exp => T_NOT bool_exp;
void bool_exp_not(Item* it)
{
	it->attr.truelist = st.s[st.top]->attr.falselist;
	it->attr.falselist = st.s[st.top]->attr.truelist;
}

//bool_exp = > T_LPAR bool_exp T_RPAR;
void bool_exp_parentheses(Item* it)
{
	it->attr.truelist = st.s[st.top - 1]->attr.truelist;
	it->attr.falselist = st.s[st.top - 1]->attr.falselist;
}

//bool_exp => bool_exp_item;
void bool_exp_boolexpitem(Item* it)
{
	it->attr.truelist = st.s[st.top]->attr.truelist;
	it->attr.falselist = st.s[st.top]->attr.falselist;
}

//bool_exp_item = > exp_item relop exp_item;
void bool_exp_relop(Item* it)
{
	int next_quad = GetNextQuad();
	it->attr.truelist = MakeList(next_quad);
	it->attr.falselist = MakeList(next_quad + 1);

	string E1_addr = st.s[st.top - 2]->attr.name_addr;
	string E2_addr = st.s[st.top]->attr.name_addr;
	string op_addr = st.s[st.top - 1]->attr.name_addr;

	CodeLine codeLine(F_IF_E1_RELOP_E2_GOTO, op_addr, E1_addr, E2_addr, "%d");
	GenCode(codeLine);
	GenCode(CodeLine(F_GOTO, "", "", "", "%d"));
}

//bool_exp_item = > T_TRUE;
void bool_exp_true(Item *it)
{
	it->attr.truelist = MakeList(GetNextQuad());
	CodeLine codeLine(F_GOTO, "", "", "", "%d");
	GenCode(codeLine);
}

//bool_exp_item = > T_FLASE;
void bool_exp_false(Item *it)
{
	it->attr.falselist = MakeList(GetNextQuad());
	CodeLine codeLine(F_GOTO, "", "", "", "%d");
	GenCode(codeLine);
}

//variable => T_ID
void var_id(Item* it)
{
	it->attr.name_addr = st.s[st.top]->attr.name_addr;
}

//variable => T_ID T_LBRKPAR exp_item T_RBRKPAR ;
void var_array_element(Item* it)	//Todo :array
{}

//procedure_statement => T_ID T_LPAR T_RPAR 
void procedure_st_func_call_no_param(Item* it)
{
	GenCode(CodeLine(F_CALL_PARAM_N, "", st.s[st.top - 2]->attr.name_addr,
		"0", ""));
}

//procedure_statement => T_ID T_LPAR expression_list T_RPAR ;
void procedure_st_func_call_param(Item* it)
{
	GenCode(CodeLine(F_CALL_PARAM_N, "", st.s[st.top - 3]->attr.name_addr,
		IntToStr(st.s[st.top - 1]->attr.idn_vec.size()), ""));
}

//expression_list => exp_item;
void expressionlist_expitem(Item* it)
{
	it->attr.idn_vec.push_back(st.s[st.top]->attr.name_addr);
	GenCode(CodeLine(F_PARAM, "", st.s[st.top]->attr.name_addr, "", ""));
}

//expression_list => expression_list T_COMMA exp_item ;
void expressionlist_expression_list(Item* it)
{
	it->attr.idn_vec = st.s[st.top-2]->attr.idn_vec;
	it->attr.idn_vec.push_back(st.s[st.top]->attr.name_addr);
	GenCode(CodeLine(F_PARAM, "", st.s[st.top]->attr.name_addr, "", ""));
}

//exp_item = > term;
void expitem_term(Item* it)
{
	it->attr.name_addr = st.s[st.top]->attr.name_addr;
}

//exp_item => sign term;
void expitem_signterm(Item* it)
{
	string new_tmp_name = GetNewTmp();
	GenCode(CodeLine(F_Z_ASS_OP_Y, ":=", st.s[st.top - 1]->attr.name_addr,
		st.s[st.top]->attr.name_addr, new_tmp_name));
	it->attr.name_addr = new_tmp_name;
}

//exp_item => exp_item addop term; 
void expitem_addop(Item* it)
{
	string new_tmp_name = GetNewTmp();
	GenCode(CodeLine(F_Z_ASS_X_OP_Y, st.s[st.top - 1]->attr.name_addr, 
		st.s[st.top - 2]->attr.name_addr, st.s[st.top]->attr.name_addr, new_tmp_name));
	it->attr.name_addr = new_tmp_name;
}

//term = > factor ;
void term_factor(Item* it)
{
	it->attr.name_addr = st.s[st.top]->attr.name_addr;
}

//term = > term mulop factor;
void term_mulop(Item* it)
{
	string new_tmp_name = GetNewTmp();
	GenCode(CodeLine(F_Z_ASS_X_OP_Y, st.s[st.top - 1]->attr.name_addr,
		st.s[st.top - 2]->attr.name_addr, st.s[st.top]->attr.name_addr, new_tmp_name));
	it->attr.name_addr = new_tmp_name;
}

void factor_id(Item* it)
{
	it->attr.name_addr = st.s[st.top]->attr.name_addr;
}

void factor_func_call_param(Item* it)
{}

void factor_num(Item* it)
{
	it->attr.name_addr = st.s[st.top]->attr.name_addr;
}

void factor_exp_item(Item* it)
{
	it->attr.name_addr = st.s[st.top-1]->attr.name_addr;
}

void pass_op(Item* it)
{
	it->attr.name_addr = GetLiteral(st.s[st.top]->attr.token_type);
}

void num_int(Item* it)
{
	it->attr.name_addr = string(IntToStr(st.s[st.top]->attr.int_val));
	it->attr.type = (SymType)Int;
}

void num_real(Item* it)
{
	it->attr.name_addr = string(RealToStr(st.s[st.top]->attr.real_val));
	it->attr.type = (SymType)Real;
}

void M_function(Item* it)
{
	it->attr.quad = GetNextQuad();
	SymTable* new_sym_table = new SymTable(crt_sym_table);
	crt_sym_table->EnterSymbol("SymTable", (SymType)SymTableType, new_sym_table);
	crt_sym_table = new_sym_table;
	GenCode(CodeLine(F_TITLE_FUNC, "", "", "", "-"));
}
void M_procedure(Item* it)
{
	it->attr.quad = GetNextQuad();
	SymTable* new_sym_table = new SymTable(crt_sym_table);
	crt_sym_table->EnterSymbol("SymTable", (SymType)SymTableType, new_sym_table);
	crt_sym_table = new_sym_table;
	GenCode(CodeLine(F_TITLE_PROCEDURE, "", "", "", "-"));
}
#endif 
