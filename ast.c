#include "ast.h"

ast* mkNode(NodeType nodeType, ast* parent, ast* firstChild, ast* nextSibling, parseTree ptNode) {
    ast* node = (ast*)malloc(sizeof(ast));
    node->nodeType = nodeType;
    node->parent = parent;
    node->firstChild = firstChild;
    node->nextSibling = nextSibling;
    node->symbol = ptNode->symbol;
    node->lex = ptNode->lex;
    node->line = ptNode->lineNo;
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
    ast* curr, *temp1;

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
            curr = makeAST(node->firstChild, parent);
            curr->nextSibling = makeAST(node->firstChild->nextSibling, parent);
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
        curr = mkNode(OUTPUT_PARAMETERS, parent, NULL, NULL, node->firstChild);
        curr->firstChild = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling, curr);
    }

    //parameter_list ==> dataType TK_ID remaining_list
    if(node->symbol == parameter_list) {
        curr = makeAST(node->firstChild, parent);
        curr->firstChild = mkNode(ID, curr, NULL, NULL, curr->firstChild);
        curr->nextSibling = makeAST(node->firstChild->nextSibling, parent);
    }

    //remaining_list ==> TK_COMMA <parameter_list>
    if(node->symbol == remaining_list) {
        curr = makeAST(node->firstChild->nextSibling, parent);
    }

    //dataType ==> primitiveDatatype | constructedDatatype
    if(node->symbol == dataType) {
        curr = makeAST(node->firstChild, parent);
    }

    if(node->symbol == primitiveDatatype) {
        //primitiveDatatype ==> TK_INT
        if(node->firstChild->symbol == TK_INT) {
            curr = mkNode(INTEGER, parent, NULL, NULL, node->firstChild);
        } //primtitiveDatatype ==> TK_REAL
        else if(node->firstChild->symbol == TK_REAL) {
            curr = mkNode(REAL, parent, NULL, NULL, node->firstChild);
        }
    }

    
    if(node->symbol == constructedDatatype) {
        //constructedDatatype ==> TK_RECORD TK_RUID
        if(node->firstChild->symbol == TK_RECORD) {
            curr = mkNode(RECORD_OR_UNION, parent, NULL, NULL, node->firstChild->nextSibling);
        } //constructedDatatype ==> TK_UNION TK_RUID 
        else if (node->firstChild->symbol == TK_UNION) {
            curr = mkNode(RECORD_OR_UNION, parent, NULL, NULL, node->firstChild->nextSibling);
        } //constructedDatatype ==> TK_RUID
        else if(node->firstChild->symbol == TK_RUID) {
            curr = mkNode(RECORD_OR_UNION, parent, NULL, NULL, node->firstChild);
        }
    }

    //stmts ==> <typeDefinitions> <declarations> <otherStmts> <returnStmt>
    if(node->symbol == stmts) {
        curr = makeAST(node->firstChild, parent);

        if(curr == NULL) {
            curr = mkNode(TYPE_DEFINITIONS, parent, NULL, NULL, node->firstChild);
        }

        temp1 = curr;
        curr = lastNode(curr);

        curr->nextSibling = makeAST(node->firstChild->nextSibling, parent);

        if(curr == NULL) {
            curr = mkNode(DECLARATIONS, parent, NULL, NULL, node->firstChild->nextSibling);
        }

        curr = lastNode(curr);

        curr->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling, parent);

        if(curr==NULL) {
            curr = mkNode(OTHERSTMTS, parent, NULL, NULL, node->firstChild->nextSibling->nextSibling);
        }

        curr = lastNode(curr);

        curr->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling, parent);

        curr = temp1;
    }

    //typeDefinitions ==> <actualOrRedefined> <typeDefinitions>
    if(node->symbol == typeDefinitions) {
        //we expand on the actualOrRedefined and typeDefinitions
        //linearizing all the type definitions
        //child can either be a definetype node or RECORD_OR_UNION node 
        curr = makeAST(node->firstChild, parent);
    }

    //actualOrRedefined ==> definetypestmt | typeDefinition
    if(node->symbol == actualOrRedefined) {
        curr = makeAST(node->firstChild, parent);
    }  

    //definetypestmt ==> TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID TK_SEM
    if(node->symbol == definetypestmt) {
        curr = mkNode(DEFINETYPE, parent, NULL, NULL, node->firstChild);
        curr->firstChild = makeAST(node->firstChild, curr);
        curr->firstChild->nextSibling = mkNode(RECORD_OR_UNION, curr, NULL, NULL, node->firstChild->nextSibling->nextSibling);
        curr->firstChild->nextSibling->nextSibling = mkNode(RECORD_OR_UNION, curr, NULL, NULL, node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling);
    }

    //A ==> TK_RECORD | TK_UNION
    if(node->symbol == A) {
        curr = mkNode(RECORD_OR_UNION, parent, NULL, NULL, node->firstChild);
    }

    //typeDefinition ==> TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD | TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
    if(node->symbol == typeDefinition) {
        curr = mkNode(RECORD_OR_UNION, parent, NULL, NULL, node->firstChild);
        curr->firstChild = makeAST(node->firstChild->nextSibling->nextSibling, curr);
    }

    //fieldDefinitions -> <fieldDefinition> <fieldDefinition> <moreFields>
    if(node->symbol == fieldDefinitions) {
        curr = makeAST(node->firstChild, parent);
        curr->nextSibling = makeAST(node->firstChild->nextSibling, parent);
        curr->nextSibling->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling, parent);
    }

    //fieldDefinition ==> TK_TYPE <fieldType > TK_COLON TK_FIELDID TK_SEM
    if(node->symbol == fieldDefinition) {
        //ignore the TYPE and ; node 
        curr = makeAST(node->firstChild->nextSibling, parent);
        curr->firstChild = mkNode(FIELDID, curr, NULL, NULL, node->firstChild->nextSibling->nextSibling->nextSibling);
    }

    //fieldType ==> primitiveDatatype | TK_RUID
    if(node->symbol == fieldtype) {
        if(node->firstChild->symbol == primitiveDatatype) {
            curr = makeAST(node->firstChild, parent);
        }
        else if(node->firstChild->symbol == TK_RUID) {
            curr = mkNode(RECORD_OR_UNION, parent, NULL, NULL, node->firstChild);
        }
    }

    //moreFields ==> <fieldDefinition> <moreFields>
    if(node->symbol == moreFields) {
        curr = makeAST(node->firstChild, parent);
        curr->nextSibling = makeAST(node->firstChild->nextSibling, parent);
    }

    //declarations ==> <declaration> <declarations>
    if(node->symbol == declarations) {
        curr = makeAST(node->firstChild, parent);
        curr->nextSibling = makeAST(node->firstChild->nextSibling, parent);
    }

    //declaration ==>TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
    if(node->symbol == declaration) {
        curr = makeAST(node->firstChild, parent);
        curr->firstChild = mkNode(ID, curr, NULL, NULL, node->firstChild->nextSibling->nextSibling->nextSibling);
        curr->firstChild->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling, curr);
    }

    //global_or_not ==> TK_COLON TK_GLOBAL
    if(node->symbol == global_or_not) {
        curr = mkNode(GLOBAL, parent, NULL, NULL, node->firstChild->nextSibling);
    }

    //<otherStmts> ==> <stmt> <otherStmts>
    if(node->symbol == otherStmts) {
        curr = makeAST(node->firstChild, parent);
        curr->nextSibling = makeAST(node->firstChild->nextSibling, parent);
    }   

    if(node->symbol == stmt) {
        switch(node->firstChild->symbol) {
            //stmt ==> assignmentStmt
            case assignmentStmt:
                curr = makeAST(node->firstChild, parent);
                break;
            //stmt ==> iterativeStmt
            case iterativeStmt:
                curr = makeAST(node->firstChild, parent);
            //stmt ==> conditionalStmt
        }
    }
}

ast* initASt(parseTree root) {
    return makeAST(root, NULL);
}