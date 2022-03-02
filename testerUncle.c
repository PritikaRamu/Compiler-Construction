#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum {
	program, mainFunction, otherFunctions, function, input_par, output_par,
	parameter_list, dataType, primitiveDatatype, constructedDatatype,
	remaining_list, stmts, typeDefinitions, typeDefinition, fieldDefinitions,
	fieldDefinition, moreFields, declarations, declaration, global_or_not,
	otherStmts, stmt, assignmentStmt, singleOrRecId, actualOrRedefined, funCallStmt,
	outputParameters, inputParameters, iterativeStmt, conditionalStmt, elsePart,
	ioStmt, fieldtype, option_single_constructed, arithmeticExpression, expPrime, term, termPrime, factor,
	highPrecedenceOperators, lowPrecedenceOperators, oneExpansion, moreExpansions, booleanExpression,
	var, logicalOp, relationalOp, returnStmt, optionalReturn, idList, more_ids,
	eps,  TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM,
	 TK_FUNID, TK_WITH, TK_PARAMETERS, TK_END, TK_WHILE, TK_TYPE, TK_MAIN,
	TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT,
	TK_REAL, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF,
	TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV,
	TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_AS,
	TK_EQ, TK_GT, TK_GE, TK_NE, SENTINEL, LEX_ERROR, TK_RUID, TK_UNION, TK_ENDUNION, TK_DEFINETYPE,
	definetypestmt, A
} g_Term;

struct tree {
    //TreeNode* data;
    g_Term symbol;
    bool isTerminal;
    int lineNo;
    struct tree* firstChild;
    struct tree* nextSibling;
    struct tree* parent;
};

typedef struct tree* parseTree;

parseTree getNearestUncle(parseTree curr){
	//reached root of tree
	if(!curr->parent){
		return NULL;
	}
	if(curr->parent->nextSibling){
		return curr->parent->nextSibling;
	}
	else return getNearestUncle(curr->parent);
}

int main(){
        parseTree root = (parseTree)malloc(sizeof(struct tree));
        parseTree uncle = (parseTree)malloc(sizeof(struct tree));
        parseTree fChild = (parseTree)malloc(sizeof(struct tree));
        parseTree sChild = (parseTree)malloc(sizeof(struct tree));
        parseTree grandChild = (parseTree)malloc(sizeof(struct tree));
        parseTree greatChild = (parseTree)malloc(sizeof(struct tree));

        root->symbol = program;
        root->isTerminal = true;
        root->firstChild = fChild;
        root->nextSibling = uncle;
        root->parent = NULL;

        uncle->symbol = function;
        uncle->isTerminal = true;
        uncle->firstChild = NULL;
        uncle->nextSibling = NULL;
        uncle->parent = root;

        fChild->symbol = eps;
        fChild->isTerminal = true;
        fChild->firstChild = grandChild;
        fChild->nextSibling = sChild;
        fChild->parent = root;

        sChild->symbol = mainFunction;
        sChild->isTerminal = true;
        sChild->firstChild = NULL;
        sChild->nextSibling = NULL;
        sChild->parent = root;

        grandChild->symbol = otherFunctions;
        grandChild->isTerminal = true;
        grandChild->firstChild = NULL;
        grandChild->nextSibling = NULL;
        grandChild->parent = fChild;

        greatChild->symbol = eps;
        greatChild->isTerminal = true;
        greatChild->firstChild = NULL;
        greatChild->nextSibling = NULL;
        greatChild->parent = grandChild;

        parseTree test1 = (parseTree)malloc(sizeof(struct tree));
        parseTree test2 = (parseTree)malloc(sizeof(struct tree));
        parseTree test3 = (parseTree)malloc(sizeof(struct tree));
        parseTree test4 = (parseTree)malloc(sizeof(struct tree));
        test1 = getNearestUncle(fChild);
        test2 = getNearestUncle(grandChild);
        test3 = getNearestUncle(greatChild);
        test4 = getNearestUncle(root);
        if(test4 == NULL){
            printf("root has no uncle.\n");
        }
        printf("%d\n",test1->symbol);
        printf("%d\n",test2->symbol);
        printf("%d\n",test3->symbol);
        //shoudn't print anything
        printf("%d\n",test4->symbol);
        return 0;
    
}