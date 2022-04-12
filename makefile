stage1exe:
	gcc driver_ast.c parser.c lexer.c ll1Grammar.c predParseTable.c stack.c lookupTable.c ast.c symbolTabel.c -o stage1exe
