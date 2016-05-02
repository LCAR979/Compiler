#define _CRT_SECURE_NO_WARNINGS 1 
#include "scanner.h"
#include "parser.h"
#include <cstdio>

int main()
{
	Scanner scanner;
	scanner.Init();
	scanner.ScanToken();

	Parser parser;
	parser.Init();
	parser.Startup(scanner.token_list_);

	return 0;
}