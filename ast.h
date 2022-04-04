#ifndef _AST_
#define _AST_

#include "astDef.h"

int printAST(ast* root, char* outfile);
ast* initAST(parseTree root);
ast* makeAST(parseTree node, ast* parent);
ast* mkNode(NodeType nodeType, ast* parent, ast* firstChild, ast* nextSibling, parseTree ptNode);

#endif