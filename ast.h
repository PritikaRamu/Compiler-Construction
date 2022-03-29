#ifndef _AST_
#define _AST_

#include "astDef.h"

ast* initASt(parseTree root);
ast* makeAST(parseTree node, ast* parent);