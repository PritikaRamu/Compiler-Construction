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
    ASSIGNOP
}NodeType;

typedef struct astNode {
    NodeType nodeType;

    struct astNode* parent;

    struct astNode* firstChild;

    struct astNode* nextSibling;
    
    g_Term symbol;

    char* lex;
    
    int line;
}ast;

#endif