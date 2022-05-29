stage1exe:
	gcc driver.c parser.c lexer.c ll1Grammar.c predParseTable.c stack.c lookupTable.c ast.c symbolTable.c typeChecker.c -o stage2exe
