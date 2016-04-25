#include <ctype.h>
#include <cstring>
#include <memory.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "scanner.h"
#include "common.h"
#include "hash_table.h"

void Scanner::ErrorHandle(char* err_msg)
{
	error_list_.push_back(new ErrorItem(line_, err_msg));
}

char Scanner::MoveForwardGetChar()
{
	size_t read_bytes = 0;
	
	if ((forward_pos_ == -1 || forward_pos_ == kReadBufferSize - 1) && l_read_allow_ )
	{
		read_bytes = fread(buf_, sizeof(char), kReadBufferSize / 2, fp_);
		l_read_allow_ = false;
		if (read_bytes < kReadBufferSize / 2)
			buf_[read_bytes] = -1;
	}		 
	else if ((forward_pos_ == kReadBufferSize / 2 - 1)&& r_read_allow_ )
	{
		read_bytes = fread(buf_ + kReadBufferSize / 2, sizeof(char), kReadBufferSize / 2, fp_);
		r_read_allow_ = false;
		if (read_bytes < kReadBufferSize / 2)
			buf_[kReadBufferSize / 2 + read_bytes] = -1;
	}
	forward_pos_ = (++(forward_pos_)) % kReadBufferSize;
	read_allow_count_ = (read_allow_count_ + 1) % 11;
	if (read_allow_count_ % 10 == 0)
		l_read_allow_ = r_read_allow_ = true;
	if (buf_[forward_pos_] == -1)
		return EOF;		
	return buf_[forward_pos_];	
}

/* Copy buf_[crt_pos_.. forward_pos_]  to token_buf_ */
void Scanner::DealInt(int tokenval)
{
	const_int_arr_[const_int_arr_tail_++] = tokenval;
}
void Scanner::DealReal(double tokenval)
{
	const_real_arr_[const_real_arr_tail_++] = tokenval;

}
int* Scanner::InstallID(char* crt_token_name, TokenType token_type)
{
	token_name_arr_[token_name_arr_tail_++] = '\0';
	SymbolItem* symbol_item = new SymbolItem(crt_token_name, token_type);
	symbol_table_.Insert(symbol_item->name, symbol_item);
	return (int*)symbol_table_.FindAddr(crt_token_name);
}
void Scanner::DealToken(TokenType token_type)
{
	char crt_token_name[kTokenMaxLen];
	memset(crt_token_name, 0, sizeof(crt_token_name));
	if (forward_pos_ - crt_pos_ > 0)
	{
		memcpy(crt_token_name, (char*)(buf_ + crt_pos_ + 1), forward_pos_ - crt_pos_);
	}
	else
	{
		memcpy(crt_token_name, (char*)(buf_ + crt_pos_ + 1), kReadBufferSize - 1 - crt_pos_);
		memcpy(crt_token_name + kReadBufferSize - 1 - crt_pos_,
			(char*)(buf_), forward_pos_ + 1);
	}
	if (token_type == T_ID)
	{
		memcpy(token_name_arr_ + token_name_arr_tail_, crt_token_name, strlen(crt_token_name));
		int* addr = InstallID(crt_token_name, token_type);
		token_list_.push_back(new TokenItem(token_type, addr));
	}
	else
	{
		token_list_.push_back(new TokenItem(token_type));
	}
	fprintf(token_out_fp_, "(%d, %s)\n", token_type, crt_token_name);
	crt_pos_ = forward_pos_;
}

