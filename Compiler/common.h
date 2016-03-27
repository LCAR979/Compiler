#ifndef __TOKEN_H__
#define __TOKEN_H__

typedef enum
{
	// key words
	T_AND = 0, T_ARRAY, T_BEGIN, T_CASE, T_CONST, T_DIV, T_DO, T_DOWNTO, T_ELSE, T_END,
	T_FILE, T_FOR, T_FUNCTION, T_GOTO, T_IF, T_IN, T_LABEL, T_MOD, T_NIL, T_NOT, 
	T_OF, T_OR, T_PACKED, T_PRODEDURE, T_PROGRAM, T_RECORD, T_REPEAT, T_SET, T_THEN, T_TO,
	T_TYPE, T_UNTIL, T_VAR, T_WHILE, T_WITH,
	// operators
	T_ADD, T_SUB, T_MUL, T_ADDE, T_SUBE, T_MULE, T_DIVE, T_POW,
	T_EQL, T_NEQ, T_GT, T_GTE, T_LT, T_LTE,
	T_COMMA, T_COLON, T_SEMICL, T_SLASH, T_EXC, T_QUES,
	T_SHRP, T_PERIOD, T_CONT, T_LPAR, T_RPAR, T_LBRKPAR, T_RBRKPAR, T_SINQUOTA,
	T_DOUQUOTA, T_ASS, T_DOUBLE_PEROID,
	//other types
	T_IDN, T_INT, T_REAL, T_STR
}TokenType;

const int kReadBufferSize = 4096;
const int kTokenMaxLen = 128;
const int kStringMaxLen = 12;
const int kTokenNameArrLen = 1024 * 8;
const int kHashPJWPrime = 211;
const int kHashTableSize = kHashPJWPrime;
const int kStackMaxSize = 1024;
const int kErrorMaxNum = 1024;

static char* keyword_list[] = {
	"and", "array", "begin", "case", "const", "div", "do", "downto", "else",
	"end", "file", "for", "function", "goto", "if", "in", "label",
	"mod", "nil", "not", "of", "or", "packed", "prodedure", "program",
	"record", "repeat", "set", "then", "to", "type", "until", "var",
	"while", "with"
};

typedef struct _SymbolItem
{
	char* name;
	TokenType token_type;
	int* value_addr;
	void* extend_attr;
	struct _SymbolItem* next_hash_item;
	_SymbolItem()
	{
		name = NULL;
		value_addr = NULL;
		extend_attr = NULL;
		next_hash_item = NULL;
	};
	_SymbolItem(char* _name, TokenType _tokenType)
	{
		name = _name;
		token_type = _tokenType;
		extend_attr = NULL;
		next_hash_item = NULL;
	};
} SymbolItem;

typedef struct _KeywordItem
{
	char* name;
	int type;
	_KeywordItem* next_hash_item;
	_KeywordItem(char* _name, int _type)
	{
		name = _name;
		type = _type;
		next_hash_item = NULL;
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

bool static IsHex(char _ch)
{
	if ((_ch >= '0' && _ch <= '9') || (_ch >= 'a' && _ch <= 'f') || (_ch >= 'A' && _ch <= 'F'))
		return true;
	return false;
}
bool static IsOct(char _ch)
{
	if (_ch >= '0' && _ch <= '7')
		return true;
	return false;
}
bool static IsBin(char _ch)
{
	if (_ch == '0' || _ch == '1')
		return true;
	return false;
}

#endif