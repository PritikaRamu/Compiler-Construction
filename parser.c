/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/

#include "parser.h"
#define offset eps

char *enumToStr(g_Term id);
char *getTokenStr(int token);
char *getNontermStr(int token);

parseTree initTree(g_Term symbol, int lineNo)
{
    parseTree ptree = (parseTree)malloc(sizeof(struct tree));
    ptree->symbol = symbol;
    ptree->isTerminal = isTerm(symbol);
    ptree->lineNo = lineNo;
    ptree->firstChild = NULL;
    ptree->nextSibling = NULL;
    ptree->parent = NULL;
    ptree->lex = NULL;
    return ptree;
}

parseTree getNearestUncle(parseTree curr)
{
    if (!curr)
        return NULL;
    //reached root of tree
    if (!curr->parent)
    {
        return NULL;
    }
    if ((curr->parent)->nextSibling)
    {
        return (curr->parent)->nextSibling;
    }
    else
        return getNearestUncle(curr->parent);
}

void pushRuleIntoStack(g_RHS *rule1, Stack *stack)
{
    Stack *temp = initStack();
    g_RHS *rule = rule1;
    while (rule)
    {
        push(temp, rule->symbol);
        rule = rule->next;
    }
    while (top(temp) != -1)
    {
        g_Term topStack = top(temp);
        pop(temp);
        push(stack, topStack);
    }
}

parseTree addChildren(g_RHS *rule1, parseTree ptree)
{
    g_RHS *rule = rule1;
    parseTree curr;
    if (rule != NULL)
    {
        parseTree fChild = (parseTree)malloc(sizeof(struct tree));
        fChild->symbol = rule->symbol;
        fChild->isTerminal = rule->isTerminal;
        fChild->firstChild = NULL;
        fChild->nextSibling = NULL;
        fChild->parent = ptree;
        fChild->lex = NULL;
        ptree->firstChild = fChild;
        curr = fChild;
        rule = rule->next;
    }
    while (rule)
    {
        parseTree temp = (parseTree)malloc(sizeof(struct tree));
        temp->symbol = rule->symbol;
        temp->isTerminal = rule->isTerminal;
        temp->firstChild = NULL;
        temp->nextSibling = NULL;
        temp->parent = ptree;
        temp->lex = NULL;
        curr->nextSibling = temp;
        curr = temp;
        rule = rule->next;
    }
    return ptree->firstChild;
}

