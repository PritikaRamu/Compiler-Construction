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
#include "ast.h"

int main(int argc, char* argv[]){

	if(argc<3){
		printf("Wrong command. Type: ./stage1exe testcase.txt \n");
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

	while(flag){

		printf("\n\nEnter number to perform the function:\n");
		printf("0: For exit\n");
		printf("1: For removal of comments (on the console)\n");
		printf("2: For printing the token list (on the console)\n");
		printf("3: For parsing to verify the syntactic correctness of the input source code and printing the parse tree\n");
		printf("4: For printing the total time taken (on the console)\n");
        printf("5: For printing the AST on the console\n");


		printf("\n\nEnter your choice: ");

		int choice;
		scanf("%d",&choice);

		
		switch(choice){

			case 0:{
                printf("THANK YOU!\n");
				flag = false;
			}break;

			case 1:{
				removeComments(argv[1],argv[2]);		
			}break;

            case 2:{
                //print tokens from lexer

				FILE * fp = startLexer(argv[1]);
				initTable();
				while (1)
					{
						tokenInfo pleasework = getNextToken();
						if (pleasework.tid == SENTINEL)
							break;
						if(pleasework.tid!=LEX_ERROR)
						{
						if(pleasework.tid==TK_COMMENT)
							printf("Line no. %d\tLexeme \t\t\t%%\t\tTK_COMMENT", pleasework.lineNo);
						else
							printf("Line no. %d\tLexeme %20s", pleasework.lineNo, pleasework.lexeme);
						}
						printf("\t\t");
						if(pleasework.tid!=TK_COMMENT)
							printToken(pleasework.tid);
						printf("\n");
						
					}
				fclose(fp);
				break;
            }

            case 3:{
                //parser
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
            case 4:{
                clock_t start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();
                // invoke your lexer and parser here

				printf("A\n");

				char *testFile = argv[1];
				char *outfile = argv[2];
				initGrammar(G);
				populateFirstFollow("First.txt", true);
				populateFirstFollow("Follow.txt", false);
				computeParseTable();
				parseTree ptree = parseInputSourceCode(testFile);


                end_time = clock();
                total_CPU_time = (double) (end_time - start_time);
                total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
                printf("\nTotal CPU time = %lf\nTotal CPU time in seconds = %lf\n",total_CPU_time,total_CPU_time_in_seconds);
                // Print both total_CPU_time and total_CPU_time_in_seconds
				break;
            }
            case 5:{
                //AST
                char* testFile = argv[1];
                char* outfile = argv[2];
                initGrammar(G);
				populateFirstFollow("First.txt", true);
				populateFirstFollow("Follow.txt", false);
				computeParseTable();
				parseTree ptree = parseInputSourceCode(testFile);
                ast* astree = initAST(ptree);
				int numNodes = printAST(astree, outfile);
                printf("Number of Nodes\n");
                printf("Number of Children of the root:\n");
                ast* temp = astree->firstChild;
                while(temp) {
                    printf("%d->", temp->nodeType);
                    temp = temp->nextSibling;
                }
                printf("\n");
            }
        }

	}

	return 0;
}

#endif