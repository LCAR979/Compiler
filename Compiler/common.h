#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <stdlib.h>

typedef enum TokenType
{
	T_AND, T_ARRAY, T_BEGIN, T_CASE, T_CONST, T_DIV, T_DO, T_DOWNTO, T_ELSE, T_END,
	T_FILE, T_FOR, T_FUNCTION, T_GOTO, T_IF, T_IN, T_LABEL, T_MOD, T_NIL, T_NOT, 
	T_OF, T_OR, T_PACKED, T_PRODEDURE, T_PROGRAM, T_RECORD, T_REPEAT, T_SET, T_THEN, T_TO,
	T_TYPE, T_UNTIL, T_VAR, T_WHILE, T_WITH,
	T_INT_TYPE, T_REAL_TYPE, 
	T_ADD, T_SUB, T_MUL, T_ADDE, T_SUBE, T_MULE, T_DIVE, T_POW,
	T_EQL, T_NEQ, T_GT, T_GTE, T_LT, T_LTE,
	T_COMMA, T_COLON, T_SEMICL, T_SLASH, T_EXC, T_QUESTION,
	T_SHARP, T_DOT, T_CONT, T_LPAR, T_RPAR, T_LBRKPAR, T_RBRKPAR, T_ASS, T_DOUBLE_DOT,
	T_INT, T_REAL, T_ID, T_STR, T_FINAL
}TokenType;

//T_FINAL and V_start is used for special puopose
typedef enum VarType
{
	V_start = T_FINAL + 1,
	V_program, V_subprogram_declarations, V_identifier_list, V_declarations, V_declaration,
	V_type, V_standard_type, V_subprogram_declaration, V_subprogram_head, V_arguments,
	V_parameter_list, V_optional_statements, V_statement_list, V_statement, V_procedure_statement,
	V_compound_statement, V_variable, V_expression, V_expression_list, V_simple_expression,
	V_term, V_factor, V_sign, V_relop, V_addop,
	V_mulop, V_num,
}VarType;

const int kReadBufferSize = 4096;
const int kTokenMaxLen = 128;
const int kStringMaxLen = 12;
const int kCommentMaxLen = 12;
const int kTokenNameArrLen = 1024 * 8;
const int kErrorMaxNum = 1024;

const int kMaxProductionRightLen = 20;
const int kMaxProductionNum = 100;
const int kCharTypeNum = 256;
const int kVariableTypeNum = 256;
const int kStateTypeNum = 200;

const int kOutOfRangeException = 1;

static char* keyword_list[] = {
	"and", "array", "begin", "case", "const", "div", "do", "downto", "else",
	"end", "file", "for", "function", "goto", "if", "in", "label",
	"mod", "nil", "not", "of", "or", "packed", "prodedure", "program",
	"record", "repeat", "set", "then", "to", "type", "until", "var",
	"while", "with",
	"integer", "real"
};
static char* var_list[] = {
	"+", "-", "*", "+=", "-=", "*=", "/=", "**",
	//T_EQL, T_NEQ, T_GT, T_GTE, T_LT, T_LTE,
	"=", "<>", ">", ">=", "<", "<=",
	//T_COMMA, T_COLON, T_SEMICL, T_SLASH, T_EXC, T_QUESTION,
	",", ":", ";", "/", "!", "?",
	//T_SHARP, T_DOT, T_CONT, T_LPAR, T_RPAR, T_LBRKPAR, T_RBRKPAR, T_ASS, T_DOUBLE_DOT,
	"#", ".", "&", "(", ")", "[", "]", ":=", "..",
	//T_INT, T_REAL, T_ID, T_STR, T_FINAL
	"int_num", "real_num", "id", "string", "$", 

	"start", "program", "subprogram_declarations", "identifier_list", "declarations", "declaration",
	"type", "standard_type", "subprogram_declaration", "subprogram_head", "arguments",
	"parameter_list", "optional_statements", "statement_list", "statement", "procedure_statement",
	"compound_statement", "variable", "expression", "expression_list", "simple_expression",
	"term", "factor", "sign", "relop", "addop",
	"mulop", "num",
};

typedef enum ParserStateType
{
	Acc = 1023, Fail = 254,
}ParserStateType;

typedef struct _SymbolItem
{
	char* name;
	TokenType token_type;
	int* value_addr;
	void* extend_attr;
	_SymbolItem()
	{
		name = NULL;
		value_addr = NULL;
		extend_attr = NULL;
	};
	_SymbolItem(char* item_name, TokenType item_tokenType)
	{
		name = item_name;
		token_type = item_tokenType;
		extend_attr = NULL;
	};
} SymbolItem;

typedef struct _KeywordItem
{
	char* name;
	int type;
	_KeywordItem(char* item_name, int item_type)
	{
		name = item_name;
		type = item_type;
	}
}KeywordItem;

typedef struct _ErrorItem
{
	int line;
	char* description;
	_ErrorItem(int _line, char* _description)
	{
		line = _line;
		description = _description;
	}
} ErrorItem;

typedef struct _TokenItem
{
	TokenType token_type;
	int* symbol_addr;
	_TokenItem(TokenType _token_type)
	{
		token_type = _token_type;
		symbol_addr = NULL;
	}
	_TokenItem(TokenType _token_type, int* _symbol_addr)
	{
		token_type = _token_type;
		symbol_addr = _symbol_addr;
	}
}TokenItem;

bool static IsHex(char ch)
{
	if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
		return true;
	return false;
}
bool static IsOct(char ch)
{
	if (ch >= '0' && ch <= '7')
		return true;
	return false;
}
bool static IsBin(char ch)
{
	if (ch == '0' || ch == '1')
		return true;
	return false;
}

template<typename T>
int GetArrayLen(T& arr)
{
	return sizeof(arr) / sizeof(arr[0]);
}
#endif