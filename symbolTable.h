/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/
#ifndef _ST_
#define _ST_

#include "symboltableDef.h"

void initializeSymbolTable(ast* ast);
void printRecordTable(subTable* rec_table);
void printFunctionTable(subTable* fun_table);
void* retrieve(symbol_Table* st, void* node, NodeType type);
identifierNode* createINode(ast* id, ast* func,Type type, bool is_global, int*offset);
functionNode* createFNode(ast* curr);
void printIDTable(subTable *fun_table);
void* retrieve(symbol_Table *st, void *node, NodeType type);
void printFPTable(subTable *fun_table);
void printAliasTable(subTable *fun_table);
identifierNode* retrieveFake(subTable* st, identifierNode* id, bool token, bool alias);
void printSymbolTable(symbol_Table* st);
int GodHelpMe(char* a, char* b, bool global, ast* node);
int GodHelpMeForUnion(char* unionName, char* dotName, bool global, ast*func);
void addAlias(identifierNode* existing,identifierNode* alias);
void addID(functionNode* exist, identifierNode* id);
void printIDList(functionNode* exist);
void print_Redeclarations();
void addErrorToArray(char* errorMessage);
void printFinalTable(subTable *fun_table);
void printGlobalTable(subTable *fun_table);

#endif