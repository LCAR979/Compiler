#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdlib.h>
#include <vector>
#include "hash_table.h"
#include "general_stack.h"

const int kReadBufferSize = 4096;
const int kTokenMaxLen = 128;
const int kStringMaxLen = 12;
const int kCommentMaxLen = 12;
const int kTokenNameArrLen = 1024 * 8;
const int kErrorMaxNum = 1024;

const int kMaxProductionRightLen = 20;
const int kMaxProductionNum = 100;


const int kStateTypeNum = 200;

const int kOutOfRangeException = 1;

const int kMaxStateNum = 256;
const int kMaxVarNum = 300;

const int MAX_INTERCODE_LINE = 1024;
const int INTER_CODE_OP_LEN = 12;

typedef enum TokenType
{
	T_AND, T_ARRAY, T_BEGIN, T_CASE, T_CONST, T_DIV, T_DO, T_DOWNTO, T_ELSE, T_END,
	T_FILE, T_FOR, T_FUNCTION, T_GOTO, T_IF, T_IN, T_LABEL, T_MOD, T_NIL, T_NOT, 
	T_OF, T_OR, T_PACKED, T_PROCEDURE, T_PROGRAM, T_RECORD, T_REPEAT, T_SET, T_THEN, T_TO,
	T_TYPE, T_UNTIL, T_VAR, T_WHILE, T_WITH,
	T_INT_TYPE, T_REAL_TYPE, 

	T_ADD, T_SUB, T_MUL, T_ADDE, T_SUBE, T_MULE, T_DIVE, T_POW,
	T_EQL, T_NEQ, T_GT, T_GTE, T_LT, T_LTE,
	T_COMMA, T_COLON, T_SEMICL, T_SLASH, T_EXC, T_QUESTION,
	T_SHARP, T_DOT, T_CONT, T_LPAR, T_RPAR, T_LBRKPAR, T_RBRKPAR, T_ASS, T_DOUBLE_DOT,
	T_INT, T_REAL, 

	T_ID, T_STR, 
	
	T_FINAL,			//Do not add this to compiler_bench

	V_start = T_FINAL + 1,
	V_program, V_subprogram_declarations, V_identifier_list, V_declarations, V_declaration,
	V_type, V_standard_type, V_subprogram_declaration, V_subprogram_head, V_arguments,
	V_parameter_list, V_optional_statements, V_statement_list, V_statement, V_procedure_statement,
	V_compound_statement, V_variable, V_expression, V_expression_list, V_simple_expression,
	V_term, V_factor, V_sign, V_relop, V_addop,
	V_mulop, V_num,
}TokenType;

static char* keyword_list[] = {
	"and", "array", "begin", "case", "const", "div", "do", "downto", "else",
	"end", "file", "for", "function", "goto", "if", "in", "label",
	"mod", "nil", "not", "of", "or", "packed", "procedure", "program",
	"record", "repeat", "set", "then", "to", "type", "until", "var",
	"while", "with",
	"integer", "real",
	"true", "false"
};

static char* var_list[] = {
	"+", "-", "*", "+=", "-=", "*=", "/=", "**",		//8, 0
	//T_EQL, T_NEQ, T_GT, T_GTE, T_LT, T_LTE,
	"=", "<>", ">", ">=", "<", "<=",					//6, 8
	//T_COMMA, T_COLON, T_SEMICL, T_SLASH, T_EXC, T_QUESTION,
	",", ":", ";", "/", "!", "?",						//6, 14
	//T_SHARP, T_DOT, T_CONT, T_LPAR, T_RPAR, T_LBRKPAR, T_RBRKPAR, T_ASS, T_DOUBLE_DOT,
	"#", ".", "&", "(", ")", "[", "]", ":=", "..",		//9, 20
	//T_INT, T_REAL, T_ID, T_STR, T_FINAL
	"int_num", "real_num", 
	"true", "false"
	
	"id", "string", "$",			//5, 29

	"start",											//1, 34
	"program", "subprogram_declarations", "identifier_list", "declarations", "declaration", //5, 35
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
	TokenType type;
	typedef union _TokenValue
	{
		char* name_addr;
		int int_val;
		double real_val;
	}TokenVlaue;

	TokenVlaue val;

	_TokenItem(){}
	_TokenItem(TokenType _type)
	{
		type = _type;
		val.name_addr = NULL;
	}
	_TokenItem(TokenType _type, char* _name_addr)
	{
		type = _type;
		val.name_addr = _name_addr;
	}
	_TokenItem(TokenType _type, int _int_val)
	{
		type = _type;
		val.int_val = _int_val;
	}
	_TokenItem(TokenType _type, double _real_val)
	{
		type = _type;
		val.real_val = _real_val;
	}
}TokenItem;

typedef struct _BackpatchListItem
{
	int quad;
	struct _BackpatchListItem* next;
}BackpatchListItem;

typedef struct _ItemAttribute
{
	int addr;
	int width;
	int offset;

	TokenType type;
	char* name_addr;
	int int_val;
	double real_val;
	
	int quad;
	int again;
	BackpatchListItem* truelist;
	BackpatchListItem* falselist;
	BackpatchListItem* nextlist;
	struct _ItemAttribute(){}
}ItemAttribute;

typedef struct _Item
{
	int state;
	ItemAttribute attr;
	struct _Item(){}
}Item;

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

void ErrorHandle(char* _msg);

void ErrorPrint();

char* GetLiteral(int index);

char* IntToStr(int i);

extern int line_number;
extern bool fatel_error;
extern int tmp_count;

extern HashTable<char*, int> keyword_table;

extern char token_name_arr[kTokenNameArrLen];
extern int token_name_arr_tail;

extern std::vector<int> const_int_vec;
extern std::vector<double> const_real_vec;

extern std::vector<TokenItem> token_vec;
extern std::vector<ErrorItem> error_vec;
extern GeneralStack<Item> st;

#endif