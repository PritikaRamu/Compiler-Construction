#include "symbolTable.h" 

bool isIntReal(ast* curr);
bool isOperator(ast* curr);

void validateFunction(ast* curr);
void semanticAnalyser(ast* root);
identifierNode* validateArithmetic(ast* curr, ast* func);
void validateFunCall();