parseTree parseInputSourceCode(char *testcaseFile)
{
    FILE *fp = startLexer(testcaseFile);
    initTable();

    //init stack, push SENTINEL, push start symbol
    Stack *stack = initStack();
    push(stack, SENTINEL);
    push(stack, program);
    parseTree ptree = initTree(program, 1);
    parseTree curr = ptree;
    tokenInfo look = getNextToken();
    g_RHS *rule;
    while (look.tid == TK_COMMENT || look.tid == LEX_ERROR)
    {
        look = getNextToken(); //ignore the comments and lexical errors
    }
    if (look.tid == SENTINEL)
    {
        printf("File empty\n");
        return ptree;
    }
    while (1)
    {
        
        g_Term topStack = top(stack);
        pop(stack);
        while (topStack >= eps)
        {
            if (topStack == eps)
            {
                int counter = 0;
                topStack = top(stack);
                pop(stack);
                curr->lineNo = look.lineNo;
                curr->lex = look.lexeme;
                if (curr->nextSibling)
                {
                    curr = curr->nextSibling;
                }
                else if (getNearestUncle(curr))
                {
                    curr = getNearestUncle(curr);
                }
                continue; //brings us back to while loop for terminals
            }

            if (topStack != look.tid)
            {
                //input is consumed, stack not empty
                if (look.tid == SENTINEL)
                {
                    //printf("Input consumed but stack not empty\n");
                    //printToken(topStack);
                    //printf(" at line %d \n", look.lineNo);
                    printf("Syntax error - Input consumed but stack not empty\n");
                    return ptree;
                }
                else
                {
                            printf("Line %d Error: The token %s for lexeme %s does not match with expected token %s\n",look.lineNo, enumToStr(look.tid), look.lexeme, enumToStr(topStack));
                }
                    look = getNextToken();
                    //printf("\n %d", look.tid == SENTINEL);

                    while (look.tid == TK_COMMENT || look.tid == LEX_ERROR)
                    {
                        look = getNextToken();
                        //printf("Checking for lexeme %s \n", look.lexeme);
                    }
                    if (look.tid == SENTINEL)
                    {
                        return ptree;
                    }
                    continue;
                    
            }
        

            //the top of the stack is a terminal and matches the input
            //pop the stack and move the pointer ahead
            else
            {
                //code is consumed along with the stack

                topStack = top(stack);
                pop(stack);
                //printf("Top of the stack: ");
                // if (topStack >= eps)
                //     printToken(topStack);
                // else
                //     printNonTerminal(topStack);
                curr->lineNo = look.lineNo;
                curr->lex = look.lexeme;
                look = getNextToken();
                while (look.tid == TK_COMMENT || look.tid == LEX_ERROR)
                    look = getNextToken();
                //printf("Look.tid is ");
                // printToken(look.tid);
                // printf("\n");
                if (topStack == SENTINEL || look.tid == SENTINEL)
                {
                    //printf("Input consumed and stack clear\n");
                    fclose(fp);
                    return ptree;
                }
                if (curr->nextSibling)
                {
                    curr = curr->nextSibling;
                }
                else if (getNearestUncle(curr))
                {
                    curr = getNearestUncle(curr);
                }
                else
                    continue;
            }            }
        
        rule = parseTable[topStack][look.tid - TK_ASSIGNOP];
        //printRule(rule);
        //error handling, rule
        if (!rule)
        {

            printf("Line %d Error: Invalid token %s encountered with value %s stack top %s\n",look.lineNo,enumToStr(look.tid),look.lexeme,enumToStr(topStack));
            while (1)
            {
                // printToken(look.tid);
                // printf("\n");
                look = getNextToken();
                while (look.tid == TK_COMMENT || look.tid == LEX_ERROR)
                    look = getNextToken();
                if (look.tid == SENTINEL)
                {
                    //printf("Input consumed - ERROR\n");
                    return ptree;
                }
                if (First[topStack][look.tid - eps] == true)
                {
                    rule = parseTable[topStack][look.tid - TK_ASSIGNOP];
                    // pushRuleIntoStack(rule, stack);
                    // look = getNextToken();

                    while (look.tid == TK_COMMENT || look.tid == LEX_ERROR)
                        look = getNextToken();
                    if (look.tid == SENTINEL)
                    {
                        //printf("Input consumed - ERROR\n");
                        return ptree;
                    }
                    // printRule(rule);
                    // printf("\nPushing into Stack\n");
                    pushRuleIntoStack(rule, stack);
                    //printf("Look.tid is ");
                    // if (look.tid != LEX_ERROR)
                    //     printToken(look.tid);
                    // else
                    //     printf("Lex Error");
                    // printf("\n");
                    curr = addChildren(rule, curr);
                    break;
                }

                else if (Follow[topStack][look.tid - eps] == true)
                {
                    
                    break;
                }

                else
                {
                    
                    while (look.tid == TK_COMMENT || look.tid == LEX_ERROR)
                        look = getNextToken();
                    if (look.tid == SENTINEL)
                    {
                        //printf("Syntax error");
                        return ptree;
                    }
                }
            }

        }

        else
        {
            // printRule(rule);
            // printf("\nPushing into Stack\n");
            pushRuleIntoStack(rule, stack);
            curr = addChildren(rule, curr);
            //printf("check2");
        }
    }
    return ptree;
}

void inorderNary(parseTree curr, int *numNodes, FILE *fp)
{
    if (curr == NULL)
        return;
    parseTree temp = (parseTree)malloc(sizeof(struct tree));
    temp = curr->firstChild;
    if (temp != NULL)
    {
        while (temp->nextSibling)
        {
            inorderNary(temp, numNodes, fp);
            temp = temp->nextSibling;
        }
    }
    char* valueStr, *lex1, *parentStr, *isLeaf, *nodeSymbol;
    if(curr->symbol == TK_NUM || curr->symbol ==TK_RNUM)
        valueStr = curr->lex;
    else
        valueStr = "Not a number ";
    if(curr->symbol >=eps)
        lex1 = curr->lex;
    else
        lex1 = "------";
    if(curr->parent!=NULL)
        parentStr = enumToStr((curr->parent)->symbol);
    else
        parentStr = "ROOT ";
    if(curr->firstChild!=NULL)
        {isLeaf = "NO "; nodeSymbol = enumToStr(curr->symbol);}
    else 
        {isLeaf = "YES "; nodeSymbol = "Leaf node ";}
    if(curr->symbol ==eps)
        lex1 = "------";
    printf("\n%25s %15d %30s %20s %25s %10s %25s", lex1, curr->lineNo, enumToStr(curr->symbol), valueStr, parentStr, isLeaf, nodeSymbol);
    fprintf(fp, "\n%25s %15d %30s %20s %25s %10s %25s", lex1, curr->lineNo, enumToStr(curr->symbol), valueStr, parentStr, isLeaf, nodeSymbol);
    *numNodes = *numNodes + 1;
    inorderNary(temp, numNodes, fp);
}

