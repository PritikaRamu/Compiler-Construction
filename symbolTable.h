#ifndef _ST_
#define _ST_

#include "symboltableDef.h"

void initializeSymbolTable(ast* ast);
void printRecordTable(subTable* rec_table);
void printFunctionTable(subTable* fun_table);

#endif