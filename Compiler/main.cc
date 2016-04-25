#define _CRT_SECURE_NO_WARNINGS 1 
#include "scanner.h"
#include "parser.h"
#include <cstdio>

int main()
{
	Scanner* scanner = new Scanner;
	scanner->Init();
	scanner->ScanToken();


	Parser* parser = new Parser;
	parser->Init();
	parser->Startup(scanner->token_list_);

	delete scanner;
	delete parser;
	return 0;
}