int printParseTree(parseTree PT, char* outfile)
{
    FILE *fp1 = fopen(outfile, "w");
    int numNodes = 0;
    inorderNary(PT, &numNodes, fp1);
    fclose(fp1);
    return numNodes;
}

char *getTokenStr(int token)
{
    switch (token)
    {
    case eps:
        return "eps";
        break;
    case TK_ASSIGNOP:
        return "TK_ASSIGNOP";
        break;
    case TK_COMMENT:
        return "TK_COMMENT";
        break;
    case TK_FIELDID:
        return "TK_FIELDID";
        break;
    case TK_ID:
        return "TK_ID";
        break;
    case TK_NUM:
        return "TK_NUM";
        break;
    case TK_RNUM:
        return "TK_RNUM";
        break;
    case TK_FUNID:
        return "TK_FUNID";
        break;
    case TK_WITH:
        return "TK_WITH";
        break;
    case TK_PARAMETERS:
        return "TK_PARAMETERS";
        break;
    case TK_END:
        return "TK_END";
        break;
    case TK_WHILE:
        return "TK_WHILE";
        break;
    case TK_TYPE:
        return "TK_TYPE";
        break;
    case TK_MAIN:
        return "TK_MAIN";
        break;
    case TK_GLOBAL:
        return "TK_GLOBAL";
        break;
    case TK_PARAMETER:
        return "TK_PARAMETER";
        break;
    case TK_LIST:
        return "TK_LIST";
        break;
    case TK_SQL:
        return "TK_SQL";
        break;
    case TK_SQR:
        return "TK_SQR";
        break;
    case TK_INPUT:
        return "TK_INPUT";
        break;
    case TK_OUTPUT:
        return "TK_OUTPUT";
        break;
    case TK_INT:
        return "TK_INT";
        break;
    case TK_REAL:
        return "TK_REAL";
        break;
    case TK_COMMA:
        return "TK_COMMA";
        break;
    case TK_SEM:
        return "TK_SEM";
        break;
    case TK_COLON:
        return "TK_COLON";
        break;
    case TK_DOT:
        return "TK_DOT";
        break;
    case TK_ENDWHILE:
        return "TK_ENDWHILE";
        break;
    case TK_OP:
        return "TK_OP";
        break;
    case TK_CL:
        return "TK_CL";
        break;
    case TK_IF:
        return "TK_IF";
        break;
    case TK_THEN:
        return "TK_THEN";
        break;
    case TK_ENDIF:
        return "TK_ENDIF";
        break;
    case TK_READ:
        return "TK_READ";
        break;
    case TK_WRITE:
        return "TK_WRITE";
        break;
    case TK_RETURN:
        return "TK_RETURN";
        break;
    case TK_PLUS:
        return "TK_PLUS";
        break;
    case TK_MINUS:
        return "TK_MINUS";
        break;
    case TK_MUL:
        return "TK_MUL";
        break;
    case TK_DIV:
        return "TK_DIV";
        break;
    case TK_CALL:
        return "TK_CALL";
        break;
    case TK_RECORD:
        return "TK_RECORD";
        break;
    case TK_ENDRECORD:
        return "TK_ENDRECORD";
        break;
    case TK_ELSE:
        return "TK_ELSE";
        break;
    case TK_AND:
        return "TK_AND";
        break;
    case TK_OR:
        return "TK_OR";
        break;
    case TK_NOT:
        return "TK_NOT";
        break;
    case TK_LT:
        return "TK_LT";
        break;
    case TK_LE:
        return "TK_LE";
        break;
    case TK_AS:
        return "TK_AS";
        break;
    case TK_EQ:
        return "TK_EQ";
        break;
    case TK_GT:
        return "TK_GT";
        break;
    case TK_GE:
        return "TK_GE";
        break;
    case TK_NE:
        return "TK_NE";
        break;
    case SENTINEL:
        return "SENTINEL";
        break;
    case TK_RUID:
        return "TK_RUID";
        break;
    case TK_UNION:
        return "TK_UNION";
        break;
    case TK_ENDUNION:
        return "TK_ENDUNION";
        break;
    case TK_DEFINETYPE:
        return "TK_DEFINETYPE";
        break;
    default:
        return "No Symbol found";
        break;
    }
}

