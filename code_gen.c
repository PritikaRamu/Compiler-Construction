#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_gen.h"

void generateAsm(ast* root, FILE *fptr){

}

void genAsmTypeDefs(ast* typeDefsNode, FILE *fptr){

}

void genAsmDecls(ast* declsNode, FILE *fptr){

}

void genAsmOtherStmts(ast* otherStmtsNode, FILE *fptr){

}

void genReadStmt(ast* readNode, FILE *fptr){

}

void genWriteStmt(ast* writeNode, FILE *fptr){

}

void genCondStmt(ast* condNode, FILE *fptr){

}

void genIterStmt(ast* iterNode, FILE *fptr){

}

void genBoolExpr(ast* boolNode, char *result, FILE *fptr){

}

void genAssignStmt(ast* assignNode, FILE *fptr){

}

void genArithExpr(ast* arithNode, char *result, FILE *fptr) {

}

void getLabel(char *buff){

}

void resetTemps(){

}

void getTemp(char *buff, int width){

}

void initAsm(){
    
}