#include <ctype.h>
#include <cstring>
#include <memory.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "scanner.h"
#include "global.h"

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
void Scanner::DealInt(int int_val)
{
	token_vec.push_back(TokenItem(T_INT, int_val));
}

void Scanner::DealReal(double real_val)
{
	token_vec.push_back(TokenItem(T_REAL, real_val));
}

void Scanner::DealToken(TokenType type)
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
	if (type == T_ID)
	{
		int len = strlen(crt_token_name);
		memcpy(token_name_arr + token_name_arr_tail, crt_token_name, len);

		TokenItem tmp_token_item(type, token_name_arr + token_name_arr_tail);
		token_vec.push_back(tmp_token_item);
		
		token_name_arr_tail += strlen(crt_token_name);
		token_name_arr[token_name_arr_tail++] = '\0';
	}
	else
	{
		token_vec.push_back(TokenItem(type));
	}
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

	crt_pos_ = -1;
	forward_pos_ = -1;
	l_read_allow_ = r_read_allow_ = true;
	read_allow_count_ = 0;
	line_number = 1;

	memset(token_buf_, 0, sizeof(token_buf_));
	memset(buf_, 0, sizeof(buf_));

	memset(token_name_arr, 0, sizeof(token_name_arr));

	for (int i = 0; i < T_ADD; i++)
		keyword_table[std::string(word_list[i])] = TokenType(i);
}

void Scanner::Close()
{
	printf("Scanning finished\n");
	fclose(fp_);
	PrintTokenList();
}

void Scanner::ScanIdnAndKWord()
{
	char tmp_token_name[kTokenMaxLen];
	int tmp_token_name_tail = 0;
	memset(tmp_token_name, 0, sizeof(tmp_token_name));
	while (isalpha(ch_) || isdigit(ch_) || ch_ == '$' || ch_ == '_')
	{
		tmp_token_name[tmp_token_name_tail++] = tolower(buf_[forward_pos_]);
		ch_ = MoveForwardGetChar();
	}
	MoveBack();

	if (keyword_table.end() != keyword_table.find(std::string(tmp_token_name)))
		DealToken((TokenType)keyword_table[tmp_token_name]);
	else
		DealToken(T_ID);
	return;
}

int HexToInt(char hexChar)
{
	if (hexChar >= '0' && hexChar <= 9)
		return hexChar - '0';
	else if (hexChar >= 'A' && hexChar <= 'F')
		return hexChar - 'A' + 10;
	else if (hexChar >= 'a' && hexChar <= 'f')
		return hexChar - 'a' + 10;
	else
	{
		printf("Passing non-hex character into HexToInt()\n");
		return -1;
	}		
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
				res = (res << 4) + HexToInt(ch_);
				ch_ = MoveForwardGetChar();
			}
			MoveBack();
			DealInt(res); 
			return;
		}
		else if (IsOct(ch_))
		{
			while (IsOct(ch_))
			{
				res = (res << 3) + (ch_ - '0');	//Todo: check Oct error
				ch_ = MoveForwardGetChar();
			}
			MoveBack(); 
			DealInt(res); 
			return;
		}
		else if (ch_ == '.')
		{
			ch_ = MoveForwardGetChar();
			if (ch_ == '.')
			{
				MoveBack();
				MoveBack();
				DealInt(0);
				MoveForwardGetChar();
				MoveForwardGetChar();
				DealToken(T_DOUBLE_DOT);
				return;
			}
		}
		else
		{
			DealInt(0);
			return;
		}
	}
	while (isdigit(ch_))
	{
		res = res * 10 + ch_ - '0';
		ch_ = MoveForwardGetChar();		
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
				f_res = f_res + 1.0 / digit_count * (ch_ - '0');
				digit_count *= 10;
				ch_ = MoveForwardGetChar();
			}
			MoveBack();
			DealReal(f_res);
		}
		else if (ch_ == '.')
		{
			MoveBack();
			MoveBack();
			DealInt(res);
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
					line_number ++;
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
						line_number++;
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
	FILE* log_fp = fopen("output\\token.txt", "w");
	std::vector<TokenItem>::iterator it;
	for (it = token_vec.begin(); it != token_vec.end(); it++)
	{
		if (it->type == T_INT)
			fprintf(log_fp, "(%d, %d)\n", it->type, (it->val).int_val);
		else if (it->type == T_REAL)
			fprintf(log_fp, "(%d, %f)\n", it->type, (it->val).real_val);
		else if (it->type == T_ID)
			fprintf(log_fp, "(%d, %s)\n", it->type, (it->val).name_addr);
		else
			fprintf(log_fp, "(%d, -)\n", it->type);
	}
		
	fclose(log_fp);
}
