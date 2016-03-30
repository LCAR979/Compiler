#define _CRT_SECURE_NO_WARNINGS 1 
#include "scanner.h"
#include "parser.h"
#include <map>

#include <cstdio>
#include "dynamic_array.h" 

int main()
{

	Scanner* scanner = new Scanner;
	scanner->Init();
	scanner->ScanToken();

	Parser parser;
	parser.Init();
	parser.FillActionTable();
	parser.FillGotoTable();
	parser.Startup();

	return 0;
}