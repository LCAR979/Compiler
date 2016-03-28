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


void Scanner::ErrorHandle(char* _err_msg)
{
	ErrorItem* error_item = new ErrorItem(line_, _err_msg);
	error_item_arr_[error_item_arr_tail++] = error_item;
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
	read_allow_count = (read_allow_count + 1) % 11;
	if (read_allow_count % 10 == 0)
		l_read_allow = r_read_allow = true;
	if (buf_[forward_pos_] == -1)
	{
		Close();
		exit(0);
	}
		
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
	SymbolItem* symbol_item = new SymbolItem;
	symbol_item->name = crt_token_name;
	symbol_item->token_type = token_type;
	symbol_item->next_hash_item = NULL;

	token_table_.Insert(symbol_item->name, symbol_item);
	fprintf(token_out_fp_, "(%d, %s)\n", token_type, crt_token_name);
	crt_pos_ = forward_pos_;

}

void Scanner::MoveBack()
{
	//if (buf_[forward_pos_ - 1] == '\n')
	//	line_--;
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
	token_out_fp_ = fopen("token.txt", "w");
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

	token_name_arr_tail_ = const_int_arr_tail = const_real_arr_tail =  0;
	error_item_arr_tail = 0;

	for (int i = 0; i < GetArrayLen(keyword_list); i++)
	{
		KeywordItem* keyword_item = new KeywordItem(keyword_list[i], TokenType(i));
		keyword_table_.Insert(keyword_item->name, keyword_item);
	}
	for (int i = 0; i < kErrorMaxNum; i++)
	{
		error_item_arr_[i] = NULL;
	}

	l_read_allow = r_read_allow = true;
	read_allow_count = 0;
	line_ = 1;

	
}
void Scanner::Close()
{
	int error_count = 0;
	for (int i = 0; error_item_arr_[i] != NULL; i++)
	{
		printf("Line %d, %s\n", error_item_arr_[i]->line, error_item_arr_[i]->description);
		error_count++;
	}
	if (error_count != 0)
		printf(" Error count %d\n", error_count);
	else
	{
		printf("No token error\n");
	}	
	fclose(fp_);
	fclose(token_out_fp_);
}
void Scanner::ScanIdnAndKWord()
{
	char tmp_token_name[kTokenMaxLen];
	int tmp_token_name_tail = 0;
	KeywordItem* keyword_item;
	memset(tmp_token_name, 0, sizeof(tmp_token_name));
	while (isalpha(ch_) || isdigit(ch_) || ch_ == '$' || ch_ == '_')
	{
		tmp_token_name[tmp_token_name_tail++] = tolower(buf_[forward_pos_]);
		ch_ = MoveForwardGetChar();
	}

	MoveBack();
	keyword_item = keyword_table_.Find(tmp_token_name);
	if (keyword_item != NULL)
		DealToken((TokenType)keyword_item->type);
	else
		DealToken(T_IDN);
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
				DealToken(T_DOUBLE_PEROID);
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
			DealToken(T_DOUBLE_PEROID);
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
	char l_char;
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
		case '(':
			DealToken(T_LPAR);
			break;
		case ')':
			DealToken(T_RPAR);
			break;
		case '.':
			DealToken(T_PERIOD);
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
			if (ch_ == '/')
			{
				while (ch_ != '\n')
				{
					if (ch_ == '\n')
						line_++;
					ch_ = MoveForwardGetChar();
				}			
				crt_pos_ = forward_pos_;
			}
			else
			{
				MoveBack();
				DealToken(T_SLASH);
			}		
			break;	
		case '\'':				
		case '\"':
			l_char = ch_;
			ch_ = MoveForwardGetChar();
			while (ch_ != l_char)
			{
				if ((forward_pos_ - crt_pos_ + kReadBufferSize) % kReadBufferSize > kStringMaxLen)
				{
					crt_pos_ = forward_pos_;
					ErrorHandle("String too long");
					break;
				}
				ch_ = MoveForwardGetChar();
			}
			if (ch_ == l_char)
				DealToken(T_STR);
			break;
		case '{':
			ch_ = MoveForwardGetChar();
			while (ch_ != '}')
			{
				if (abs(forward_pos_ - crt_pos_) >  kCommentMaxLen)
				{
					crt_pos_ = forward_pos_;
					ErrorHandle("Comment too long");
					break;
				}
				ch_ = MoveForwardGetChar();
			}
			if (ch_ == '}')
				DealToken(T_STR);
			break;
		default:
			ErrorHandle("Unexpected character, not in character set");
		}
	}
	Close();
}
