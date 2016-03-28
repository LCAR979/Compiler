#include <stdio.h>
#include "common.h"
#include "hash_table.h"
#include "parser.h"

void Parser::Init()
{
	grammer_fp_ = fopen("grammer.txt", "r");
	Production* pr = new Production;
	while (fscanf(grammer_fp_, "%c %s", &(pr->l_part), &(pr->r_part)) == 2)
		l2r_production_table_.Insert(pr->l_part, pr);
}
void Parser::ErrorHandle()
{
}
