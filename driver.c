#ifndef _DRIVERDEF_
#define _DRIVERDEF_
#include <stdio.h>
#include <stdbool.h>
#include "parser.h"
#include <time.h>


// char *strip(char *str)
// {
//     while(*str==' ' || *str=='\t') str++;
//     return str;
// }

// void removeComments(char* testcaseFile, char* cleanFile){
//     //printf("%s\n",testcaseFile);
//     FILE *inp = fopen(testcaseFile,"r");
//     //FILE *out = fopen(cleanFile,"w");

//     if(!inp){
//         printf("Error opening testcaseFile\n");
//         return;
//     }

//     char *line = NULL;
//     size_t len = 0;
//     size_t sizeRead;

//     while(1){
//         sizeRead = getline(&line, &len, inp);
//         if(sizeRead==-1)
//             break;
//         char *mod = strip(line);
//         if(strlen(mod)>0){
//             if(mod[0]!='%'){
//                 printf("%s",line);
//             }
//         }
//     }
// }

int main(int argc, char* argv[]){

	if(argc<2){
		printf("Wrong command. Type: ./compiler testcaseX.txt\n");
		return 0;
	}

    printf("GROUP 15:\nAadit Deshpande\nNandan Bharatkumar Parikh\nPreetika Verma\nPritika Ramu\nSneha\n");
	printf("\n\nIMPLEMENTATION DETAILS OF STAGE 1\n");
	printf("1. Initialized grammar\n");
	printf("2. Automated computation of FIRST and FOLLOW sets\n");
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


		printf("\n\nEnter your choice: ");

		int choice;
		scanf("%d",&choice);

		
		switch(choice){

			case 0:{
                printf("THANK YOU!\n");
				flag = false;
			}break;

			case 1:{
				removeComments(argv[1],"dummyname");		
			}break;

            case 2:{
                //print tokens from lexer
            }

            case 3:{
                //parser
            }
            case 4:{
                clock_t start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();
                // invoke your lexer and parser here
                end_time = clock();
                total_CPU_time = (double) (end_time - start_time);
                total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
                printf("\nTotal CPU time = %lf\nTotal CPU time in seconds = %lf\n",total_CPU_time,total_CPU_time_in_seconds);
                // Print both total_CPU_time and total_CPU_time_in_seconds
            }
        }

	}

	return 0;
}

#endif