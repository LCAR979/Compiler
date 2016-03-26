#include <ctype.h>
#include <cstring>
#include <memory.h>
#include <cstdio>
#include <cstdlib>
#include "scanner.h"
#include "common.h"
#include "hash.h"



void ErrHandle()
{

}
void GetOptToken()
{

}

char Scanner::MoveForwardGetChar()
{
	size_t readBytes;
	if (forward_pos_ == -1 || forward_pos_ == kBufferSize - 1)
	{
		readBytes = fread(buf_, sizeof(char),kBufferSize / 2, fp_);
		if (readBytes < kBufferSize / 2)
			buf_[readBytes + 1] = 255;
	}		
	else if (forward_pos_ == kBufferSize / 2 - 1)
	{
		readBytes = fread(buf_ + kBufferSize / 2, sizeof(char), kBufferSize / 2, fp_);
		if (readBytes < kBufferSize / 2)
			buf_[kBufferSize/2 + readBytes] = 255;
		forward_pos_ = (++(forward_pos_)) % kBufferSize;
	}
	forward_pos_ = (++(forward_pos_)) % kBufferSize;	
	return buf_[forward_pos_];
		
}
/* Copy buf_[crt_pos_.. forward_pos_]  to token_buf_ */
void Scanner::DealToken(TokenType token_type)
{
	char* crt_token_name = (char*)(token_name_arr_ + token_name_arr_tail_);
	if (forward_pos_ - crt_pos_ > 0)
	{
		memcpy(crt_token_name, (char*)(buf_ + crt_pos_ + 1), forward_pos_ - crt_pos_);
		token_name_arr_tail_ += (forward_pos_ - crt_pos_);
	}		
	else
	{
		memcpy(crt_token_name, (char*)(buf_ + crt_pos_ + 1), kBufferSize -1 - crt_pos_);
		memcpy(crt_token_name + kBufferSize -1 - crt_pos_,
			(char*)(buf_), forward_pos_ + 1);
		token_name_arr_tail_ += (kBufferSize + forward_pos_ - crt_pos_);
	}
	
	token_name_arr_[token_name_arr_tail_ ++] = '\0';
	token_table_->Insert(crt_token_name, token_type);
	fprintf(tokenOutfp_,"(%s, %d)\n",crt_token_name, token_type);
	printf("(%s, %d)\n", crt_token_name, token_type);
	crt_pos_ = forward_pos_;
	
}
void Scanner::MoveBack()
{
	forward_pos_ = (forward_pos_ + kBufferSize - 1) % kBufferSize;
	ch_ = buf_[forward_pos_];
}
void Scanner::Init()
{
	fp_ = fopen("source.txt", "r");
	if (fp_ == NULL)
	{
		fprintf(stderr, "Error opening source file.\n");
		exit(-1);
	}
	tokenOutfp_ = fopen("token.txt", "w");
	if (tokenOutfp_ == NULL)
	{
		fprintf(stderr, "Error opening token file.\n");
		exit(-1);
	}
	crt_pos_ = -1;
	forward_pos_ = -1;
	memset(token_buf_, 0, sizeof(token_buf_));
	memset(buf_, 0, sizeof(buf_));
	memset(token_name_arr_, 0, sizeof(token_name_arr_));
	token_name_arr_tail_ = 0;
	token_table_ = new HashTable;
	keyword_table_ = new HashTable;

	for (int i = 0; i < GetArrayLen(keyword_list); i++)
		keyword_table_->Insert(keyword_list[i], TokenType(i));

}
void Scanner::ScanToken()
{
	while ((ch_ = MoveForwardGetChar()) != -1)
	{
		if (ch_ == '\t' || ch_ == '\n' || ch_ == ' ')
		{
			while (ch_ == '\t' || ch_ == '\n' || ch_ == ' ')
			{
				if (ch_ == '\n')
					line_ ++;
				ch_ = MoveForwardGetChar();
			}
			MoveBack();
			crt_pos_ = forward_pos_;
			continue;
		}
		if (isalpha(ch_)){		
			bool test_kword_flag = true;
			bool is_kword_flag = false;
			char tmp_token_name[kTokenMaxLen];
			int tmp_token_name_tail = 0;
			memset(tmp_token_name, 0, sizeof(tmp_token_name));		
			int keyword_type;
			while (isalpha(ch_) || isdigit(ch_))
			{				
				if (isdigit(ch_))
					test_kword_flag = false;
				if (forward_pos_ - crt_pos_ > 9)
					test_kword_flag = false;
				if (test_kword_flag = true)
				{				
					tmp_token_name[tmp_token_name_tail++] = tolower(buf_[forward_pos_]);
					if ((keyword_type = keyword_table_->Find(tmp_token_name) ) != -1)
					{
						DealToken((TokenType)keyword_type);
						is_kword_flag = true;
						break;
					}
				}	
				ch_ = MoveForwardGetChar();
			}
			if (!is_kword_flag)
			{
				MoveBack();
				DealToken(T_IDN);
			}			
			continue;
		}
		if (isdigit(ch_)){
			int res = 0;
			while (isdigit(ch_))
			{
				ch_ = MoveForwardGetChar();
				res = res * 10 + ch_ - '0';
			}
			if (ch_ == '.')
			{
				double f_res = (float)res;
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
					DealToken(T_REAL);
				}
				else
				{
					ErrHandle();
				}
			}
			else
			{
				MoveBack();
				DealToken(T_INT);
			}		
			continue;
		}
		switch (ch_)
		{
		case ':':
			ch_ = MoveForwardGetChar();
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
			if (ch_ == '=')
				DealToken(T_SUBE);
			else
			{
				MoveBack();
				DealToken(T_SUB);
			}
			break;
		case '=':
			DealToken(T_EQL);
			break;
		case ';':
			DealToken(T_SEMICL);
			break;
		case '/':
			ch_ = MoveForwardGetChar();
			if (ch_ == '/')
			{
				while (ch_ != '\n')
					ch_ = MoveForwardGetChar();
				crt_pos_ = forward_pos_;
			}
			else
			{
				MoveBack();
				DealToken(T_SLASH);
			}		
			break;
		case '(':
			DealToken(T_LPAR);
			break;
		case ')':
			DealToken(T_RPAR);
			break;
		case '.':
			DealToken(T_PEROID);
			break;
		case 34:				//  Double Quotation  " 
			ch_ = MoveForwardGetChar();
			while (ch_ != 34)
			{
				ch_ = MoveForwardGetChar();
			}
			// Todo :length restriction
			DealToken(T_STR);
			// ErrHandle();
			break;
		case 39:				 // Singlequotation '
			ch_ = MoveForwardGetChar();
			while (ch_ != 39)
				ch_ = MoveForwardGetChar();
			// Todo :length restriction
			DealToken(T_STR);
			//ErrHandle();
			break;
		case '{':
			while (ch_ != '}')
			{
				if (abs(forward_pos_ - crt_pos_) > kBufferSize - 2)
				{
					crt_pos_ = forward_pos_;
				}
				ch_ = MoveForwardGetChar();
			}
		default:
			ErrHandle();
		}
	}
	fclose(fp_);
	fclose(tokenOutfp_);
}
