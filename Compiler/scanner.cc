#include <ctype.h>
#include <cstring>
#include <memory.h>
#include <cstdio>
#include <cstdlib>
#include "scanner.h"
#include "common.h"
#include "hash_table.h"

void ErrHandle()
{

}
void GetOptToken()
{

}

void Scanner::MoveCrtToForward()
{
	crt_pos_ = forward_pos_;
}
char Scanner::MoveForwardGetChar()
{
	size_t read_bytes = 0;
	
	if ((forward_pos_ == -1 || forward_pos_ == kReadBufferSize - 1) && l_read_allow )
	{
		read_bytes = fread(buf_, sizeof(char), kReadBufferSize / 2, fp_);
		l_read_allow = false;
		if (read_bytes < kReadBufferSize / 2)
			buf_[read_bytes] = -1;
	}		 
	else if ((forward_pos_ == kReadBufferSize / 2 - 1)&& r_read_allow )
	{
		read_bytes = fread(buf_ + kReadBufferSize / 2, sizeof(char), kReadBufferSize / 2, fp_);
		r_read_allow = false;
		if (read_bytes < kReadBufferSize / 2)
			buf_[kReadBufferSize / 2 + read_bytes] = -1;
	}
	forward_pos_ = (++(forward_pos_)) % kReadBufferSize;
	read_allow_count++;
	if (read_allow_count % 10 == 0)
		l_read_allow = r_read_allow = true;
	if (buf_[forward_pos_] == -1)
		exit(0);
	return buf_[forward_pos_];	
}
/* Copy buf_[crt_pos_.. forward_pos_]  to token_buf_ */
void Scanner::DealInt(int token_val)
{
	const_int_arr_[const_int_arr_tail++] = token_val;
}
void Scanner::DealReal(double token_val)
{
	const_real_arr_[const_real_arr_tail++] = token_val;

}
void Scanner::DealToken(TokenType token_type)
{
	if (token_type == T_END)
	{
		printf("++");
	}
	char* crt_token_name = (char*)(token_name_arr_ + token_name_arr_tail_);
	if (forward_pos_ - crt_pos_ > 0)
	{
		memcpy(crt_token_name, (char*)(buf_ + crt_pos_ + 1), forward_pos_ - crt_pos_);
		token_name_arr_tail_ += (forward_pos_ - crt_pos_);
	}		
	else
	{
		memcpy(crt_token_name, (char*)(buf_ + crt_pos_ + 1), kReadBufferSize -1 - crt_pos_);
		memcpy(crt_token_name + kReadBufferSize -1 - crt_pos_,
			(char*)(buf_), forward_pos_ + 1);
		token_name_arr_tail_ += (kReadBufferSize + forward_pos_ - crt_pos_);
	}
	token_name_arr_[token_name_arr_tail_ ++] = '\0';
	token_table_->Insert(crt_token_name, token_type);
	fprintf(tokenOutfp_,"(%s, %d)\n",crt_token_name, token_type);
	printf("(%s, %d)\n", crt_token_name, token_type);
	MoveCrtToForward();

}

void Scanner::MoveBack()
{
	forward_pos_ = (forward_pos_ + kReadBufferSize - 1) % kReadBufferSize;
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

	token_name_arr_tail_ = const_int_arr_tail = const_real_arr_tail =  0;
	token_table_ = new HashTable;
	keyword_table_ = new HashTable;

	for (int i = 0; i < GetArrayLen(keyword_list); i++)
		keyword_table_->Insert(keyword_list[i], TokenType(i));

	l_read_allow = r_read_allow = true;
	read_allow_count = 0;
}

void Scanner::ScanIdnAndKWord()
{
	char tmp_token_name[kTokenMaxLen];
	int tmp_token_name_tail = 0;
	int keyword_type;
	memset(tmp_token_name, 0, sizeof(tmp_token_name));
	while (isalpha(ch_) || isdigit(ch_) || ch_ == '$' || ch_ == '_')
	{
		tmp_token_name[tmp_token_name_tail++] = tolower(buf_[forward_pos_]);
		ch_ = MoveForwardGetChar();
	}

	MoveBack();
	if ((keyword_type = keyword_table_->Find(tmp_token_name)) != -1)
		DealToken((TokenType)keyword_type);
	else
		DealToken(T_IDN);
	return;
}
void Scanner::ScanNumber()
{
	int res = 0;
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
		else
			ErrHandle();
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
	while (ch_ != EOF)
	{
		ch_ = MoveForwardGetChar();
		if (ch_ == '\t' || ch_ == '\n' || ch_ == ' ')
		{
			while (ch_ == '\t' || ch_ == '\n' || ch_ == ' ')
			{
				if (ch_ == '\n')
					line_ ++;
				ch_ = MoveForwardGetChar();
			}
			MoveBack();
			MoveCrtToForward();
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
				//Todo Length Restriction
				while (ch_ != '\n' && ch_ != EOF)
				{
					ch_ = MoveForwardGetChar();
				}			
				MoveCrtToForward();
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
		case ',':
			DealToken(T_COMMA);
			break;
		case 34:				//  Double Quotation  " 
			ch_ = MoveForwardGetChar();
			while (ch_ != 34 && ch_ != EOF)
			{
				if (abs(forward_pos_ - crt_pos_) > kReadBufferSize - 2)
				{
					MoveCrtToForward();
				}
				ch_ = MoveForwardGetChar();
			}
			// Todo :length restriction
			DealToken(T_STR);
			// ErrHandle();
			break;
		case 39:				 // Singlequotation '
			ch_ = MoveForwardGetChar();
			while (ch_ != 39 && ch_ != EOF)
			{
				if (abs(forward_pos_ - crt_pos_) > kReadBufferSize - 2)
				{
					MoveCrtToForward();
				}
				ch_ = MoveForwardGetChar();
			}
				
			// Todo :length restriction
			DealToken(T_STR);
			//ErrHandle();
			break;
		case '{':
			while (ch_ != '}')
			{
				if (abs(forward_pos_ - crt_pos_) > kReadBufferSize - 2)
				{
					MoveCrtToForward();
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