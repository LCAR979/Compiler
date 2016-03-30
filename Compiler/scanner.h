#ifndef __SCANNER_H__ 
#define __SCANNER_H__

#include <cstdio>
#include "common.h"
#include "hash_table.h"

class Scanner
{
private:
	FILE* fp_;
	FILE* token_out_fp_;
	FILE* error_fp_;

	int crt_pos_, forward_pos_;
	int token_name_arr_tail_;
	int const_int_arr_tail_;
	int const_real_arr_tail_;
	int error_item_arr_tail_;

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
	void ScanNumber();
	void ScanIdnAndKWord();
	void ErrorHandle(char* _msg);

public:
	HashTable<char*, SymbolItem*> token_table_;
	HashTable<char*, int> keyword_table_;
	char token_name_arr_[ktoken_nameArrLen];
	int const_int_arr_[ktoken_nameArrLen];
	double const_real_arr_[ktoken_nameArrLen];
	ErrorItem* error_item_arr_[kErrorMaxNum];

	void Init();
	void ScanToken();
	void Close();
};


#endif