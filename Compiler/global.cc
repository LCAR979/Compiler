#include <string.h>
#include <vector>
#include "global.h"
#include "hash_table.h"

int line_number = 0;
bool fatel_error = false;

HashTable<char*, int> keyword_table;

char token_name_arr[kTokenNameArrLen];
int token_name_arr_tail = 0;


std::vector<int> const_int_vec;
std::vector<double> const_real_vec;

std::vector<TokenItem> token_vec;
std::vector<ErrorItem> error_vec;
GeneralStack<Item> item_stack;

void ErrorHandle(char* err_msg)
{
	error_vec.push_back(ErrorItem(line_number, err_msg));
}

void ErrorPrint()
{
	int error_count = 0;
	FILE* error_fp = fopen("output\\error.txt", "w");
	std::vector<ErrorItem>::iterator it;
	for (it = error_vec.begin(); it != error_vec.end(); it++)
	{
		printf("Line %d, %s\n", it->line, it->description);
		fprintf(error_fp, "Line %d, %s\n", it->line, it->description);
		error_count++;
	}

	if (error_count != 0)
		printf(" Error count %d\n", error_count);
	else
	{
		printf("No token error\n");
	}
	fclose(error_fp);
}