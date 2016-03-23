#include <sys/types.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "token.h"

int crt, forward;
int *crtPos = &crt;
int *forwardPos = &forward;
char ch;

unsigned int BKDRHash(char* str)
{
	unsigned int seed = 13131;
	unsigned int hash = 0;
	while (*str)
	{
		hash = hash * seed + (*str++);
	}
	return (hash & 0x7fffffff);
}
void ErrHandle()
{

}
void GetOptToken()
{

}
void DealIdn(){ }


char MoveForwardGetChar()
{
	size_t readBytes;
	//fgetpos(fp, &tmpFpPos);
	
	if (*forwardPos == -1 || *forwardPos == BUFFER_SIZE - 1)
	{
		readBytes = fread(buf, sizeof(char),BUFFER_SIZE / 2, fp);
		if (readBytes < BUFFER_SIZE / 2)
			buf[readBytes + 1] = 255;
		//fsetpos(fp, &tmpFpPos);
		*forwardPos = (++(*forwardPos)) % BUFFER_SIZE;
	}		
	else if (*forwardPos == BUFFER_SIZE / 2 - 1)
	{
		readBytes = fread(buf + BUFFER_SIZE / 2, sizeof(char), BUFFER_SIZE / 2, fp);
		if (readBytes < BUFFER_SIZE / 2)
			buf[BUFFER_SIZE/2 + readBytes] = 255;
		//fsetpos(fp, &tmpFpPos);
		*forwardPos = (++(*forwardPos)) % BUFFER_SIZE;
	}
	else
	{
		*forwardPos = (++(*forwardPos)) % BUFFER_SIZE;	
	}
	return buf[*forwardPos];
		
}
/* Copy buf[crtPos.. forwardPos]  to tokenBuf */
void DealToken(char *tokenName, int attrVal)
{
	memset(tokenBuf, 0, sizeof(tokenBuf));
	memcpy(tokenBuf, (char*)(buf+*crtPos+1), *forwardPos - *crtPos);
	fprintf(tokenOutFp,"(%s, %d)\n",tokenBuf, attrVal);
	printf("(%s, %d)\n", tokenBuf, attrVal);
	*crtPos = *forwardPos;
}
void Moveback()
{
	*forwardPos = (*forwardPos + BUFFER_SIZE - 1) % BUFFER_SIZE;
	ch = buf[*forwardPos];
}
void Scan()
{
	fp = fopen("source.txt", "r");
	if (fp == NULL)
	{
		fprintf(stderr, "Error opening source file.\n");
		exit(-1);
	}
	tokenOutFp = fopen("token.txt", "w");
	if (tokenOutFp == NULL)
	{
		fprintf(stderr, "Error opening token file.\n");
		exit(-1);
	}
	int lineCount = 1;
	*crtPos = -1;
	*forwardPos = -1;
	memset(tokenBuf, 0, sizeof(tokenBuf));
	memset(buf, 0, sizeof(buf));

	while ((ch = MoveForwardGetChar()) != -1)
	{
		if (ch == '\t' || ch == '\n' || ch == ' ')
		{
			while (ch == '\t' || ch == '\n' || ch == ' ')
			{
				if (ch == '\n')
					lineCount++;
				ch = MoveForwardGetChar();
			}
			Moveback();
			*crtPos = *forwardPos;
			continue;
		}
		if (isalpha(ch)){		
			while (isalpha(ch))
				ch = MoveForwardGetChar();
			Moveback();
			DealToken(tokenBuf, T_IDN);		
			continue;
		}
		if (isdigit(ch)){
			while (isdigit(ch))
				ch = MoveForwardGetChar();
			Moveback();
			DealToken(tokenBuf, T_INT);
			continue;
		}
		switch (ch)
		{
		case ':':
			ch = MoveForwardGetChar();
			if (ch == '=')
				DealToken(tokenBuf, T_ASS);
			else
			{
				Moveback();
				DealToken(tokenBuf, T_COLON);
			}
			break;
		case '=':
			DealToken(tokenBuf, T_EQL);
			break;
		case ';':
			DealToken(tokenBuf, T_SEMICL);
			break;
		case '*':
			ch = MoveForwardGetChar();
			if (ch == '*')
				DealToken(tokenBuf, T_POW);
			else if (ch == '=')
				DealToken(tokenBuf, T_MULE);
			else
			{
				Moveback();
				DealToken(tokenBuf, T_MUL);
			}
			break;
		case '+':
			ch = MoveForwardGetChar();
			if (ch == '=')
				DealToken(tokenBuf, T_ADDE);
			else
			{
				Moveback();
				DealToken(tokenBuf, T_ADD);
			}
			break;
		case '-':
			ch = MoveForwardGetChar();
			if (ch == '=')
				DealToken(tokenBuf, T_SUBE);
			else
			{
				Moveback();
				DealToken(tokenBuf, T_SUB);
			}
			break;
		case '/':
			DealToken(tokenBuf, T_SLASH);
			break;
		case '(':
			DealToken(tokenBuf, T_LPAR);
			break;
		case ')':
			DealToken(tokenBuf, T_RPAR);
			break;
		case '.':
			DealToken(tokenBuf, T_PEROID);
			break;
		case 34:				//  Double Quotation  " 
			ch = MoveForwardGetChar();
			while (ch != 34)
			{
				ch = MoveForwardGetChar();
			}
			// Todo :length restriction
			DealToken(tokenBuf, T_STR);
			// ErrHandle();
			break;
		case 39:				 // Singlequotation '
			ch = MoveForwardGetChar();
			while (ch != 39)
				ch = MoveForwardGetChar();
			// Todo :length restriction
			DealToken(tokenBuf, T_STR);
			//ErrHandle();
			break;
		}
	}
	fclose(fp);
	fclose(tokenOutFp);
}
