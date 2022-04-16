#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_gen.h"


asmTracker finalAsm; // Global

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
    finalAsm.labelCount++;
    sprintf(buff, "label%d", finalAsm.labelCount); // Creates a new label, puts it in this string

}

void resetTemps(){

    for(int i=0;i<MAX_TEMP_WIDTH;i++){
        finalAsm.currentTempCount[i] = 0; // Set Current Temp Count to zero, but not "Temp Count", it's cummulative
    }

}

void getTemp(char *buff, int width) {
	finalAsm.currentTempCount[width]+=1;

	sprintf(buff, "temp%d + %d", width, (finalAsm.currentTempCount[width] - 1) * width); // Copies Base-Relative Address of the new Temp to this string
	
    if (finalAsm.currentTempCount[width] > finalAsm.tempCount[width]) { // Update the TempCount array!
		finalAsm.tempCount[width]+=1;
	}
}

// Sets ASM Tracker fields to Zero
void initAsm(){
    for(int i=0;i<MAX_TEMP_WIDTH;i++){
        finalAsm.tempCount[i] = finalAsm.currentTempCount[i] = 0;
    }

    finalAsm.labelCount = 0;
}

// Helper
void printAsmTracker(){
    printf("\nASM Tracker:\n");
    printf("Temp Count: ");
    for(int i=0;i<MAX_TEMP_WIDTH;i++){
        printf("%d,",finalAsm.tempCount[i]);
    }
    printf("\n");

    printf("Current Temp Count: ");
    for(int i=0;i<MAX_TEMP_WIDTH;i++){
        printf("%d,",finalAsm.currentTempCount[i]);
    }
    printf("\n");

    printf("Label Count: %d\n",finalAsm.labelCount);   
    printf("\n");

}

// int main(){
//     char* string = (char*)malloc(sizeof(char)*123);

//     initAsm();    


//     printf("\tmov\tr8w, %s\n", "4");
// 	getTemp(string, 2);
// 	printf("\tmov\t[%s], r8w\n", string);
//     printAsmTracker();
    
// }