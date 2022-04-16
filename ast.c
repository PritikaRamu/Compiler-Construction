/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/
#include "ast.h"

char nodeTypeStr[][50] = {
    "PROGRAM",
    "MAIN_FUNCTION", 
    "FUNCTION_SEQ", 
    "INPUT_PARAMETERS",
    "OUTPUT_PARAMETERS",
    "INTEGER",
    "REAL",
    "RECORD_OR_UNION",
    "ID",
    "FIELDID",
    "TYPE_DEFINITIONS",
    "DEFINETYPE",
    "DECLARATIONS",
    "OTHERSTMTS",
    "GLOBAL", 
    "ASSIGNOP",
    "CALL", 
    "FUNCTION_CALL",
    "OUTPUT_PARAMETERS_CALL",
    "INPUT_PARAMETERS_CALL",
    "ITERATIVE",
    "CONDITIONAL",
    "ELSE",
    "IOREAD", 
    "IOWRITE",
    "MULTIPLY",
    "DIVIDE",
    "PLUS", 
    "MINUS",
    "NOT_BOOL",
    "NUM", 
    "RNUM",
    "AND",
    "OR",
    "LESS_THAN",
    "LESS_EQUAL",
    "EQUAL",
    "GREATER_THAN",
    "GREATER_EQUAL",
    "NOT_EQUAL",
    "RETURN"
};

char* nodeTypeToStr(NodeType type) {
    return nodeTypeStr[type];
}