void Scanner::MoveBack()
{
	forward_pos_ = (forward_pos_ + kReadBufferSize - 1) % kReadBufferSize;
	ch_ = buf_[forward_pos_];
}
void Scanner::Init()
{
	fp_ = fopen("res\\source.txt", "r");
	if (fp_ == NULL)
	{
		fprintf(stderr, "Error opening source file.\n");
		exit(-1);
	}
	token_out_fp_ = fopen("output\\token.txt", "w");
	if (token_out_fp_ == NULL)
	{
		fprintf(stderr, "Error opening token file.\n");
		exit(-1);
	}

	crt_pos_ = -1;
	forward_pos_ = -1;
	memset(token_buf_, 0, sizeof(token_buf_));
	memset(buf_, 0, sizeof(buf_));
	memset(token_name_arr_, 0, sizeof(token_name_arr_));

	token_name_arr_tail_ = const_int_arr_tail_ = const_real_arr_tail_ =  0;

	for (int i = 0; i < GetArrayLen(keyword_list); i++)
		keyword_table_.Insert(keyword_list[i], TokenType(i));
	l_read_allow_ = r_read_allow_ = true;
	read_allow_count_ = 0;
	line_ = 1;
}
void Scanner::Close()
{
	int error_count = 0;
	FILE* error_fp = fopen("output\\error.txt", "w");
	std::vector<ErrorItem*>::iterator it;
	for (it = error_list_.begin(); it != error_list_.end();it++)
	{
		printf("Line %d, %s\n", (*it)->line, (*it)->description);
		fprintf(error_fp, "Line %d, %s\n", (*it)->line, (*it)->description);
		error_count++;
	}
	printf("Scanning finished\n");
	if (error_count != 0)
		printf(" Error count %d\n", error_count);
	else
	{
		printf("No token error\n");
	}	
	fclose(fp_);
	fclose(token_out_fp_);
	fclose(error_fp);
	//PrintTokenList();
}
void Scanner::ScanIdnAndKWord()
{
	char tmp_token_name[kTokenMaxLen];
	int tmp_token_name_tail = 0;
	int token_type;
	memset(tmp_token_name, 0, sizeof(tmp_token_name));
	while (isalpha(ch_) || isdigit(ch_) || ch_ == '$' || ch_ == '_')
	{
		tmp_token_name[tmp_token_name_tail++] = tolower(buf_[forward_pos_]);
		ch_ = MoveForwardGetChar();
	}
	MoveBack();
	if ((token_type = keyword_table_.Find(tmp_token_name)) != NULL)
		DealToken((TokenType)token_type);
	else
		DealToken(T_ID);
	return;
}
void Scanner::ScanNumber()
{
	int res = 0;
	if (ch_ == '0')
	{
		ch_ = MoveForwardGetChar();
		if (ch_ == 'x' || ch_ == 'X')
		{
			ch_ = MoveForwardGetChar();
			while (IsHex(ch_))
			{
				ch_ = MoveForwardGetChar();
			}
			MoveBack();
			DealToken(T_INT);
			return;
		}
		else if (IsOct(ch_))
		{
			while (IsOct(ch_))
			{
				ch_ = MoveForwardGetChar();
			}
			MoveBack();
			DealToken(T_INT);
			return;
		}
		else if (ch_ == '.')
		{
			ch_ = MoveForwardGetChar();
			if (ch_ == '.')
			{
				MoveBack();
				MoveBack();
				DealToken(T_INT);
				MoveForwardGetChar();
				MoveForwardGetChar();
				DealToken(T_DOUBLE_DOT);
				return;
			}
		}
		else
		{
			DealToken(T_INT);
			return;
		}
	}
	while (isdigit(ch_))
	{
		ch_ = MoveForwardGetChar();
		res = res * 10 + ch_ - '0';
	}
	if (ch_ == '.')
	{
		double f_res = (double)res;
		double digit_count = 10;
		ch_ = MoveForwardGetChar();
		if (isdigit(ch_))
		{
			while (isdigit(ch_))
			{
				ch_ = MoveForwardGetChar();
				f_res = f_res + 1.0 / digit_count * (ch_ - '0');
				digit_count++;
				ch_ = MoveForwardGetChar();
			}
			MoveBack();
			DealReal(f_res);
			DealToken(T_REAL);
		}
		else if (ch_ == '.')
		{
			MoveBack();
			MoveBack();
			DealToken(T_INT);
			MoveForwardGetChar();
			MoveForwardGetChar();
			DealToken(T_DOUBLE_DOT);
			return;
		}
		else
		{
			ErrorHandle("Unexpected character, expecting digit");
			MoveBack();
			return;
		}
			
	}
	else
	{
		MoveBack();
		DealInt(res);
		DealToken(T_INT);
	}
	return;
}
void Scanner::ScanToken()
{
	char lchar;
	while (ch_ != EOF)
	{
		ch_ = MoveForwardGetChar();
		if (ch_ == EOF)
			break;
		if (ch_ == '\t' || ch_ == '\n' || ch_ == ' ')
		{
			while (ch_ == '\t' || ch_ == '\n' || ch_ == ' ')
			{
				if (ch_ == '\n')
					line_ ++;
				ch_ = MoveForwardGetChar();
				if (ch_ == EOF)
					break;
			}
			MoveBack();
			crt_pos_ = forward_pos_;
			continue;
		}
		if (isalpha(ch_) || ch_ == '$' || ch_ == '_')
		{
			ScanIdnAndKWord();
			continue;
		}		
		if (isdigit(ch_))
		{
			ScanNumber();
			continue;
		}		
		switch (ch_)
		{
		case -1:
			break;
		case ':':
			ch_ = MoveForwardGetChar();
			if (ch_ == EOF)
				break;
			if (ch_ == '=')
				DealToken(T_ASS);
			else
			{
				MoveBack();
				DealToken(T_COLON);
			}
			break;
		case '*':
			ch_ = MoveForwardGetChar();
			if (ch_ == EOF)
				break;
			if (ch_ == '*')
				DealToken(T_POW);
			else if (ch_ == '=')
				DealToken(T_MULE);
			else
			{
				MoveBack();
				DealToken(T_MUL);
			}
			break;
		case '+':
			ch_ = MoveForwardGetChar();
			if (ch_ == EOF)
				break;
			if (ch_ == '=')
				DealToken(T_ADDE);
			else
			{
				MoveBack();
				DealToken(T_ADD);
			}
			break;
		case '-':
			ch_ = MoveForwardGetChar();
			if (ch_ == EOF)
				break;
			if (ch_ == '=')
				DealToken(T_SUBE);
			else
			{
				MoveBack();
				DealToken(T_SUB);
			}
			break;
		case '(':
			DealToken(T_LPAR);
			break;
		case ')':
			DealToken(T_RPAR);
			break;
		case '.':
			DealToken(T_DOT);
			break;
		case ',':
			DealToken(T_COMMA);
			break;
		case '=':
			DealToken(T_EQL);
			break;
		case ';':
			DealToken(T_SEMICL);
			break;
		case '[':
			DealToken(T_LBRKPAR);
			break;
		case ']':
			DealToken(T_RBRKPAR);
			break;
		case '<':
			ch_ = MoveForwardGetChar();
			if (ch_ == EOF)
				break;
			if (ch_ == '=')
				DealToken(T_LTE);
			else if (ch_ == '>')
				DealToken(T_NEQ);
			else
			{
				MoveBack();
				DealToken(T_LT);
			}
			break;
		case '>':
			ch_ = MoveForwardGetChar();
			if (ch_ == EOF)
				break;
			if (ch_ == '=')
				DealToken(T_GTE);
			else
			{
				MoveBack();
				DealToken(T_GT);
			}
			break;
		case '/':
			ch_ = MoveForwardGetChar();
			if (ch_ == EOF)
				break;
			if (ch_ == '/')
			{
				while (ch_ != '\n')
				{
					if (ch_ == '\n')
						line_++;
					ch_ = MoveForwardGetChar();
					if (ch_ == EOF)
						break;
				}			
				crt_pos_ = forward_pos_;
			}
			else
			{
				MoveBack();
				DealToken(T_DIV);
			}		
			break;	
		case '\'':				
		case '\"':
			lchar = ch_;
			ch_ = MoveForwardGetChar();
			if (ch_ == EOF)
				break;
			while (ch_ != lchar)
			{
				if ((forward_pos_ - crt_pos_ + kReadBufferSize) % kReadBufferSize > kStringMaxLen)
				{
					crt_pos_ = forward_pos_;
					ErrorHandle("String too long");
					break;
				}
				ch_ = MoveForwardGetChar();
				if (ch_ == EOF)
					break;
			}
			if (ch_ == lchar)
				DealToken(T_STR);
			break;
		case '{':
			ch_ = MoveForwardGetChar();
			if (ch_ == EOF)
				break;
			while (ch_ != '}')
			{
				if (abs(forward_pos_ - crt_pos_) >  kCommentMaxLen)
				{
					crt_pos_ = forward_pos_;
					ErrorHandle("Comment too long");
					break;
				}
				ch_ = MoveForwardGetChar();
				if (ch_ == EOF)
					break;
			}
			if (ch_ == '}')
				crt_pos_ = forward_pos_;
			break;
		default:
			ErrorHandle("Unexpected character, not in character set");
		}
	}
	Close();
}
void Scanner::PrintTokenList()
{
	FILE* log_fp = fopen("output\\log.txt", "w");
	std::vector<TokenItem*>::iterator it;
	for (it = token_list_.begin(); it != token_list_.end(); it++)
		fprintf(log_fp, "%d, %p\n", (*it)->token_type, (*it)->symbol_addr);
	fclose(log_fp);
}

Scanner::~Scanner()
{
	std::vector<ErrorItem*>::iterator it;
	for (it = error_list_.begin(); it != error_list_.end();it++)
	{
		delete *it;
		*it = NULL;
	}
}