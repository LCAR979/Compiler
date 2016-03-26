#ifndef __SCANNER_H__ 
#define __SCANNER_H__

#include <cstdio>
#include "hash_table.h"

class Scanner
{
private:
	FILE* fp_;
	FILE* tokenOutfp_;

	int crt_pos_, forward_pos_;
	int token_name_arr_tail_;
	int const_int_arr_tail;
	int const_real_arr_tail;
	char ch_;
	int line_;
	char buf_[kReadBufferSize];
	char token_buf_[kTokenMaxLen];
	
	bool l_read_allow;;
	bool r_read_allow;
	int read_allow_count;

	void MoveBack();
	char MoveForwardGetChar();
	void MoveCrtToForward();

	void DealToken(TokenType token_type);
	void DealInt(int token_val);
	void DealReal(double token_val);
	void ScanNumber();
	void ScanIdnAndKWord();

public:
	HashTable* token_table_;
	HashTable* keyword_table_;
	char token_name_arr_[kTokenNameArrLen];
	int const_int_arr_[kTokenNameArrLen];
	double const_real_arr_[kTokenNameArrLen];

	void Init();
	void ScanToken();
	void Close();
};

template<typename T>
int GetArrayLen(T& arr)
{
	return sizeof(arr) / sizeof(arr[0]);
}
#endif