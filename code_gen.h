#ifndef _ASSEMBLY_CODE_GEN_
#define _ASSEMBLY_CODE_GEN_

#include <stdio.h>
#include "ast.h"

#define MAX_TEMP_WIDTH 8

typedef struct _asmTracker{
    int tempCount[MAX_TEMP_WIDTH+1];
    int currentTempCount[MAX_TEMP_WIDTH+1];
    int labelCount;

} asmTracker;

void generateAsm(ast* root, FILE *fptr);

void genAsmTypeDefs(ast* typeDefsNode, FILE *fptr);

void genAsmDecls(ast* declsNode, FILE *fptr);

void genAsmOtherStmts(ast* otherStmtsNode, FILE *fptr);

void genReadStmt(ast* readNode, FILE *fptr);

void genWriteStmt(ast* writeNode, FILE *fptr);

void genCondStmt(ast* condNode, FILE *fptr);

void genIterStmt(ast* iterNode, FILE *fptr);

void genBoolExpr(ast* boolNode, char *result, FILE *fptr);

void genAssignStmt(ast* assignNode, FILE *fptr);

void genArithExpr(ast* arithNode, char *result, FILE *fptr) ;

void getLabel(char *buff);

void resetTemps();

void getTemp(char *buff, int width);

void initAsm() ;

#endif