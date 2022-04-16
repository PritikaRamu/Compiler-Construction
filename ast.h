/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/
#ifndef _AST_
#define _AST_

#include "astDef.h"

int printAST(ast* root, char* outfile);
ast* initAST(parseTree root);
ast* makeAST(parseTree node, ast* parent);
ast* mkNode(NodeType nodeType, ast* parent, ast* firstChild, ast* nextSibling, parseTree ptNode);

#endif