/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/
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