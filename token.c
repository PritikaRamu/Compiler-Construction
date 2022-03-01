#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>

void printNonTerminal(int token)
{
	switch (token)
	{
	case 0:
		printf("<program> ");
		break;
	case 1:
		printf("<mainFunction> ");
		break;
	case 2:
		printf("<otherFunctions> ");
		break;
	case 3:
		printf("<function> ");
		break;
	case 4:
		printf("<input_par> ");
		break;
	case 5:
		printf("<output_par> ");
		break;
	case 6:
		printf("<parameter_list> ");
		break;
	case 7:
		printf("<dataType> ");
		break;
	case 8:
		printf("<primitiveDatatype> ");
		break;
	case 9:
		printf("<constructedDatatype> ");
		break;
	case 10:
		printf("<remaining_list> ");
		break;
	case 11:
		printf("<stmts> ");
		break;
	case 12:
		printf("<typeDefinitions> ");
		break;
	case 13:
		printf("<typeDefinition> ");
		break;
	case 14:
		printf("<fieldDefinitions> ");
		break;
	case 15:
		printf("<fieldDefinition> ");
		break;
	case 16:
		printf("<moreFields> ");
		break;
	case 17:
		printf("<declarations> ");
		break;
	case 18:
		printf("<declaration> ");
		break;
	case 19:
		printf("<global_or_not> ");
		break;
	case 20:
		printf("<otherStmts> ");
		break;
	case 21:
		printf("<stmt> ");
		break;
	case 22:
		printf("<assignmentStmt> ");
		break;
	case 23:
		printf("<singleOrRecId> ");
		break;
	case 24:
		printf("<actualOrRedefined> ");
		break;
	case 25:
		printf("<funCallStmt> ");
		break;
	case 26:
		printf("<outputParameters> ");
		break;
	case 27:
		printf("<inputParameters> ");
		break;
	case 28:
		printf("<iterativeStmt> ");
		break;
	case 29:
		printf("<conditionalStmt> ");
		break;
	case 30:
		printf("<elsePart> ");
		break;
	case 31:
		printf("<ioStmt> ");
		break;
	case 32:
		printf("<fieldtype> ");
		break;
	case 33:
		printf("<option_single_constructed> ");
		break;
	case 34:
		printf("<arithmeticExpression> ");
		break;
	case 35:
		printf("<expPrime> ");
		break;
	case 36:
		printf("<term> ");
		break;
	case 37:
		printf("<termPrime> ");
		break;
	case 38:
		printf("<factor> ");
		break;
	case 39:
		printf("<highPrecedenceOperators> ");
		break;
	case 40:
		printf("<lowPrecedenceOperators> ");
		break;
	case 41:
		printf("<oneExpansion> ");
		break;
	case 42:
		printf("<moreExpansions> ");
		break;
	case 43:
		printf("<booleanExpression> ");
		break;
	case 44:
		printf("<var> ");
		break;
	case 45:
		printf("<logicalOp> ");
		break;
	case 46:
		printf("<relationalOp> ");
		break;
	case 47:
		printf("<returnStmt> ");
		break;
	case 48:
		printf("<optionalReturn> ");
		break;
	case 49:
		printf("<idList> ");
		break;
	case 50:
		printf("<more_ids> ");
		break;
	case 51:
		printf("<eps> ");
		break;
	case 111:
		printf("<definetypestmt> ");
		break;
	case 112:
		printf("<A> ");
		break;
	}
}

void printToken(int token)
{
	//gterm base=eps-1;
	//token -= base;
	switch (token)
	{
	case 52:
		printf("TK_ASSIGNOP ");
		break;
	case 53:
		printf("TK_COMMENT ");
		break;
	case 54:
		printf("TK_FIELDID ");
		break;
	case 55:
		printf("TK_ID ");
		break;
	case 56:
		printf("TK_NUM ");
		break;
	case 57:
		printf("TK_RNUM ");
		break;
	case 58:
		printf("TK_FUNID ");
		break;
	case 59:
		printf("TK_WITH ");
		break;
	case 60:
		printf("TK_PARAMETERS ");
		break;
	case 61:
		printf("TK_END ");
		break;
	case 62:
		printf("TK_WHILE ");
		break;
	case 63:
		printf("TK_TYPE ");
		break;
	case 64:
		printf("TK_MAIN ");
		break;
	case 65:
		printf("TK_GLOBAL ");
		break;
	case 66:
		printf("TK_PARAMETER ");
		break;
	case 67:
		printf("TK_LIST ");
		break;
	case 68:
		printf("TK_SQL ");
		break;
	case 69:
		printf("TK_SQR ");
		break;
	case 70:
		printf("TK_INPUT ");
		break;
	case 71:
		printf("TK_OUTPUT");
		break;
	case 72:
		printf("TK_INT ");
		break;
	case 73:
		printf("TK_REAL ");
		break;
	case 74:
		printf("TK_COMMA ");
		break;
	case 75:
		printf("TK_SEM ");
		break;
	case 76:
		printf("TK_COLON ");
		break;
	case 77:
		printf("TK_DOT ");
		break;
	case 78:
		printf("TK_ENDWHILE ");
		break;
	case 79:
		printf("TK_OP ");
		break;
	case 80:
		printf("TK_CL ");
		break;
	case 81:
		printf("TK_IF ");
		break;
	case 82:
		printf("TK_THEN ");
		break;
	case 83:
		printf("TK_ENDIF ");
		break;
	case 84:
		printf("TK_READ ");
		break;
	case 85:
		printf("TK_WRITE ");
		break;
	case 86:
		printf("TK_RETURN ");
		break;
	case 87:
		printf("TK_PLUS ");
		break;
	case 88:
		printf("TK_MINUS ");
		break;
	case 89:
		printf("TK_MUL ");
		break;
	case 90:
		printf("TK_DIV ");
		break;
	case 91:
		printf("TK_CALL ");
		break;
	case 92:
		printf("TK_RECORD ");
		break;
	case 93:
		printf("TK_ENDRECORD ");
		break;
	case 94:
		printf("TK_ELSE ");
		break;
	case 95:
		printf("TK_AND ");
		break;
	case 96:
		printf("TK_OR ");
		break;
	case 97:
		printf("TK_NOT ");
		break;
	case 98:
		printf("TK_LT ");
		break;
	case 99:
		printf("TK_LE ");
		break;
	case 100:
		printf("TK_AS ");
		break;
	case 101:
		printf("TK_EQ ");
		break;
	case 102:
		printf("TK_GT ");
		break;
	case 103:
		printf("TK_GE ");
		break;
	case 104:
		printf("TK_NE ");
		break;
	case 105:
		printf("SENTINEL ");
		break;
	case 106:
		printf("LEX_ERROR ");
		break;
	case 107:
		printf("TK_RUID ");
		break;
	case 108:
		printf("TK_UNION");
		break;
	case 109:
		printf("TK_ENDUNION ");
		break;
	case 110:
		printf("TK_DEFINETYPE");
		break;
	case 113:
		printf("$ ");
		break;
	}
}