#ifndef SCANNER_H 
#define SCANNER_H

#define TOKEN_MAX_N 128
#define BUFFER_SIZE 128

FILE *fp;
FILE *tokenOutFp;
char buf[BUFFER_SIZE];
char tokenBuf[TOKEN_MAX_N];

//suppose we have less than BUFFERSIZE(128) tokens
int tokenArr[BUFFER_SIZE]; 

void InitTokenArr();
void ScanToken();
#endif