#include "scanner.h"
int main()
{
	Scanner* scanner = new Scanner;
	scanner->Init();
	scanner->ScanToken();
	return 0;
}