#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "newRules.h"


void populateFirstFollow(char* textfile, bool first){
    FILE* fp = fopen(textfile,"r");

    if(fp == NULL) {
		printf("Error opening file\n");
        return;
	}

    char *line = NULL;
    size_t len = 0;
    size_t sizeRead;

    sizeRead = getline(&line, &len, fp);
    //printf("%d\n", sizeRead);
    
    //int linecount = 1;

    while(sizeRead != -1) {
        char* tok = strtok(line," ");
        //printf("\nLine number %d\n",linecount);
        int row = atoi(tok);
        //printf("\nLine number %d\n",row);
        tok = strtok(NULL," ");
        while(tok){
            //printf("%d ",atoi(tok)-51);
            if(first){
                First[row][atoi(tok)-51]=true;
            }
            else{
                Follow[row][atoi(tok)-51]=true;
            }
            tok = strtok(NULL," ");
        }
        sizeRead = getline(&line, &len, fp);
        //linecount++;
    }
    //printf("\n");
    // if(first){
    //     printf("Population of First Set Successful\n");
    // }
    // else{
    //     printf("Population of Follow Set Successful\n");
    // }
    fclose(fp);
}

void printFirst(){
    for(int i = 0; i < NON_TERMINALS + TERMINALS; i++){
        printf("First Set of Symbol %d is = {", i);
        for(int j =0; j<TERMINALS; j++){
            if(First[i][j]){
                // printf("%d\t",j+51);
                
                printToken(j+51);
                printf(",");
               
            }
            
        }
        printf("}\n");
        // printf("\n");
    }
}

void printFollow(){
    for(int i = 0; i < NON_TERMINALS + TERMINALS; i++){
        printf("Follow Set of Symbol %d is = {", i);
        for(int j =0; j<TERMINALS; j++){
            if(Follow[i][j]){
                // printf("%d\t",j+51);
                
                printToken(j+51);
                printf(",");
               
            }
            
        }
        printf("}\n");
        // printf("\n");
    }
}


ruleHead* insertRuleList(ruleHead* head, g_RHS* rule){


	ruleHead* temp = (ruleHead *)malloc(sizeof(ruleHead));
	temp->listHead = rule;
	temp->next = head;
	return temp;
}

g_RHS* insertIntoRule(g_RHS* head, g_Term s, bool isTerm){
	//we insert the symbols to get the RHS for the rules

	//insertion has to be done in reverse order

	//g_RHS* temp = (g_RHS *)malloc(sizeof(g_RHS));

	//temp->symbol = s;
	g_RHS* temp = (g_RHS *)malloc(sizeof(g_RHS));
	temp->symbol = s;
	temp->next = NULL;
    temp->isTerminal = isTerm;
	if(head==NULL)
		return temp;

	temp->next = head;
	return temp;
}

void printRule(g_RHS* head){ // Print 1 single rule

	g_RHS* temp = head;
	while(temp!=NULL)
	{
		// printf("A%d ", temp->symbol); // For number printing
        if(temp->isTerminal)
            printToken(temp->symbol);
        else
            printNonTerminal(temp->symbol);
		temp = temp->next;
	} 
	printf("\n");

}

void printRules(Grammar G, int i, bool needArrow){ // Print All Rules for a NT
	
	ruleHead* temp = G[i];
	while(temp!=NULL)
	{
        printNonTerminal(i);
        // printf("A%d ", i); // For number printing
        if(needArrow){
            printf(" -> ");
        }
		printRule(temp->listHead);
		temp = temp->next;

	}

}

void printGrammar(Grammar G, int len){

    for(int i=0;i<len;i++){
		// printf("\n");
		printRules(G, i, true);
    }

}