char *getNontermStr(int token)
{
    switch (token)
    {
    case 0:
        return "program";
        break;
    case 1:
        return "mainFunction";
        break;
    case 2:
        return "otherFunctions";
        break;
    case 3:
        return "function";
        break;
    case 4:
        return "input_par";
        break;
    case 5:
        return "output_par";
        break;
    case 6:
        return "parameter_list";
        break;
    case 7:
        return "dataType";
        break;
    case 8:
        return "primitiveDatatype";
        break;
    case 9:
        return "constructedDatatype";
        break;
    case 10:
        return "remaining_list";
        break;
    case 11:
        return "stmts";
        break;
    case 12:
        return "typeDefinitions";
        break;
    case 13:
        return "typeDefinition";
        break;
    case 14:
        return "fieldDefinitions";
        break;
    case 15:
        return "fieldDefinition";
        break;
    case 16:
        return "moreFields";
        break;
    case 17:
        return "declarations";
        break;
    case 18:
        return "declaration";
        break;
    case 19:
        return "global_or_not";
        break;
    case 20:
        return "otherStmts";
        break;
    case 21:
        return "stmt";
        break;
    case 22:
        return "assignmentStmt";
        break;
    case 23:
        return "singleOrRecId";
        break;
    case 24:
        return "actualOrRedefined"; // Changed to Support Grammar
        break;
    case 25:
        return "funCallStmt";
        break;
    case 26:
        return "outputParameters";
        break;
    case 27:
        return "inputParameters";
        break;
    case 28:
        return "iterativeStmt";
        break;
    case 29:
        return "conditionalStmt";
        break;
    case 30:
        return "elsePart";
        break;
    case 31:
        return "ioStmt";
        break;
    case 32:
        return "fieldtype"; // Changed to Support Grammar
        break;
    case 33:
        return "option_single_constructed"; // Changed to Support Grammar
        break;
    case 34:
        return "arithmeticExpression";
        break;
    case 35:
        return "expPrime";
        break;
    case 36:
        return "term";
        break;
    case 37:
        return "termPrime";
        break;
    case 38:
        return "factor";
        break;
    case 39:
        return "highPrecedenceOperators";
        break;
    case 40:
        return "lowPrecedenceOperators";
        break;
    case 41:
        return "oneExpansion"; // Changed to Support Grammar
        break;
    case 42:
        return "moreExpansions"; // Changed to Support Grammar
        break;
    case 43:
        return "booleanExpression";
        break;
    case 44:
        return "var";
        break;
    case 45:
        return "logicalOp";
        break;
    case 46:
        return "relationalOp";
        break;
    case 47:
        return "returnStmt";
        break;
    case 48:
        return "optionalReturn";
        break;
    case 49:
        return "idList";
        break;
    case 50:
        return "more_ids";
        break;
    case definetypestmt:
        return "definetypestmt";
        break;
    case A:
        return "A";
        break;

    default:
        return "No Symbol found";
        break;
    }
}

char *enumToStr(g_Term id)
{
    if (id >= eps)
    {
        return getTokenStr(id);
    }
    else
    {
        return getNontermStr(id);
    }
}

// FOR TESTING PURPOSES

// int main()
// {
//     char *testFile = "p3.txt";
//     char *outfile = "outp2.txt";
//     initGrammar(G);
//     populateFirstFollow("First.txt", true);
//     populateFirstFollow("Follow.txt", false);
//     computeParseTable();
//     parseTree ptree = parseInputSourceCode(testFile);
//     printf("\n\n\nCheck");
//     printf("Root node %d\n", ptree->symbol);
//     int numNodes = printParseTree(ptree, "outp2.txt");
//     //inorderNary(ptree, &numNodes,fp);
// }