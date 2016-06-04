#include <string.h>
#include <vector>
#include "global.h"
#include "production.h"
#include "translation.h"

int line_number = 0;
bool fatel_error = false;
int tmp_count = 0;

std::map<std::string, int> keyword_table;

char token_name_arr[kTokenNameArrLen];
int token_name_arr_tail = 0;


std::vector<TokenItem> token_vec;
std::vector<ErrorItem> error_vec;
GeneralStack<Item*> st;

Code intercode;

void ErrorHandle(char* err_msg)
{
	error_vec.push_back(ErrorItem(line_number, err_msg));
}

void ErrorPrint()
{
	int error_count = 0;
	FILE* error_fp = fopen("output\\error.txt", "w");
	std::vector<ErrorItem>::iterator it;
	for (it = error_vec.begin(); it != error_vec.end(); it++)
	{
		printf("Line %d, %s\n", it->line, it->description);
		fprintf(error_fp, "Line %d, %s\n", it->line, it->description);
		error_count++;
	}

	if (error_count != 0)
		printf(" Error count %d\n", error_count);
	else
	{
		printf("No token error\n");
	}
	fclose(error_fp);
}

char* GetLiteral(int index)
{
	return word_list[index];
}

char tmp_int_buf[INTER_CODE_OP_LEN];
char* IntToStr(int i)
{
	memset(tmp_int_buf, 0, sizeof(tmp_int_buf));
	char *format = "%d";
	sprintf(tmp_int_buf, format, i);
	return tmp_int_buf;
}

char tmp_real_buf[INTER_CODE_OP_LEN*2];
char* RealToStr(double i)
{
	memset(tmp_real_buf, 0, sizeof(tmp_real_buf));
	char *format = "%d";
	sprintf(tmp_real_buf, format, i);
	return tmp_real_buf;
}

