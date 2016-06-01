#ifndef __SCANNER_H__ 
#define __SCANNER_H__

#include <cstdio>
#include <vector>
#include "global.h"
#include "hash_table.h"
#include "parser.h"

class Scanner
{
private:
	FILE* fp_;
	FILE* token_out_fp_;

	int crt_pos_, forward_pos_;

	char ch_;
	char buf_[kReadBufferSize];
	char token_buf_[kTokenMaxLen];
	
	bool l_read_allow_;
	bool r_read_allow_;
	int read_allow_count_;

	void MoveBack();
	char MoveForwardGetChar();

	void DealToken(TokenType type);
	void DealInt(int tokenval);
	void DealReal(double tokenval);
	//int* InstallID(char* crt_token_name, TokenType type);

	void ScanNumber();
	void ScanIdnAndKWord();
	void ErrorHandle(char* _msg);

public:
	void Init();
	void ScanToken();
	void Close();
	void PrintTokenList();
};

#endif