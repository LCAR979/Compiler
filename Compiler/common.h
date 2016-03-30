#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <stdlib.h>

typedef enum _TokenType
{
	// key words
	T_AND = 0, T_ARRAY, T_BEGIN, T_CASE, T_CONST, T_DIV, T_DO, T_DOWNTO, T_ELSE, T_END,
	T_FILE, T_FOR, T_FUNCTION, T_GOTO, T_IF, T_IN, T_LABEL, T_MOD, T_NIL, T_NOT, 
	T_OF, T_OR, T_PACKED, T_PRODEDURE, T_PROGRAM, T_RECORD, T_REPEAT, T_SET, T_THEN, T_TO,
	T_TYPE, T_UNTIL, T_VAR, T_WHILE, T_WITH,
	// operators
	T_ADD, T_SUB, T_MUL, T_ADDE, T_SUBE, T_MULE, T_DIVE, T_POW,
	T_EQL, T_NEQ, T_GT, T_GTE, T_LT, T_LTE,
	T_COMMA, T_COLON, T_SEMICL, T_SLASH, T_EXC, T_QUESTION,
	T_SHARP, T_PERIOD, T_CONT, T_LPAR, T_RPAR, T_LBRKPAR, T_RBRKPAR, T_ASS, T_DOUBLE_PEROID,
	//other types
	T_IDN, T_INT, T_REAL, T_STR
}TokenType;




const int kReadBufferSize = 4096;
const int kTokenMaxLen = 128;
const int kStringMaxLen = 12;
const int kCommentMaxLen = 12;
const int ktoken_nameArrLen = 1024 * 8;
const int kErrorMaxNum = 1024;

const int kMaxProductionRightLen = 20;
const int kMaxProductionNum = 100;
const int kCharTypeNum = 256;
const int kVariableTypeNum = 256;
const int kStateTypeNum = 7;

const int kOutOfRangeException = 1;

static char* keyword_list[] = {
	"and", "array", "begin", "case", "const", "div", "do", "downto", "else",
	"end", "file", "for", "function", "goto", "if", "in", "label",
	"mod", "nil", "not", "of", "or", "packed", "prodedure", "program",
	"record", "repeat", "set", "then", "to", "type", "until", "var",
	"while", "with"
};
static char char_set[] = {
	'a', 'b', '#'
};
static char variable_set[] = {
	'S', 'B'
};

typedef enum _ParserStateType
{
	Acc = 254, Fail = 255, Parsing= 253
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

typedef struct keywordItem
{
	char* name;
	int type;
	keywordItem(char* item_name, int item_type)
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