void printToken(int token) {
	switch(token)
	{
		case 51: printf("<eps> ");
				break;
		case 52: printf("TK_ASSIGNOP ");
				break;
		case 53: printf("TK_COMMENT ");
				break;
		case 54: printf("TK_FIELDID ");
				break;
		case 55: printf("TK_ID ");
				break;
		case 56: printf("TK_NUM ");
				break;
		case 57: printf("TK_RNUM ");
				break;
		case 58: printf("TK_FUNID ");
				break;
		case 59: printf("TK_WITH ");
				break;
		case 60: printf("TK_PARAMETERS ");
				break;
		case 61: printf("TK_END ");
				break;
		case 62: printf("TK_WHILE ");
				break;
		case 63: printf("TK_TYPE ");
				break;
		case 64: printf("TK_MAIN ");
				break;
		case 65: printf("TK_GLOBAL ");
				break;
		case 66: printf("TK_PARAMETER ");
				break;
		case 67: printf("TK_LIST ");
				break;
		case 68: printf("TK_SQL ");
				break;
		case 69: printf("TK_SQR ");
				break;
		case 70: printf("TK_INPUT ");
				break;
		case 71: printf("TK_OUTPUT ");
				break;
		case 72: printf("TK_INT ");
				break;
		case 73: printf("TK_REAL ");
				break;
		case 74: printf("TK_COMMA ");
				break;
		case 75: printf("TK_SEM ");
				break;
		case 76: printf("TK_COLON ");
				break;
		case 77: printf("TK_DOT ");
				break;
		case 78: printf("TK_ENDWHILE ");
				break;
		case 79: printf("TK_OP ");
				break;
		case 80: printf("TK_CL ");
				break;
		case 81: printf("TK_IF ");
				break;
		case 82: printf("TK_THEN ");
				break;
		case 83: printf("TK_ENDIF ");
				break;
		case 84: printf("TK_READ ");
				break;
		case 85: printf("TK_WRITE ");
				break;
		case 86: printf("TK_RETURN ");
				break;
		case 87: printf("TK_PLUS ");
				break;
		case 88: printf("TK_MINUS ");
				break;
		case 89: printf("TK_MUL ");
				break;
		case 90: printf("TK_DIV ");
				break;
		case 91: printf("TK_CALL ");
				break;
		case 92: printf("TK_RECORD ");
				break;
		case 93: printf("TK_ENDRECORD ");
				break;
		case 94: printf("TK_ELSE ");
				break;
		case 95: printf("TK_AND ");
				break;
		case 96: printf("TK_OR ");
				break;
		case 97: printf("TK_NOT ");
				break;
		case 98: printf("TK_LT ");
				break;
		case 99: printf("TK_LE ");
				break;
		case 100: printf("TK_AS ");
				break;
		case 101: printf("TK_EQ ");
				break;
		case 102: printf("TK_GT ");
				break;
		case 103: printf("TK_GE ");
				break;
		case 104: printf("TK_NE ");
				break;
        case 105: printf("SENTINEL ");
				break;
		case 107: printf("TK_RUID");
				break;
		case 108: printf("TK_UNION ");
				break;
		case 109: printf("TK_ENDUNION");
				break;
		case 110: printf("TK_DEFINETYPE");
				break;
		default: printf("<No Symbol found>");
				break;
	}
}

void printNonTerminal(int token) {
	switch(token)
	{
		case 0: printf("<program> ");
				break;
		case 1: printf("<mainFunction> ");
				break;
		case 2: printf("<otherFunctions> ");
				break;
		case 3: printf("<function> ");
				break;
		case 4: printf("<input_par> ");
				break;
		case 5: printf("<output_par> ");
				break;
		case 6: printf("<parameter_list> ");
				break;
		case 7: printf("<dataType> ");
				break;
		case 8: printf("<primitiveDatatype> ");
				break;
		case 9: printf("<constructedDatatype> ");
				break;
		case 10: printf("<remaining_list> ");
				break;
		case 11: printf("<stmts> ");
				break;
		case 12: printf("<typeDefinitions> ");
				break;
		case 13: printf("<typeDefinition> ");
				break;
		case 14: printf("<fieldDefinitions> ");
				break;
		case 15: printf("<fieldDefinition> ");
				break;
		case 16: printf("<moreFields> ");
				break;
		case 17: printf("<declarations> ");
				break;
		case 18: printf("<declaration> ");
				break;
		case 19: printf("<global_or_not> ");
				break;
		case 20: printf("<otherStmts> ");
				break;
		case 21: printf("<stmt> ");
				break;
		case 22: printf("<assignmentStmt> ");
				break;
		case 23: printf("<singleOrRecId> ");
				break;
		case 24: printf("<actualOrRedefined> ");// Changed to Support Grammar
				break;
		case 25: printf("<funCallStmt> ");
				break;
		case 26: printf("<outputParameters> ");
				break;
		case 27: printf("<inputParameters> ");
				break;
		case 28: printf("<iterativeStmt> ");
				break;
		case 29: printf("<conditionalStmt> ");
				break;
		case 30: printf("<elsePart> ");
				break;
		case 31: printf("<ioStmt> ");
				break;
		case 32: printf("<fieldtype> "); // Changed to Support Grammar
				break;
		case 33: printf("<option_single_constructed> "); // Changed to Support Grammar
				break;
		case 34: printf("<arithmeticExpression> ");
				break;
		case 35: printf("<expPrime> ");
				break;
		case 36: printf("<term> ");
				break;
		case 37: printf("<termPrime> ");
				break;
		case 38: printf("<factor> ");
				break;
		case 39: printf("<highPrecedenceOperators> ");
				break;
		case 40: printf("<lowPrecedenceOperators> ");
				break;
		case 41: printf("<oneExpansion> "); // Changed to Support Grammar
				break;
		case 42: printf("<moreExpansions> "); // Changed to Support Grammar 
				break;
		case 43: printf("<booleanExpression> ");
				break;
		case 44: printf("<var> ");
				break;
		case 45: printf("<logicalOp> ");
				break;
		case 46: printf("<relationalOp> ");
				break;
		case 47: printf("<returnStmt> ");
				break;
		case 48: printf("<optionalReturn> ");
				break;
		case 49: printf("<idList> ");
				break;
		case 50: printf("<more_ids> ");
				break;
		case 111: printf("<definetypestmt> ");
				break;
		case 112: printf("<A> ");
				break;

		default: printf("<No Symbol found>");
				break;
		
	}
}

