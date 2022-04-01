stage1exe:
	gcc driver.c parser.c lexer.c ll1Grammar.c predParseTable.c stack.c symbolTable.c -o stage1exe
