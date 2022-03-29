#include "ast.h"

ast* mkNode(NodeType nodeType, ast* parent, ast* firstChild, ast* nextSibling, parseTree ptNode) {
    ast* node = (ast*)malloc(sizeof(ast));
    node->nodeType = nodeType;
    node->parent = parent;
    node->firstChild = firstChild;
    node->nextSibling = nextSibling;
    node->symbol = ptNode->symbol;
    node->lex = ptNode->lex;
    node->line = ptNode->line;
    return node;
}

ast* lastNode(ast* head) {
    ast* temp = head;
    while(temp->nextSibling != NULL) {
        temp = temp->nextSibling;
    }
    return temp;
}

ast* makeAST(parseTree node, ast* parent) {
    ast* curr;

    //if the LHS produces epsilon then the rule is useless
    //remove the node
    //Non-Terminal ==> eps
    if(node->firstChild->symbol == eps) {
        return NULL;
    }

    //program ==> otherFunctions mainFunction
    if(node->symbol == program) {
        curr = mkNode(PROGRAM, parent, NULL, NULL, node->symbol);
        curr->firstChild = makeAST(node->firstChild, curr);
        if(curr->firstChild == NULL) {
            curr->firstChild = makeAST(node->firstChild->nextSibling, parent);
        }
        else {
            ast* last = lastNode(curr->firstChild);
            last->nextSibling = makeAST(node->firstChild->nextSibling, parent);
        }
    }

    //mainFunction ==> TK_MAIN <stmts> TK_END
    //create a leaf node for TK_MAIN and ignore TK_END
    if(node->symbol == mainFunction) {
        curr = mkNode(MAIN_FUNCTION, parent, NULL, NULL, node->firstChild);
        curr->nextSibling = makeAST(node->firstChild->nextSibling, parent);
    }

    //otherFunctions ==> function otherFunctions
    if(node->symbol == otherFunctions) {
        if(node->firstChild->symbol == function) {
            curr = makeAST(otherFunctions->firstChild, parent);
            curr->nextSibling = makeAST(otherFunctions->firstChild->nextSibling, parent);
        }
    }

    //function ==> TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
    if(node->symbol == function) {
        curr = mkNode(FUNCTION_SEQ, parent, NULL, NULL, node->firstChild);
        curr->firstChild = makeAST(node->firstChild->nextSibling, curr); //for input_par
        curr->firstChild->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling, curr); //for output_par
        curr->firstChild->nextSibling->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling, curr); //for stmts
    }

    //input_par ==> TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    if(node->symbol == input_par) {
        curr = mkNode(INPUT_PARAMETERS, parent, NULL, NULL, node->firstChild);
        curr->firstChild = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling, curr);
    }

    //output_par ==> TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    if(node->symbol == output_par) {
        curr = mkNode(OUTPUT_PARAMETERS, parent, NULL, NULL, node->firstChild)
        curr->firstChild = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling, curr);
    }
}

ast* initASt(parseTree root) {
    return makeAST(root, NULL);
}