#ifndef _ASTDEF_
#define __ASTDEF_

#include "parser.h"

typedef enum {
    PROGRAM,
    MAIN_FUNCTION, 
    FUNCTION_SEQ, 
    INPUT_PARAMETERS,
    OUTPUT_PARAMETERS,
    INTEGER,
    REAL,
    RECORD,
    UNION,
    ID,
    FIELDID,
    TYPE_DEFINITIONS,
    DECLARATIONS,
    OTHERSTMTS,
}NodeType;

typedef struct astNode {
    NodeType nodeType;

    struct ast* parent;

    struct ast* firstChild;

    struct ast* nextSibling;
    
    g_Term symbol;

    char* lex;
    
    int line;
}ast;