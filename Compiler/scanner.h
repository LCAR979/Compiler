#ifndef __SCANNER_H__ 
#define __SCANNER_H__

#include <cstdio>
#include "hash.h"

class Scanner
{
private:
	FILE* fp_;
	FILE* tokenOutfp_;
	int crt_pos_, forward_pos_;
	char ch_;
	int line_;
	char buf_[kBufferSize];
	char token_buf_[kTokenMaxLen];
	int token_name_arr_tail_;	
	void MoveBack();
	char MoveForwardGetChar();
	void DealToken(TokenType token_type);
public:
	HashTable* token_table_;
	HashTable* keyword_table_;
	char token_name_arr_[kTokenNameArrLen];
	int const_int_arr_[kTokenNameArrLen];
	float const_real_arr_[kTokenNameArrLen];
	void Init();
	void ScanToken();
};
template<typename T>
int GetArrayLen(T& arr)
{
	return sizeof(arr) / sizeof(arr[0]);
}
#endif