void initGrammar(){
	G[0] = NULL;
    g_RHS* Rule141765 = NULL;
    Rule141765 = insertIntoRule(Rule141765,1, false);
    Rule141765 = insertIntoRule(Rule141765,2, false);
    G[0] = insertRuleList(G[0], Rule141765);

    G[1] = NULL;
    g_RHS* Rule591183 = NULL;
    Rule591183 = insertIntoRule(Rule591183,61, true);
    Rule591183 = insertIntoRule(Rule591183,11, false);
    Rule591183 = insertIntoRule(Rule591183,64, true);
    G[1] = insertRuleList(G[1], Rule591183);

    G[2] = NULL;
    g_RHS* Rule310546 = NULL;
    Rule310546 = insertIntoRule(Rule310546,2, false);
    Rule310546 = insertIntoRule(Rule310546,3, false);
    G[2] = insertRuleList(G[2], Rule310546);

    g_RHS* Rule207287 = NULL;
    Rule207287 = insertIntoRule(Rule207287,51, true);
    G[2] = insertRuleList(G[2], Rule207287);

    G[3] = NULL;
    g_RHS* Rule728832 = NULL;
    Rule728832 = insertIntoRule(Rule728832,61, true);
    Rule728832 = insertIntoRule(Rule728832,11, false);
    Rule728832 = insertIntoRule(Rule728832,75, true);
    Rule728832 = insertIntoRule(Rule728832,5, false);
    Rule728832 = insertIntoRule(Rule728832,4, false);
    Rule728832 = insertIntoRule(Rule728832,58, true);
    G[3] = insertRuleList(G[3], Rule728832);

    G[4] = NULL;
    g_RHS* Rule786790 = NULL;
    Rule786790 = insertIntoRule(Rule786790,69, true);
    Rule786790 = insertIntoRule(Rule786790,6, false);
    Rule786790 = insertIntoRule(Rule786790,68, true);
    Rule786790 = insertIntoRule(Rule786790,67, true);
    Rule786790 = insertIntoRule(Rule786790,66, true);
    Rule786790 = insertIntoRule(Rule786790,70, true);
    G[4] = insertRuleList(G[4], Rule786790);

    G[5] = NULL;
    g_RHS* Rule656885 = NULL;
    Rule656885 = insertIntoRule(Rule656885,69, true);
    Rule656885 = insertIntoRule(Rule656885,6, false);
    Rule656885 = insertIntoRule(Rule656885,68, true);
    Rule656885 = insertIntoRule(Rule656885,67, true);
    Rule656885 = insertIntoRule(Rule656885,66, true);
    Rule656885 = insertIntoRule(Rule656885,71, true);
    G[5] = insertRuleList(G[5], Rule656885);

    g_RHS* Rule60880 = NULL;
    Rule60880 = insertIntoRule(Rule60880,51, true);
    G[5] = insertRuleList(G[5], Rule60880);

    G[6] = NULL;
    g_RHS* Rule682698 = NULL;
    Rule682698 = insertIntoRule(Rule682698,10, false);
    Rule682698 = insertIntoRule(Rule682698,55, true);
    Rule682698 = insertIntoRule(Rule682698,7, false);
    G[6] = insertRuleList(G[6], Rule682698);

    G[7] = NULL;
    g_RHS* Rule508400 = NULL;
    Rule508400 = insertIntoRule(Rule508400,8, false);
    G[7] = insertRuleList(G[7], Rule508400);

    g_RHS* Rule938990 = NULL;
    Rule938990 = insertIntoRule(Rule938990,9, false);
    G[7] = insertRuleList(G[7], Rule938990);

    G[8] = NULL;
    g_RHS* Rule385359 = NULL;
    Rule385359 = insertIntoRule(Rule385359,72, true);
    G[8] = insertRuleList(G[8], Rule385359);

    g_RHS* Rule112664 = NULL;
    Rule112664 = insertIntoRule(Rule112664,73, true);
    G[8] = insertRuleList(G[8], Rule112664);

    G[9] = NULL;
    g_RHS* Rule294065 = NULL;
    Rule294065 = insertIntoRule(Rule294065,107, true);
    Rule294065 = insertIntoRule(Rule294065,92, true);
    G[9] = insertRuleList(G[9], Rule294065);

    g_RHS* Rule898819 = NULL;
    Rule898819 = insertIntoRule(Rule898819,107, true);
    Rule898819 = insertIntoRule(Rule898819,108, true);
    G[9] = insertRuleList(G[9], Rule898819);

    g_RHS* Rule556652 = NULL;
    Rule556652 = insertIntoRule(Rule556652,107, true);
    G[9] = insertRuleList(G[9], Rule556652);

    G[10] = NULL;
    g_RHS* Rule998087 = NULL;
    Rule998087 = insertIntoRule(Rule998087,6, false);
    Rule998087 = insertIntoRule(Rule998087,74, true);
    G[10] = insertRuleList(G[10], Rule998087);

    g_RHS* Rule166887 = NULL;
    Rule166887 = insertIntoRule(Rule166887,51, true);
    G[10] = insertRuleList(G[10], Rule166887);

    G[11] = NULL;
    g_RHS* Rule583743 = NULL;
    Rule583743 = insertIntoRule(Rule583743,47, false);
    Rule583743 = insertIntoRule(Rule583743,20, false);
    Rule583743 = insertIntoRule(Rule583743,17, false);
    Rule583743 = insertIntoRule(Rule583743,12, false);
    G[11] = insertRuleList(G[11], Rule583743);

    G[12] = NULL;
    g_RHS* Rule756180 = NULL;
    Rule756180 = insertIntoRule(Rule756180,12, false);
    Rule756180 = insertIntoRule(Rule756180,24, false);
    G[12] = insertRuleList(G[12], Rule756180);

    g_RHS* Rule452661 = NULL;
    Rule452661 = insertIntoRule(Rule452661,51, true);
    G[12] = insertRuleList(G[12], Rule452661);

    G[24] = NULL;
    g_RHS* Rule630284 = NULL;
    Rule630284 = insertIntoRule(Rule630284,13, false);
    G[24] = insertRuleList(G[24], Rule630284);

    g_RHS* Rule916641 = NULL;
    Rule916641 = insertIntoRule(Rule916641,111, false);
    G[24] = insertRuleList(G[24], Rule916641);

    G[13] = NULL;
    g_RHS* Rule958605 = NULL;
    Rule958605 = insertIntoRule(Rule958605,93, true);
    Rule958605 = insertIntoRule(Rule958605,14, false);
    Rule958605 = insertIntoRule(Rule958605,107, true);
    Rule958605 = insertIntoRule(Rule958605,92, true);
    G[13] = insertRuleList(G[13], Rule958605);

    g_RHS* Rule248097 = NULL;
    Rule248097 = insertIntoRule(Rule248097,109, true);
    Rule248097 = insertIntoRule(Rule248097,14, false);
    Rule248097 = insertIntoRule(Rule248097,107, true);
    Rule248097 = insertIntoRule(Rule248097,108, true);
    G[13] = insertRuleList(G[13], Rule248097);

    G[14] = NULL;
    g_RHS* Rule965234 = NULL;
    Rule965234 = insertIntoRule(Rule965234,16, false);
    Rule965234 = insertIntoRule(Rule965234,15, false);
    Rule965234 = insertIntoRule(Rule965234,15, false);
    G[14] = insertRuleList(G[14], Rule965234);

    G[15] = NULL;
    g_RHS* Rule42739 = NULL;
    Rule42739 = insertIntoRule(Rule42739,75, true);
    Rule42739 = insertIntoRule(Rule42739,54, true);
    Rule42739 = insertIntoRule(Rule42739,76, true);
    Rule42739 = insertIntoRule(Rule42739,32, false);
    Rule42739 = insertIntoRule(Rule42739,63, true);
    G[15] = insertRuleList(G[15], Rule42739);

    G[32] = NULL;
    g_RHS* Rule957280 = NULL;
    Rule957280 = insertIntoRule(Rule957280,8, false);
    G[32] = insertRuleList(G[32], Rule957280);

    g_RHS* Rule843083 = NULL;
    Rule843083 = insertIntoRule(Rule843083,107, true);
    G[32] = insertRuleList(G[32], Rule843083);

    G[16] = NULL;
    g_RHS* Rule100043 = NULL;
    Rule100043 = insertIntoRule(Rule100043,16, false);
    Rule100043 = insertIntoRule(Rule100043,15, false);
    G[16] = insertRuleList(G[16], Rule100043);

    g_RHS* Rule437859 = NULL;
    Rule437859 = insertIntoRule(Rule437859,51, true);
    G[16] = insertRuleList(G[16], Rule437859);

    G[17] = NULL;
    g_RHS* Rule518766 = NULL;
    Rule518766 = insertIntoRule(Rule518766,17, false);
    Rule518766 = insertIntoRule(Rule518766,18, false);
    G[17] = insertRuleList(G[17], Rule518766);

    g_RHS* Rule336261 = NULL;
    Rule336261 = insertIntoRule(Rule336261,51, true);
    G[17] = insertRuleList(G[17], Rule336261);

    G[18] = NULL;
    g_RHS* Rule972573 = NULL;
    Rule972573 = insertIntoRule(Rule972573,75, true);
    Rule972573 = insertIntoRule(Rule972573,19, false);
    Rule972573 = insertIntoRule(Rule972573,55, true);
    Rule972573 = insertIntoRule(Rule972573,76, true);
    Rule972573 = insertIntoRule(Rule972573,7, false);
    Rule972573 = insertIntoRule(Rule972573,63, true);
    G[18] = insertRuleList(G[18], Rule972573);

    G[19] = NULL;
    g_RHS* Rule451182 = NULL;
    Rule451182 = insertIntoRule(Rule451182,65, true);
    Rule451182 = insertIntoRule(Rule451182,76, true);
    G[19] = insertRuleList(G[19], Rule451182);

    g_RHS* Rule547762 = NULL;
    Rule547762 = insertIntoRule(Rule547762,51, true);
    G[19] = insertRuleList(G[19], Rule547762);

    G[20] = NULL;
    g_RHS* Rule216042 = NULL;
    Rule216042 = insertIntoRule(Rule216042,20, false);
    Rule216042 = insertIntoRule(Rule216042,21, false);
    G[20] = insertRuleList(G[20], Rule216042);

    g_RHS* Rule485571 = NULL;
    Rule485571 = insertIntoRule(Rule485571,51, true);
    G[20] = insertRuleList(G[20], Rule485571);

    G[21] = NULL;
    g_RHS* Rule425397 = NULL;
    Rule425397 = insertIntoRule(Rule425397,22, false);
    G[21] = insertRuleList(G[21], Rule425397);

    g_RHS* Rule965149 = NULL;
    Rule965149 = insertIntoRule(Rule965149,28, false);
    G[21] = insertRuleList(G[21], Rule965149);

    g_RHS* Rule515461 = NULL;
    Rule515461 = insertIntoRule(Rule515461,29, false);
    G[21] = insertRuleList(G[21], Rule515461);

    g_RHS* Rule867604 = NULL;
    Rule867604 = insertIntoRule(Rule867604,31, false);
    G[21] = insertRuleList(G[21], Rule867604);

    g_RHS* Rule788056 = NULL;
    Rule788056 = insertIntoRule(Rule788056,25, false);
    G[21] = insertRuleList(G[21], Rule788056);

    G[22] = NULL;
    g_RHS* Rule854326 = NULL;
    Rule854326 = insertIntoRule(Rule854326,75, true);
    Rule854326 = insertIntoRule(Rule854326,34, false);
    Rule854326 = insertIntoRule(Rule854326,52, true);
    Rule854326 = insertIntoRule(Rule854326,23, false);
    G[22] = insertRuleList(G[22], Rule854326);

    G[23] = NULL;
    g_RHS* Rule996835 = NULL;
    Rule996835 = insertIntoRule(Rule996835,33, false);
    Rule996835 = insertIntoRule(Rule996835,55, true);
    G[23] = insertRuleList(G[23], Rule996835);

    G[33] = NULL;
    g_RHS* Rule495311 = NULL;
    Rule495311 = insertIntoRule(Rule495311,51, true);
    G[33] = insertRuleList(G[33], Rule495311);

    G[33] = NULL;
    g_RHS* Rule540189 = NULL;
    Rule540189 = insertIntoRule(Rule540189,42, false);
    Rule540189 = insertIntoRule(Rule540189,41, false);
    G[33] = insertRuleList(G[33], Rule540189);

    G[41] = NULL;
    g_RHS* Rule314852 = NULL;
    Rule314852 = insertIntoRule(Rule314852,54, true);
    Rule314852 = insertIntoRule(Rule314852,77, true);
    G[41] = insertRuleList(G[41], Rule314852);

    G[42] = NULL;
    g_RHS* Rule288892 = NULL;
    Rule288892 = insertIntoRule(Rule288892,42, false);
    Rule288892 = insertIntoRule(Rule288892,41, false);
    G[42] = insertRuleList(G[42], Rule288892);

    G[42] = NULL;
    g_RHS* Rule7077 = NULL;
    Rule7077 = insertIntoRule(Rule7077,51, true);
    G[42] = insertRuleList(G[42], Rule7077);

    G[25] = NULL;
    g_RHS* Rule381719 = NULL;
    Rule381719 = insertIntoRule(Rule381719,75, true);
    Rule381719 = insertIntoRule(Rule381719,27, false);
    Rule381719 = insertIntoRule(Rule381719,60, true);
    Rule381719 = insertIntoRule(Rule381719,59, true);
    Rule381719 = insertIntoRule(Rule381719,58, true);
    Rule381719 = insertIntoRule(Rule381719,91, true);
    Rule381719 = insertIntoRule(Rule381719,26, false);
    G[25] = insertRuleList(G[25], Rule381719);

    G[26] = NULL;
    g_RHS* Rule377416 = NULL;
    Rule377416 = insertIntoRule(Rule377416,52, true);
    Rule377416 = insertIntoRule(Rule377416,69, true);
    Rule377416 = insertIntoRule(Rule377416,49, false);
    Rule377416 = insertIntoRule(Rule377416,68, true);
    G[26] = insertRuleList(G[26], Rule377416);

    g_RHS* Rule478079 = NULL;
    Rule478079 = insertIntoRule(Rule478079,51, true);
    G[26] = insertRuleList(G[26], Rule478079);

    G[27] = NULL;
    g_RHS* Rule843901 = NULL;
    Rule843901 = insertIntoRule(Rule843901,69, true);
    Rule843901 = insertIntoRule(Rule843901,49, false);
    Rule843901 = insertIntoRule(Rule843901,68, true);
    G[27] = insertRuleList(G[27], Rule843901);

    G[28] = NULL;
    g_RHS* Rule307932 = NULL;
    Rule307932 = insertIntoRule(Rule307932,78, true);
    Rule307932 = insertIntoRule(Rule307932,20, false);
    Rule307932 = insertIntoRule(Rule307932,21, false);
    Rule307932 = insertIntoRule(Rule307932,80, true);
    Rule307932 = insertIntoRule(Rule307932,43, false);
    Rule307932 = insertIntoRule(Rule307932,79, true);
    Rule307932 = insertIntoRule(Rule307932,62, true);
    G[28] = insertRuleList(G[28], Rule307932);

    G[29] = NULL;
    g_RHS* Rule280473 = NULL;
    Rule280473 = insertIntoRule(Rule280473,30, false);
    Rule280473 = insertIntoRule(Rule280473,20, false);
    Rule280473 = insertIntoRule(Rule280473,21, false);
    Rule280473 = insertIntoRule(Rule280473,82, true);
    Rule280473 = insertIntoRule(Rule280473,80, true);
    Rule280473 = insertIntoRule(Rule280473,43, false);
    Rule280473 = insertIntoRule(Rule280473,79, true);
    Rule280473 = insertIntoRule(Rule280473,81, true);
    G[29] = insertRuleList(G[29], Rule280473);

    G[30] = NULL;
    g_RHS* Rule516166 = NULL;
    Rule516166 = insertIntoRule(Rule516166,83, true);
    Rule516166 = insertIntoRule(Rule516166,20, false);
    Rule516166 = insertIntoRule(Rule516166,21, false);
    Rule516166 = insertIntoRule(Rule516166,94, true);
    G[30] = insertRuleList(G[30], Rule516166);

    g_RHS* Rule190139 = NULL;
    Rule190139 = insertIntoRule(Rule190139,83, true);
    G[30] = insertRuleList(G[30], Rule190139);

    G[31] = NULL;
    g_RHS* Rule887236 = NULL;
    Rule887236 = insertIntoRule(Rule887236,75, true);
    Rule887236 = insertIntoRule(Rule887236,80, true);
    Rule887236 = insertIntoRule(Rule887236,44, false);
    Rule887236 = insertIntoRule(Rule887236,79, true);
    Rule887236 = insertIntoRule(Rule887236,84, true);
    G[31] = insertRuleList(G[31], Rule887236);

    g_RHS* Rule386388 = NULL;
    Rule386388 = insertIntoRule(Rule386388,75, true);
    Rule386388 = insertIntoRule(Rule386388,80, true);
    Rule386388 = insertIntoRule(Rule386388,44, false);
    Rule386388 = insertIntoRule(Rule386388,79, true);
    Rule386388 = insertIntoRule(Rule386388,85, true);
    G[31] = insertRuleList(G[31], Rule386388);

    G[34] = NULL;
    g_RHS* Rule947820 = NULL;
    Rule947820 = insertIntoRule(Rule947820,35, false);
    Rule947820 = insertIntoRule(Rule947820,36, false);
    G[34] = insertRuleList(G[34], Rule947820);

    G[35] = NULL;
    g_RHS* Rule654984 = NULL;
    Rule654984 = insertIntoRule(Rule654984,35, false);
    Rule654984 = insertIntoRule(Rule654984,36, false);
    Rule654984 = insertIntoRule(Rule654984,40, false);
    G[35] = insertRuleList(G[35], Rule654984);

    g_RHS* Rule587669 = NULL;
    Rule587669 = insertIntoRule(Rule587669,51, true);
    G[35] = insertRuleList(G[35], Rule587669);

    G[36] = NULL;
    g_RHS* Rule23762 = NULL;
    Rule23762 = insertIntoRule(Rule23762,37, false);
    Rule23762 = insertIntoRule(Rule23762,38, false);
    G[36] = insertRuleList(G[36], Rule23762);

    G[37] = NULL;
    g_RHS* Rule884708 = NULL;
    Rule884708 = insertIntoRule(Rule884708,37, false);
    Rule884708 = insertIntoRule(Rule884708,38, false);
    Rule884708 = insertIntoRule(Rule884708,39, false);
    G[37] = insertRuleList(G[37], Rule884708);

    g_RHS* Rule934942 = NULL;
    Rule934942 = insertIntoRule(Rule934942,51, true);
    G[37] = insertRuleList(G[37], Rule934942);

    G[38] = NULL;
    g_RHS* Rule458110 = NULL;
    Rule458110 = insertIntoRule(Rule458110,80, true);
    Rule458110 = insertIntoRule(Rule458110,34, false);
    Rule458110 = insertIntoRule(Rule458110,79, true);
    G[38] = insertRuleList(G[38], Rule458110);

    g_RHS* Rule582613 = NULL;
    Rule582613 = insertIntoRule(Rule582613,44, false);
    G[38] = insertRuleList(G[38], Rule582613);

    G[39] = NULL;
    g_RHS* Rule856761 = NULL;
    Rule856761 = insertIntoRule(Rule856761,89, true);
    G[39] = insertRuleList(G[39], Rule856761);

    g_RHS* Rule128228 = NULL;
    Rule128228 = insertIntoRule(Rule128228,90, true);
    G[39] = insertRuleList(G[39], Rule128228);

    G[40] = NULL;
    g_RHS* Rule189832 = NULL;
    Rule189832 = insertIntoRule(Rule189832,87, true);
    G[40] = insertRuleList(G[40], Rule189832);

    g_RHS* Rule260401 = NULL;
    Rule260401 = insertIntoRule(Rule260401,88, true);
    G[40] = insertRuleList(G[40], Rule260401);

    G[43] = NULL;
    g_RHS* Rule156608 = NULL;
    Rule156608 = insertIntoRule(Rule156608,80, true);
    Rule156608 = insertIntoRule(Rule156608,43, false);
    Rule156608 = insertIntoRule(Rule156608,79, true);
    Rule156608 = insertIntoRule(Rule156608,45, false);
    Rule156608 = insertIntoRule(Rule156608,80, true);
    Rule156608 = insertIntoRule(Rule156608,43, false);
    Rule156608 = insertIntoRule(Rule156608,79, true);
    G[43] = insertRuleList(G[43], Rule156608);

    g_RHS* Rule333934 = NULL;
    Rule333934 = insertIntoRule(Rule333934,44, false);
    Rule333934 = insertIntoRule(Rule333934,46, false);
    Rule333934 = insertIntoRule(Rule333934,44, false);
    G[43] = insertRuleList(G[43], Rule333934);

    g_RHS* Rule85068 = NULL;
    Rule85068 = insertIntoRule(Rule85068,80, true);
    Rule85068 = insertIntoRule(Rule85068,43, false);
    Rule85068 = insertIntoRule(Rule85068,79, true);
    Rule85068 = insertIntoRule(Rule85068,97, true);
    G[43] = insertRuleList(G[43], Rule85068);

    G[44] = NULL;
    g_RHS* Rule959415 = NULL;
    Rule959415 = insertIntoRule(Rule959415,23, false);
    G[44] = insertRuleList(G[44], Rule959415);

    g_RHS* Rule559772 = NULL;
    Rule559772 = insertIntoRule(Rule559772,56, true);
    G[44] = insertRuleList(G[44], Rule559772);

    g_RHS* Rule661976 = NULL;
    Rule661976 = insertIntoRule(Rule661976,57, true);
    G[44] = insertRuleList(G[44], Rule661976);

    G[45] = NULL;
    g_RHS* Rule741429 = NULL;
    Rule741429 = insertIntoRule(Rule741429,95, true);
    G[45] = insertRuleList(G[45], Rule741429);

    g_RHS* Rule969827 = NULL;
    Rule969827 = insertIntoRule(Rule969827,96, true);
    G[45] = insertRuleList(G[45], Rule969827);

    G[46] = NULL;
    g_RHS* Rule851079 = NULL;
    Rule851079 = insertIntoRule(Rule851079,98, true);
    G[46] = insertRuleList(G[46], Rule851079);

    g_RHS* Rule804224 = NULL;
    Rule804224 = insertIntoRule(Rule804224,99, true);
    G[46] = insertRuleList(G[46], Rule804224);

    g_RHS* Rule324551 = NULL;
    Rule324551 = insertIntoRule(Rule324551,101, true);
    G[46] = insertRuleList(G[46], Rule324551);

    g_RHS* Rule237761 = NULL;
    Rule237761 = insertIntoRule(Rule237761,102, true);
    G[46] = insertRuleList(G[46], Rule237761);

    g_RHS* Rule416836 = NULL;
    Rule416836 = insertIntoRule(Rule416836,103, true);
    G[46] = insertRuleList(G[46], Rule416836);

    g_RHS* Rule872933 = NULL;
    Rule872933 = insertIntoRule(Rule872933,104, true);
    G[46] = insertRuleList(G[46], Rule872933);

    G[47] = NULL;
    g_RHS* Rule146010 = NULL;
    Rule146010 = insertIntoRule(Rule146010,75, true);
    Rule146010 = insertIntoRule(Rule146010,48, false);
    Rule146010 = insertIntoRule(Rule146010,86, true);
    G[47] = insertRuleList(G[47], Rule146010);

    G[48] = NULL;
    g_RHS* Rule214012 = NULL;
    Rule214012 = insertIntoRule(Rule214012,69, true);
    Rule214012 = insertIntoRule(Rule214012,49, false);
    Rule214012 = insertIntoRule(Rule214012,68, true);
    G[48] = insertRuleList(G[48], Rule214012);

    g_RHS* Rule272677 = NULL;
    Rule272677 = insertIntoRule(Rule272677,51, true);
    G[48] = insertRuleList(G[48], Rule272677);

    G[49] = NULL;
    g_RHS* Rule299435 = NULL;
    Rule299435 = insertIntoRule(Rule299435,50, false);
    Rule299435 = insertIntoRule(Rule299435,55, true);
    G[49] = insertRuleList(G[49], Rule299435);

    G[50] = NULL;
    g_RHS* Rule889578 = NULL;
    Rule889578 = insertIntoRule(Rule889578,49, false);
    Rule889578 = insertIntoRule(Rule889578,74, true);
    G[50] = insertRuleList(G[50], Rule889578);

    g_RHS* Rule506593 = NULL;
    Rule506593 = insertIntoRule(Rule506593,51, true);
    G[50] = insertRuleList(G[50], Rule506593);

    G[111] = NULL;
    g_RHS* Rule288585 = NULL;
    Rule288585 = insertIntoRule(Rule288585,75, true);
    Rule288585 = insertIntoRule(Rule288585,107, true);
    Rule288585 = insertIntoRule(Rule288585,100, true);
    Rule288585 = insertIntoRule(Rule288585,107, true);
    Rule288585 = insertIntoRule(Rule288585,112, false);
    Rule288585 = insertIntoRule(Rule288585,110, true);
    G[111] = insertRuleList(G[111], Rule288585);

    G[112] = NULL;
    g_RHS* Rule204291 = NULL;
    Rule204291 = insertIntoRule(Rule204291,92, true);
    G[112] = insertRuleList(G[112], Rule204291);

    g_RHS* Rule236082 = NULL;
    Rule236082 = insertIntoRule(Rule236082,108, true);
    G[112] = insertRuleList(G[112], Rule236082);
}

// int main(){
   
//     // Insert all Grammar Rules
// 	initGrammar();


//     printGrammar(G, 200);
  
//     //populateFirstFollow("First.txt",true);
//     //printFirst();

//     // populateFirstFollow("Follow.txt",false);
//     // printFollow();

//     // for(int i = 0; i < NON_TERMINALS; i++){
//     //     for(int j =0; j<TERMINALS; j++){
//     //         if(Follow[i][j]){
//     //             printf("%d\t",j+51);
//     //         }
//     //     }
//     //     printf("\n");
//     // }
// }
