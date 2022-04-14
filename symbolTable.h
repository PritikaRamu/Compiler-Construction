#ifndef _ST_
#define _ST_

#include "symboltableDef.h"

void initializeSymbolTable(ast* ast);
void printRecordTable(subTable* rec_table);
void printFunctionTable(subTable* fun_table);
void* retrieve(symbol_Table* st, void* node, NodeType type);
identifierNode* createINode(ast* id, ast* func, Type type, bool is_global, int*offset);

#endif