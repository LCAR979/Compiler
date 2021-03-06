#define _CRT_SECURE_NO_WARNINGS 1 
#include "scanner.h"
#include "parser.h"
#include <cstdio>

int main()
{
	printf("%d", GetArrayLen(word_list));
	Scanner scanner;
	scanner.Init();
	scanner.ScanToken();

	Parser parser;
	parser.Init();
	parser.Startup();


	ErrorPrint();

	return 0;
}