ast* mkNode(NodeType nodeType, ast* parent, ast* firstChild, ast* nextSibling, parseTree ptNode) {
    ast* node = (ast*)malloc(sizeof(ast));
    node->nodeType = nodeType;
    node->parent = parent;
    node->firstChild = firstChild;
    node->nextSibling = nextSibling;
    node->symbol = ptNode->symbol;
    node->lex = ptNode->lex;
    node->line = ptNode->lineNo;
    node->is_union = false;
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
    //printf("\nCurrent parseTree node: %d\n", node->symbol);

    //if the LHS produces epsilon then the rule is useless
    //remove the node
    //Non-Terminal ==> eps
    if(node->firstChild->symbol == eps) {
        return NULL;
    }

    //program ==> otherFunctions mainFunction
    if(node->symbol == program) {
        curr = mkNode(PROGRAM, parent, NULL, NULL, node);
        //printf("%d\n", node->firstChild->symbol);
        //printf("Before otherFunctions\n");
        curr->firstChild = makeAST(node->firstChild, curr);
        if(curr->firstChild == NULL) {
            curr->firstChild = makeAST(node->firstChild->nextSibling, curr);
        }
        else {
            ast* last = lastNode(curr->firstChild);
            //printf("After otherFunctions\n");
            last->nextSibling = makeAST(node->firstChild->nextSibling, curr);
        }
    }

    //mainFunction ==> TK_MAIN <stmts> TK_END
    //create a leaf node for TK_MAIN and ignore TK_END
    if(node->symbol == mainFunction) {
        curr = mkNode(MAIN_FUNCTION, parent, NULL, NULL, node->firstChild);
        curr->firstChild = makeAST(node->firstChild->nextSibling, curr);
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
       // printf("Before input_par\n");
        curr->firstChild = makeAST(node->firstChild->nextSibling, curr); //for input_par
       // printf("Before output_par\n");
        curr->firstChild->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling, curr); //for output_par
        if(curr->firstChild->nextSibling == NULL) {
            curr->firstChild->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling, curr); //for stmts
        }
        else {
            curr->firstChild->nextSibling->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling, curr); //for stmts
        }
    }

    //input_par ==> TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    if(node->symbol == input_par) {
        curr = mkNode(INPUT_PARAMETERS, parent, NULL, NULL, node->firstChild);
        //printf("Before parameter_list\n");
        curr->firstChild = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling, curr);
        //printf("After parameter_list\n");
    }

    //output_par ==> TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    if(node->symbol == output_par) {
        curr = mkNode(OUTPUT_PARAMETERS, parent, NULL, NULL, node->firstChild);
        curr->firstChild = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling, curr);
    }

    //parameter_list ==> dataType TK_ID remaining_list
    if(node->symbol == parameter_list) {
        //printf("Before dataType\n");
        curr = makeAST(node->firstChild, parent);
        //printf("After data type\n");
        curr->firstChild = mkNode(ID, curr, NULL, NULL, node->firstChild->nextSibling);
       // printf("Before remlist\n");
        curr->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling, parent);
        //printf("After remlist\n");
    }

    //remaining_list ==> TK_COMMA <parameter_list>
    if(node->symbol == remaining_list) {
       // printf("Start remlist\n");
        curr = makeAST(node->firstChild->nextSibling, parent);
       // printf("Inside remlist\n");
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
            curr->is_union = true;
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
       // printf("After Typedefinitons\n");

        curr->nextSibling = makeAST(node->firstChild->nextSibling, parent);
       // printf("Here1\n");

        if(curr->nextSibling == NULL) {
            curr->nextSibling = mkNode(DECLARATIONS, parent, NULL, NULL, node->firstChild->nextSibling);
        }
       // printf("After Declarations\n");

        curr = lastNode(curr);

        curr->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling, parent);

        if(curr->nextSibling == NULL) {
            curr->nextSibling = mkNode(OTHERSTMTS, parent, NULL, NULL, node->firstChild->nextSibling->nextSibling);
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
        curr->nextSibling = makeAST(node->firstChild->nextSibling, parent);
    }

    //actualOrRedefined ==> definetypestmt | typeDefinition
    if(node->symbol == actualOrRedefined) {
        curr = makeAST(node->firstChild, parent);
    }  

    //definetypestmt ==> TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID TK_SEM
    if(node->symbol == definetypestmt) {
        curr = mkNode(DEFINETYPE, parent, NULL, NULL, node->firstChild);
        curr->firstChild = makeAST(node->firstChild->nextSibling, curr);
        curr->firstChild->nextSibling = mkNode(RECORD_OR_UNION, curr, NULL, NULL, node->firstChild->nextSibling->nextSibling);
        //curr->firstChild->nextSibling->is_union = (curr->firstChild->symbol == TK_UNION);
        curr->firstChild->nextSibling->nextSibling = mkNode(RECORD_OR_UNION, curr, NULL, NULL, node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling);
        //curr->firstChild->nextSibling->nextSibling->is_union = curr->firstChild->nextSibling->is_union;
    }

    //A ==> TK_RECORD | TK_UNION
    if(node->symbol == A) {
        curr = mkNode(RECORD_OR_UNION, parent, NULL, NULL, node->firstChild);
        curr->is_union = (node->firstChild->symbol == TK_UNION) ? true : false;
    }

    //typeDefinition ==> TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD | TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
    if(node->symbol == typeDefinition) {
        curr = mkNode(RECORD_OR_UNION, parent, NULL, NULL, node->firstChild->nextSibling);
        curr->is_union = (node->firstChild->symbol == TK_UNION) ? true : false;
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
       // printf("In declarations\n");
        curr = makeAST(node->firstChild, parent);
       // printf("After declaration\n");
        curr->nextSibling = makeAST(node->firstChild->nextSibling, parent);
    }

    //declaration ==>TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
    if(node->symbol == declaration) {
        curr = makeAST(node->firstChild->nextSibling, parent);
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
                break;
            //stmt ==> conditionalStmt
            case conditionalStmt:
                curr = makeAST(node->firstChild, parent);
                break;
            //stmt ==> ioStmt
            case ioStmt:
                curr = makeAST(node->firstChild, parent);
                break;
            //stmt ==> funCallStmt
            case funCallStmt:
                curr = makeAST(node->firstChild, parent);
                break;
        }
    }

    //assignmentStmt ==> <SingleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
    if(node->symbol == assignmentStmt) {
        curr = mkNode(ASSIGNOP, parent, NULL, NULL, node->firstChild->nextSibling);
        curr->firstChild = makeAST(node->firstChild, curr);
        curr->firstChild->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling, curr);
    }

    //<SingleOrRecId> ==> TK_ID <option_single_constructed>
    if(node->symbol == singleOrRecId) {
        //of the form:-
        /*
                    ID
                   /
                  /
                FIELID-->FIELDID-->FIELDID....--FIELDID 
        */
        curr = mkNode(ID, parent, NULL, NULL, node->firstChild);
        curr->firstChild = makeAST(node->firstChild->nextSibling, curr);
    } 

    //<option_single_constructed> ==> <oneExpansion> <moreExpansions>
    if(node->symbol == option_single_constructed) {
       // printf("Entering option_single_constructed\n");
        curr = makeAST(node->firstChild, parent);
        curr->nextSibling = makeAST(node->firstChild->nextSibling, parent);
       // printf("Exiting option_single_constructed\n");
    }

    //oneExpansion ==> TK_DOT TK_FIELDID
    if(node->symbol == oneExpansion) {
        curr = mkNode(FIELDID, parent, NULL, NULL, node->firstChild->nextSibling);
    }

    //moreExpansions ==> oneExpansion moreExpansions
    if(node->symbol == moreExpansions) {
        curr = makeAST(node->firstChild, parent);
        curr->nextSibling = makeAST(node->firstChild->nextSibling, parent);
    }

    //<funCallStmt> ==> <outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
    if(node->symbol == funCallStmt) {
        curr = mkNode(CALL, parent, NULL, NULL, node->firstChild->nextSibling);
        curr->firstChild = makeAST(node->firstChild, curr);
        if(curr->firstChild == NULL) {
            curr->firstChild = mkNode(OUTPUT_PARAMETERS_CALL, parent, NULL, NULL, node->firstChild);
        }
        curr->firstChild->nextSibling = mkNode(FUNCTION_CALL, curr, NULL, NULL, node->firstChild->nextSibling->nextSibling);
        curr->firstChild->nextSibling->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling, curr);
    }

    //<outputParameters> ==> TK_SQL <idList> TK_SQR TK_ASSIGNOP
    if(node->symbol == outputParameters) {
        curr = mkNode(OUTPUT_PARAMETERS_CALL, parent, NULL, NULL, node);
        curr->firstChild = makeAST(node->firstChild->nextSibling, curr);
    }

    //<inputParameters> ==> TK_SQL <idList> TK_SQR
    if(node->symbol == inputParameters) {
        curr = mkNode(INPUT_PARAMETERS_CALL, parent, NULL, NULL, node);
        curr->firstChild = makeAST(node->firstChild->nextSibling, curr);
    }

    //<iterativeStmt> ==> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
    if(node->symbol == iterativeStmt) {
        curr = mkNode(ITERATIVE, parent, NULL, NULL, node->firstChild);
       // printf("\nBefore boolexpr\n");
        curr->firstChild = makeAST(node->firstChild->nextSibling->nextSibling, curr);
      //  printf("\nAfter boolexpr\n");
        curr->firstChild->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling, curr);
      //  printf("\nAfter stmt\n");
        curr->firstChild->nextSibling->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling, curr);
      //  printf("\nAfter otherStmts\n");
    }

    //<conditionalStmt>-> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <elsePart>
    if(node->symbol == conditionalStmt) {
        curr = mkNode(CONDITIONAL, parent, NULL, NULL, node->firstChild);
        curr->firstChild = makeAST(node->firstChild->nextSibling->nextSibling, curr);
        curr->firstChild->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling, curr);
        curr->firstChild->nextSibling->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling, curr);
        temp1 = curr->firstChild->nextSibling;
        temp1 = lastNode(temp1);
        temp1->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling, curr);
    }

    if(node->symbol == elsePart) {
        switch(node->firstChild->symbol) {
            //<elsePart> ==> TK_ENDIF
            case TK_ENDIF:
                return NULL;
                break;
            //<elsePart> ==> TK_ELSE <stmt> <otherStmts> TK_ENDIF 
            case TK_ELSE:
                curr = mkNode(ELSE, parent, NULL, NULL, node->firstChild);
                curr->firstChild = makeAST(node->firstChild->nextSibling, curr);
                curr->firstChild->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling, curr);
        }
    }

    if(node->symbol == ioStmt) {
        switch(node->firstChild->symbol) {
            //<ioStmt> ==> TK_READ TK_OP <var> TK_CL TK_SEM 
            case TK_READ:
                curr = mkNode(IOREAD, parent, NULL, NULL, node->firstChild);
                curr->firstChild = makeAST(node->firstChild->nextSibling->nextSibling, curr);
                break;
            //<ioStmt> ==> TK_WRITE TK_OP <var> TK_CL TK_SEM
            case TK_WRITE:
                curr = mkNode(IOWRITE, parent, NULL, NULL, node->firstChild);
                curr->firstChild = makeAST(node->firstChild->nextSibling->nextSibling, curr);
                break;
        }
    }

    //<arithmeticExpression> ==> <term> <expPrime>
    if(node->symbol == arithmeticExpression) {
        curr = makeAST(node->firstChild, parent);
        temp1 = makeAST(node->firstChild->nextSibling, NULL);
        if(temp1) {
            ast* temp2 = temp1->firstChild;
            temp1->firstChild = curr;
            temp1->firstChild->parent = temp1;
            temp1->firstChild->nextSibling = temp2;
            temp1->firstChild->nextSibling->parent = temp1;
            while(temp1->parent != NULL) {
               // printf("\nParent NodeType: %s, Current Nodetype: %s\n", nodeTypeToStr(temp1->parent->nodeType), nodeTypeToStr(temp1->nodeType));
                temp1 = temp1->parent;
              //  printf("In the loop? 1\n");
            }
            curr = temp1;
          //  printf("Out of the loop 1\n");
        }
    }

    //<term> ==> <factor> <termPrime>
    if(node->symbol == term) {
        curr = makeAST(node->firstChild, parent);
        temp1 = makeAST(node->firstChild->nextSibling, NULL);
        if(temp1) {
            ast* temp2 = temp1->firstChild;
            temp1->firstChild = curr;
            temp1->firstChild->parent = temp1;
            temp1->firstChild->nextSibling = temp2;
            temp1->firstChild->nextSibling->parent = temp1;
            while(temp1->parent) {
                temp1 = temp1->parent;
               // printf("In the loop?\n");
            }
            curr = temp1;
           // printf("Out of the loop\n");
        }
    }

    //<expPrime> ==> <lowPrecedenceOperators> <term> <expPrime>_1 
    if(node->symbol == expPrime) {
        curr = makeAST(node->firstChild, parent);
        curr->firstChild = makeAST(node->firstChild->nextSibling, curr);
        temp1 = makeAST(node->firstChild->nextSibling->nextSibling, parent);
        if(temp1) {
            ast* temp2 = temp1->firstChild;
            temp1->firstChild = curr;
            curr->parent = temp1;
            curr->nextSibling = temp2;
            temp2->parent = temp1;
            curr = temp1->firstChild;
            return curr;
        }
    }

    //<termPrime> ==> <highPrecedenceOperators> <factor> <termPrime>
    if(node->symbol == termPrime) {
       // printf("\nIn termPrime\n");
        curr = makeAST(node->firstChild, parent);
        curr->firstChild = makeAST(node->firstChild->nextSibling, curr);
        temp1 = makeAST(node->firstChild->nextSibling->nextSibling, parent);
        if(temp1) {
            ast* temp2 = temp1->firstChild;
            temp1->firstChild = curr;
            curr->parent = temp1;
            curr->nextSibling = temp2;
            temp2->parent = temp1;
            curr = temp1->firstChild;
         //   printf("End of termPrime\n");
            return curr;
        }
       // printf("End of termPrime temp1 NULL\n");
    } 

    if(node->symbol == factor) {
        switch(node->firstChild->symbol) {
            //<factor> ==> TK_OP <arithmeticExpression> TK_CL
            case TK_OP:
                curr = makeAST(node->firstChild->nextSibling, parent);
                break;
            //<factor> ==> <var>
            case var:
                curr = makeAST(node->firstChild, parent);
                break;
        }
    }

    //<highPrecedenceOperator> ==> TK_MUL | TK_DIV
    if(node->symbol == highPrecedenceOperators) {
        switch(node->firstChild->symbol) {
            case TK_MUL:
                curr = mkNode(MULTIPLY, parent, NULL, NULL, node->firstChild);
                break;
            case TK_DIV:
                curr = mkNode(DIVIDE, parent, NULL, NULL, node->firstChild);
                break;
        }
    }

    //lowPrecedenceOperators ==> TK_PLUS | TK_MINUS
    if(node->symbol == lowPrecedenceOperators) {
        switch(node->firstChild->symbol) {
            case TK_PLUS:
                curr = mkNode(PLUS, parent, NULL, NULL, node->firstChild);
                break;
            case TK_MINUS:
                curr = mkNode(MINUS, parent, NULL, NULL, node->firstChild);
                break;
        }
    }

    if(node->symbol == booleanExpression) {
        switch(node->firstChild->symbol) {
            //<booleanExpression> ==>TK_OP <booleanExpression> TK_CL <logicalOp> TK_OP <booleanExpression> TK_CL
            case TK_OP:
                curr = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling, parent);
                curr->firstChild = makeAST(node->firstChild->nextSibling, curr);
                curr->firstChild->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling, curr);
                break;
            //<booleanExpression> ==> <var> <relationalOp> <var>
            case var:
                curr = makeAST(node->firstChild->nextSibling, parent);
                curr->firstChild = makeAST(node->firstChild, curr);
                curr->firstChild->nextSibling = makeAST(node->firstChild->nextSibling->nextSibling, curr);
                break;
            //<booleanExpression> ==> TK_NOT TK_OP <booleanExpression> TK_CL
            case TK_NOT:
                curr = mkNode(NOT_BOOL, parent, NULL, NULL, node->firstChild);
                curr->firstChild = makeAST(node->firstChild->nextSibling->nextSibling, curr);
        }
    }

    if(node->symbol == var) {
        switch(node->firstChild->symbol) {
            case singleOrRecId:
                curr = makeAST(node->firstChild, parent);
                break;
            case TK_NUM:
                curr = mkNode(NUM, parent, NULL, NULL, node->firstChild);
                break;
            case TK_RNUM:
                curr = mkNode(RNUM, parent, NULL, NULL, node->firstChild);
                break;
        }
    }

    if(node->symbol == logicalOp) {
        switch(node->firstChild->symbol) {
            case TK_AND:
                curr = mkNode(AND, parent, NULL, NULL, node->firstChild);
                break;
            case TK_OR:
                curr = mkNode(OR, parent, NULL, NULL, node->firstChild);
                break;
        }
    }

    if(node->symbol == relationalOp) {
        switch(node->firstChild->symbol) {
            case TK_LT:
                curr = mkNode(LESS_THAN, parent, NULL, NULL, node->firstChild);
                break;
            case TK_LE:
                curr = mkNode(LESS_EQUAL, parent, NULL, NULL, node->firstChild);
                break;
            case TK_EQ:
                curr = mkNode(EQUAL, parent, NULL, NULL, node->firstChild);
                break;
            case TK_GT:
                curr = mkNode(GREATER_THAN, parent, NULL, NULL, node->firstChild);
                break;
            case TK_GE:
                curr = mkNode(GREATER_EQUAL, parent, NULL, NULL, node->firstChild);
                break;
            case TK_NE:
                curr = mkNode(NOT_EQUAL, parent, NULL, NULL, node->firstChild);
                break;
        }
    }

    //<returnStmt> ==>TK_RETURN <optionalReturn> TK_SEM
    if(node->symbol == returnStmt) {
       // printf("Entered returnStmt\n");
        curr = mkNode(RETURN, parent, NULL, NULL, node->firstChild);
        curr->firstChild = makeAST(node->firstChild->nextSibling, curr);
      //  printf("Exited returnStmt\n");
    }

    //<optionalReturn> ==> TK_SQL <idList> TK_SQR 
    if(node->symbol == optionalReturn) {
        curr = makeAST(node->firstChild->nextSibling, parent);
    }

    //idList ==> TK_ID more_ids
    if(node->symbol == idList) {
        curr = mkNode(ID, parent, NULL, NULL, node->firstChild);
        curr->nextSibling = makeAST(node->firstChild->nextSibling, parent);
    }

    //<more_ids> ==> TK_COMMA <idList>
    if(node->symbol == more_ids) {
        curr = makeAST(node->firstChild->nextSibling, parent);
    }

    if(curr) {
        curr->parent = parent;
        /*char* valueStr, *lex1, *parentStr, *isLeaf, *nodeSymbol;
        if(curr->nodeType == NUM || curr->nodeType == RNUM)
            valueStr = curr->lex;
        else
            valueStr = "Not a number ";
        if(curr->symbol >= eps)
            lex1 = curr->lex;
        else
            lex1 = "------";
        if(curr->parent!=NULL)
            parentStr = nodeTypeToStr((curr->parent)->nodeType);
        else
            parentStr = "ROOT ";
        if(curr->firstChild!=NULL)
            {isLeaf = "NO "; nodeSymbol = nodeTypeToStr(curr->nodeType);}
        else 
            {isLeaf = "YES "; nodeSymbol = "Leaf node ";}
        if(curr->symbol == eps)
            lex1 = "------";
        printf("\n%25s %15d %30s %20s %25s %10s %25s", lex1, curr->line, nodeTypeToStr(curr->nodeType), valueStr, parentStr, isLeaf, nodeSymbol);*/
        //fprintf(fp, "\n%25s %15d %30s %20s %25s %10s %25s", lex1, curr->line, nodeTypeToStr(curr->nodeType), valueStr, parentStr, isLeaf, nodeSymbol);
    }
    return curr;
}

