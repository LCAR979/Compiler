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