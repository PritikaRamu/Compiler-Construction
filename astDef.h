/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/
#ifndef _ASTDEF_
#define _ASTDEF_

#include "parser.h"

typedef enum {
    PROGRAM,
    MAIN_FUNCTION, 
    FUNCTION_SEQ, 
    INPUT_PARAMETERS,
    OUTPUT_PARAMETERS,
    INTEGER,
    REAL,
    RECORD_OR_UNION,
    ID,
    FIELDID,
    TYPE_DEFINITIONS,
    DEFINETYPE,
    DECLARATIONS,
    OTHERSTMTS,
    GLOBAL, 
    ASSIGNOP,
    CALL, 
    FUNCTION_CALL,
    OUTPUT_PARAMETERS_CALL,
    INPUT_PARAMETERS_CALL,
    ITERATIVE,
    CONDITIONAL,
    ELSE,
    IOREAD, 
    IOWRITE,
    MULTIPLY,
    DIVIDE,
    PLUS, 
    MINUS,
    NOT_BOOL,
    NUM, 
    RNUM,
    AND,
    OR,
    LESS_THAN,
    LESS_EQUAL,
    EQUAL,
    GREATER_THAN,
    GREATER_EQUAL,
    NOT_EQUAL,
    RETURN
}NodeType;  

typedef struct astNode {
    NodeType nodeType;

    struct astNode* parent;

    struct astNode* firstChild;

    struct astNode* nextSibling;
    
    g_Term symbol;

    char* lex;
    
    int line;

    bool is_union;
}ast;

#endif