void inorderAST(ast* curr, int *numNodes, FILE *fp)
{
    if (curr == NULL)
        return;
    ast* temp = (ast*)malloc(sizeof(ast));
    temp = curr->firstChild;
    if (temp != NULL)
    {
        while (temp->nextSibling)
        {
            inorderAST(temp, numNodes, fp);
            temp = temp->nextSibling;
        }
    }
    char* valueStr, *lex1, *parentStr, *isLeaf, *nodeSymbol;
    if(curr->nodeType == NUM || curr->nodeType == RNUM)
        valueStr = curr->lex;
    else
        valueStr = "Not a number ";
    if(curr->symbol >= eps)
        lex1 = curr->lex;
    else
        lex1 = "------";
    if(curr->parent!=NULL)
        parentStr = nodeTypeToStr((curr->parent)->nodeType);
    else
        parentStr = "ROOT ";
    if(curr->firstChild!=NULL)
        {isLeaf = "NO "; nodeSymbol = nodeTypeToStr(curr->nodeType);}
    else 
        {isLeaf = "YES "; nodeSymbol = "Leaf node ";}
    if(curr->symbol == eps)
        lex1 = "------";
    printf("\n%25s %15d %30s %20s %25s %10s %25s", lex1, curr->line, nodeTypeToStr(curr->nodeType), valueStr, parentStr, isLeaf, nodeSymbol);
    fprintf(fp, "\n%25s %15d %30s %20s %25s %10s %25s", lex1, curr->line, nodeTypeToStr(curr->nodeType), valueStr, parentStr, isLeaf, nodeSymbol);
    *numNodes = *numNodes + 1;
    inorderAST(temp, numNodes, fp);
}

int printAST(ast* root, char* outfile) {
    FILE* fp2 = fopen(outfile, "w");
    int numNodes = 0;
    inorderAST(root, &numNodes, fp2);
    fclose(fp2);
    return numNodes;
}

ast* initAST(parseTree root) {
    return makeAST(root, NULL);
}