#ifndef __SCANNER_H__ 
#define __SCANNER_H__

#include <cstdio>
#include <vector>
#include "common.h"
#include "hash_table.h"
#include "parser.h"

class Scanner
{
private:
	FILE* fp_;
	FILE* token_out_fp_;

	int crt_pos_, forward_pos_;
	int token_name_arr_tail_;
	int const_int_arr_tail_;
	int const_real_arr_tail_;

	char ch_;
	int line_;
	char buf_[kReadBufferSize];
	char token_buf_[kTokenMaxLen];
	
	bool l_read_allow_;
	bool r_read_allow_;
	int read_allow_count_;

	void MoveBack();
	char MoveForwardGetChar();

	void DealToken(TokenType token_type);
	void DealInt(int tokenval);
	void DealReal(double tokenval);
	int* InstallID(char* crt_token_name, TokenType token_type);

	void ScanNumber();
	void ScanIdnAndKWord();
	void ErrorHandle(char* _msg);

public:
	HashTable<char*, SymbolItem*> symbol_table_;
	HashTable<char*, int> keyword_table_;
	char token_name_arr_[kTokenNameArrLen];
	int const_int_arr_[kTokenNameArrLen];
	double const_real_arr_[kTokenNameArrLen];

	std::vector<TokenItem*> token_list_;
	std::vector<ErrorItem*> error_list_;

	void Init();
	void ScanToken();
	void Close();
	void PrintTokenList();
	~Scanner();

	friend void Parser::Startup(std::vector<TokenItem*>&);
};


#endif