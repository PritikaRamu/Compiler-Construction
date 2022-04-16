/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/

#ifndef _DRIVERDEF_
#define _DRIVERDEF_
#include "typeChecker.h"

int main(int argc, char *argv[])
{

	if (argc < 3)
	{
		printf("Wrong command. Type: ./stage2exe testcase.txt outputFile.txt\n");
		return 0;
	}

	printf("GROUP 15:\nAadit Deshpande\nNandan Bharatkumar Parikh\nPreetika Verma\nPritika Ramu\nSneha\n");
	printf("\n\nIMPLEMENTATION DETAILS OF STAGE 1\n");
	printf("1. Initialized grammar\n");
	printf("2. Automated computation of FIRST\n");
	printf("3. Implemented lexical analyser\n");
	printf("4. Implemented syntax analyser\n");
	printf("5. Implemented PANIC MODE error recovery\n");

	bool flag = true;

	while (flag)
	{

		printf("\n\nEnter number to perform the function:\n");
		printf("0: For exit\n");
		printf("1: For removal of comments (on the console)\n");
		printf("2: For printing the token list (on the console)\n");
		printf("3: For parsing to verify the syntactic correctness of the input source code and printing the parse tree\n");
		printf("4: For printing the total time taken (on the console)\n");
		printf("5: For printing the AST on the console\n");
		printf("6: For printing the symbol table on the console\n");
		printf("7: For Semantic Error detection\n");

		printf("\n\nEnter your choice: ");

		int choice;
		scanf("%d", &choice);

		switch (choice)
		{

		case 0: // Exit (done)
		{
			printf("THANK YOU!\n");
			flag = false;
			break;
		}

		case 1: // Print Token List (done)
		{
			removeComments(argv[1], argv[2]);

			line = 1;
			FILE *fp = startLexer(argv[1]);
			initTable();
			while (1)
			{
				tokenInfo pleasework = getNextToken();
				if (pleasework.tid == SENTINEL)
					break;
				if (pleasework.tid != LEX_ERROR)
				{
					if (pleasework.tid == TK_COMMENT)
						printf("Line no. %d\tLexeme \t\t\t%%\t\tTK_COMMENT", pleasework.lineNo);
					else
						printf("Line no. %d\tLexeme %20s", pleasework.lineNo, pleasework.lexeme);
				}
				printf("\t\t");
				if (pleasework.tid != TK_COMMENT)
					printToken(pleasework.tid);
				printf("\n");
			}
			fclose(fp);
			break;
		}

		case 2: // Print Parse Tree (done)
		{
			line = 1;
			// invoke your lexer and parser here
			char *testFile = argv[1];
			char *outfile = argv[2];
			initGrammar(G);
			populateFirstFollow("First.txt", true);
			populateFirstFollow("Follow.txt", false);
			computeParseTable();
			parseTree ptree = parseInputSourceCode(testFile);
			int numNodes = printParseTree(ptree, outfile);

			break;
			
		}

		case 3: // Print AST (done)
		{
			// AST
			line = 1;
			char *testFile = argv[1];
			char *outfile = argv[2];
			initGrammar(G);
			populateFirstFollow("First.txt", true);
			populateFirstFollow("Follow.txt", false);
			computeParseTable();
			parseTree ptree = parseInputSourceCode(testFile);
			ast *astree = initAST(ptree);
			
			printf("\nTraversing AST in in-order traversal\n");
			int numNodes = printAST(astree, outfile);
			printf("\nNumber of Nodes %d\n",numNodes);
			break;
		}
		case 4: // Memory Allocated to Parse Tree/AST Nodes
		{	
			char *testFile = argv[1];
			char *outfile = argv[2];
			initGrammar(G);
			populateFirstFollow("First.txt", true);
			populateFirstFollow("Follow.txt", false);
			computeParseTable();
			parseTree ptree = parseInputSourceCode(testFile);
			int numNodes1 = printParseTree(ptree, outfile);

			ast *astree = initAST(ptree);
			int numNodes = printAST(astree, outfile);


			break;
		}
		case 5: // Print Symbol Table (done)
		{

			line = 1;
			char *testFile = argv[1];
			char *outfile = argv[2];
			initGrammar(G);
			populateFirstFollow("First.txt", true);
			populateFirstFollow("Follow.txt", false);
			computeParseTable();
			parseTree ptree = parseInputSourceCode(testFile);
			ast *astree = initAST(ptree);
			
			initializeSymbolTable(astree);
			printFinalTable(SymbolTable->FunctionTable);
			
			break;
		}
		case 6: // Print Global Variables
		{

			
			break;
			
		}
		case 7: // Activation Record Sizes Odone)
		{
			printFunctionTable(SymbolTable->FunctionTable);
			break;
		}

		case 8: // Record Types and Sizes
		{
			printRecordTable(SymbolTable->RecordUnionTable);
			break;
		}

		case 9: // Type Checking and Semantic Analysis (done)
		{

			line = 1;
			char *testFile = argv[1];
			char *outfile = argv[2];
			initGrammar(G);
			populateFirstFollow("First.txt", true);
			populateFirstFollow("Follow.txt", false);
			computeParseTable();
			parseTree ptree = parseInputSourceCode(testFile);
			ast *astree = initAST(ptree);
			free(ptree);
			
			initializeSymbolTable(astree);
			printf("Starting Semantic Analysis\n");
			semanticAnalyser(astree);

    		//print_Redeclarations();

			printf("\nEnd of Semantic Analysis.\n");
			break;
		}

		case 10: // Code Generation (done)
		{
			printf("Not Implemented\n");
			break;
		}

		
		}
	}

	return 0;
}

#endif