void PrintIntercode()
{
	FILE *fp = fopen("output\\intercode.txt", "w");
	std::vector<CodeLine>::iterator it;
	int line_count = 0;
	for (it = intercode.code.begin(); it != intercode.code.end(); it++)
	{
		fprintf(fp, "%d\t", line_count++);
		fprintf(fp, "(%8s, %8s, %8s, %8s)\t\t\t", it->op.c_str(), it->arg1.c_str(),
			it->arg2.c_str(), it->result.c_str());
		switch (it->fmt)
		{
		case F_IF_E1_RELOP_E2_GOTO:
			fprintf(fp, "if %s %s %s then goto %s",
				it->arg1.c_str(), it->op.c_str(), it->arg2.c_str(), it->result.c_str());
			break;
		case F_IF_E_GOTO:
			fprintf(fp, "if %s then goto %s", it->arg1.c_str(), it->result.c_str());
			break;
		case F_GOTO:
			fprintf(fp, "goto %s", it->result.c_str());
			break;
		case F_PARAM:
			fprintf(fp, "param %s", it->arg1.c_str());
			break;
		case F_CALL_PARAM_N:
			fprintf(fp, "call %s %s", it->arg1.c_str(), it->arg2.c_str());
			break;
		case F_Z_ASS_X:
			fprintf(fp, "%s := %s", it->result.c_str(), it->arg1.c_str());
			break;
		case F_Z_ASS_X_OP_Y:
			fprintf(fp, "%s := %s %s %s", it->result.c_str(), it->arg1.c_str(), it->op.c_str(), it->arg2.c_str());
			break;
		case F_Z_ASS_OP_Y:
			fprintf(fp, "%s := %s %s", it->result.c_str(), it->op.c_str(), it->arg1.c_str());
			break;
		case F_TITLE_FUNC:
			fprintf(fp, "FUNC %s", it->result.c_str());
			break;
		case F_TITLE_PROCEDURE:
			fprintf(fp, "PROC %s", it->result.c_str());
			break;
		case F_RET:
			fprintf(fp, "RET");
			break;
		}
		fprintf(fp, "\n");
		
	}
	fclose(fp);
}

 extern Production production[] =
 {
	 { 0, "start -> program ", start_program },
	 { 1, "program -> T_PROGRAM T_ID T_LPAR identifier_list T_RPAR T_SEMICL   declarations M_quad   subprogram_declarations M_quad   compound_statement T_DOT M_quad", program },
	 { 2, "identifier_list -> T_ID ", identifier_list_id },
	 { 3, "identifier_list -> identifier_list T_COMMA T_ID ", identifier_list_list_id },
	 { 4, "declarations -> T_VAR declaration T_SEMICL ", declarations },
	 { 5, "declarations -> ", declarations_null },
	 { 6, "declaration -> declaration T_SEMICL identifier_list T_COLON type ", declaration_declaration_identifier },
	 { 7, "declaration ->   identifier_list T_COLON type ", declaration_identifier },
	 { 8, "type -> standard_type ", type_standard },
	 { 9, "type ->T_ARRAY T_LBRKPAR T_INT T_DOUBLE_DOT T_INT T_RBRKPAR T_OF standard_type ", type_array },
	 { 10, "standard_type -> T_INT_TYPE ", standard_type_int },
	 { 11, "standard_type -> T_REAL_TYPE ", standard_type_real },
	 { 12, "subprogram_declarations -> subprogram_declarations M_quad   subprogram_declaration T_SEMICL ", subprogram_declarations_sub_declarations },
	 { 13, "subprogram_declarations -> ", subprogram_declarations_null },
	 { 14, "subprogram_declaration -> subprogram_head  declarations  compound_statement ", subprogram_declaration_head_dec_compound },
	 { 15, "subprogram_head -> T_FUNCTION M_function T_ID arguments T_COLON standard_type T_SEMICL ", subprogram_head_function },
	 { 16, "subprogram_head ->   T_PROCEDURE M_procedure T_ID arguments T_SEMICL ", subprogram_head_procedure },
	 { 17, "arguments -> T_LPAR parameter_list T_RPAR ", arguments_param_list },
	 { 18, "arguments -> ", arguments_null },
	 { 19, "parameter_list -> identifier_list T_COLON type ", parma_list_id_list },
	 { 20, "parameter_list ->  parameter_list T_SEMICL identifier_list T_COLON type ", parma_list_parma_list_id },
	 { 21, "compound_statement -> T_BEGIN optional_statements T_END ", compound_statement },
	 { 22, "optional_statements -> statement_list ", optional_statements_st_list },
	 { 23, "optional_statements -> ", optional_statements_null },
	 { 24, "statement_list -> statement", statement_list_statement },
	 { 25, "statement_list -> statement_list T_SEMICL M_quad statement ", statement_list_st_list_statement },
	 { 26, "statement -> variable T_ASS exp_item", statement_var_assign },
	 { 27, "statement -> procedure_statement", statement_procedure_st },
	 { 28, "statement -> compound_statement", statement_compound_st },
	 { 29, "statement -> T_IF bool_exp T_THEN M_quad statement T_SEMICL", statement_if_then },
	 { 30, "statement -> T_IF bool_exp T_THEN M_quad statement T_ELSE N_IF M_quad statement", statement_if_then_else },
	 { 31, "N_IF -> ", N_if },
	 { 32, "M_quad -> ", M_quad },
	 { 33, "statement -> T_WHILE M_quad bool_exp T_DO M_quad statement ", statement_while },
	 { 34, "statement -> T_FOR variable M_quad T_ASS exp_item T_TO exp_item T_DO M_FOR statement", statement_for },
	 { 35, "M_FOR -> ", M_for },
	 { 36, "bool_exp -> bool_exp_item T_OR M_quad bool_exp", bool_exp_or },
	 { 37, "bool_exp -> bool_exp_item T_AND M_quad bool_exp", bool_exp_and },
	 { 38, "bool_exp -> T_NOT bool_exp", bool_exp_not },
	 { 39, "bool_exp -> T_LPAR bool_exp T_RPAR", bool_exp_parentheses },
	 { 40, "bool_exp -> bool_exp_item", bool_exp_boolexpitem },
	 { 41, "bool_exp_item -> exp_item relop exp_item", bool_exp_relop },
	 { 42, "bool_exp_item -> T_TRUE", bool_exp_true },
	 { 43, "bool_exp_item -> T_FLASE", bool_exp_false },
	 { 44, "variable -> T_ID ", var_id },
	 { 45, "variable -> T_ID T_LBRKPAR exp_item T_RBRKPAR ", var_array_element },
	 { 46, "procedure_statement -> T_ID T_LPAR T_RPAR ", procedure_st_func_call_no_param },
	 { 47, "procedure_statement -> T_ID T_LPAR expression_list T_RPAR ", procedure_st_func_call_param },
	 { 48, "expression_list -> exp_item ", expressionlist_expitem },
	 { 49, "expression_list -> expression_list T_COMMA exp_item ", expressionlist_expression_list },
	 { 50, "exp_item -> term ", expitem_term },
	 { 51, "exp_item -> sign term ", expitem_signterm },
	 { 52, "exp_item -> exp_item addop term ", expitem_addop },
	 { 53, "term -> factor ", term_factor },
	 { 54, "term -> term mulop factor ", term_mulop },
	 { 55, "factor -> T_ID ", factor_id },
	 { 56, "factor -> T_ID T_LPAR expression_list T_RPAR ", factor_func_call_param },
	 { 57, "factor -> num ", factor_num },
	 { 58, "factor -> T_LPAR exp_item T_RPAR  ", factor_exp_item },
	 { 59, "sign -> T_ADD", pass_op },
	 { 60, "sign -> T_SUB", pass_op },
	 { 61, "addop -> T_ADD", pass_op },
	 { 62, "addop -> T_SUB", pass_op },
	 { 63, "mulop -> T_MUL", pass_op },
	 { 64, "mulop -> T_DIV", pass_op },
	 { 65, "relop -> T_EQL", pass_op },
	 { 66, "relop -> T_NEQ", pass_op },
	 { 67, "relop -> T_GT", pass_op },
	 { 68, "relop -> T_GTE", pass_op },
	 { 69, "relop -> T_LT", pass_op },
	 { 70, "relop -> T_LTE", pass_op },
	 { 71, "num -> T_INT", num_int },
	 { 72, "num -> T_REAL", num_real },
	 { 73, "M_function -> ", M_function },
	 { 74, "M_procedure -> ", M_procedure },

 };
