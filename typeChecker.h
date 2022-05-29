#include "symbolTable.h" 

bool isIntReal(ast* curr);
bool isOperator(ast* curr);

void validateFunction(ast* curr);
void semanticAnalyser(ast* root);
identifierNode* validateArithmetic(ast* curr, ast* func);
void validateFunCall(ast* curr, ast* func);
void validateConditional(ast* curr, ast* func);

void printSemanticErrors();
void addSemErrortoArray(char* message);

char sem_error_array[1000][100];
int semanticErrors;