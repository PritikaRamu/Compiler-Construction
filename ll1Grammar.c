/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/

#include "parser.h"

// Automatic First Set Computation

void fillFirstArray(g_Term lhs, bool* A){ // Union of bool array and First Table Row
    for(int i=0; i<TERMINALS;i++){ 

        if(A[i]==true || First[lhs][i]==true)
            First[lhs][i] = true;
        else
            First[lhs][i] = false;
    }
}

bool containsEpsilon(g_Term lhs){

    bool containsERule = false;
    ruleHead* iter = G[lhs];
    while(iter!=NULL){
        g_RHS* rhs = iter->listHead; 
      		
        if(rhs->symbol == eps){ // If First Symbol is a Terminal
            containsERule = true;
    }
        iter = iter->next;
    }

    return containsERule;
}

bool* firstNT(g_Term i){ // GOES OVER all rules of 1 Non-Terminal
    ruleHead* iter = G[i];
    while(iter!=NULL){
        firstRuleNT(i, iter);
        iter = iter->next;
    }

    return First[i];
}

void firstRuleNT(g_Term lhs, ruleHead* rhs){ // GOES OVER 1 rule of 1 Non-Terminal
	
    g_RHS* iter = rhs->listHead; 
	      		
    if(iter->isTerminal){ // If First Symbol is a Terminal
            First[lhs][(iter->symbol)-eps] = true;
    }
	else{ // If First Symbol is a Non-Terminal
		bool* array = firstNT(iter->symbol);
        fillFirstArray(lhs, array);
	}
    if(containsEpsilon(iter->symbol)){ // If it contains an e-rule, check next too
        iter = iter->next;
        bool* array = firstNT((iter)->symbol);
        fillFirstArray(lhs, array);
    }
}

void computeFirstAndFollow(){ // GOES OVER ALL grammar symbols

    // Initialize ALL to False
    for(int i=0;i<NON_TERMINALS+TERMINALS;i++){
        for(int j=0; j<TERMINALS; j++){
            First[i][j] = false;
        }
    }
    // Real Computation
	for(int i = 0; i < TERMINALS + NON_TERMINALS; i++){	
		
        if(i>eps){ // First Set of all Terminals are themselves!
            First[i][i-eps] = true;
        }
        
        else{ // First Set of Non-Terminals
            firstNT(i);
        }
	}
}

void populateFirstFollow(char *textfile, bool first)
{
    FILE *fp = fopen(textfile, "r");

    if (fp == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    size_t sizeRead;

    sizeRead = getline(&line, &len, fp);

    while (sizeRead != -1)
    {
        char *tok = strtok(line, " ");
        int row = atoi(tok);
        tok = strtok(NULL, " ");
        while (tok)
        {
            if (first)
            {
                First[row][atoi(tok) - eps] = true;
            }
            else
            {
                Follow[row][atoi(tok) - eps] = true;
            }
            tok = strtok(NULL, " ");
        }
        sizeRead = getline(&line, &len, fp);
    }
    fclose(fp);
}

void printFirst()
{
    for (int i = 0; i < NON_TERMINALS + TERMINALS; i++)
    {
        // printf("First Set of Symbol ");
        printSymbol(i);
        printf(" ");
        // printf(" is = {");
        for (int j = 0; j < TERMINALS; j++)
        {
            if (First[i][j] == true && (i != LEX_ERROR))
            {
                printToken(j + eps);
                // printf(",");
            }
        }
        printf("\n");
    }
}

void printFollow()
{
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        // printf("Follow Set of");
        printSymbol(i);
        printf(" ");
        for (int j = 0; j < TERMINALS; j++)
        {
            if (Follow[i][j])
            {
                printToken(j + eps);
                // printf(",");
            }
        }
        // printf("}\n");
        printf("\n");
    }
}

// GRAMMAR Functions
ruleHead *insertRuleList(ruleHead *head, g_RHS *rule)
{
    ruleHead *temp = (ruleHead *)malloc(sizeof(ruleHead));
    temp->listHead = rule;
    temp->next = head;
    return temp;
}

g_RHS *insertIntoRule(g_RHS *head, g_Term s, bool isTerm)
{
    g_RHS *temp = (g_RHS *)malloc(sizeof(g_RHS));
    temp->symbol = s;
    temp->next = NULL;
    temp->isTerminal = isTerm;
    if (head == NULL)
        return temp;
    temp->next = head;
    return temp;
}

void initGrammar()
{
    G[0] = NULL;
    g_RHS *Rule1417TK_GLOBAL = NULL;
    Rule1417TK_GLOBAL = insertIntoRule(Rule1417TK_GLOBAL, 1, false);
    Rule1417TK_GLOBAL = insertIntoRule(Rule1417TK_GLOBAL, 2, false);
    G[0] = insertRuleList(G[0], Rule1417TK_GLOBAL);

    G[1] = NULL;
    g_RHS *RuleTK_WITH11TK_ENDIF = NULL;
    RuleTK_WITH11TK_ENDIF = insertIntoRule(RuleTK_WITH11TK_ENDIF, TK_END, true);
    RuleTK_WITH11TK_ENDIF = insertIntoRule(RuleTK_WITH11TK_ENDIF, 11, false);
    RuleTK_WITH11TK_ENDIF = insertIntoRule(RuleTK_WITH11TK_ENDIF, TK_MAIN, true);
    G[1] = insertRuleList(G[1], RuleTK_WITH11TK_ENDIF);

    G[2] = NULL;
    g_RHS *Rule310TK_FIELDID6 = NULL;
    Rule310TK_FIELDID6 = insertIntoRule(Rule310TK_FIELDID6, 2, false);
    Rule310TK_FIELDID6 = insertIntoRule(Rule310TK_FIELDID6, 3, false);
    G[2] = insertRuleList(G[2], Rule310TK_FIELDID6);

    g_RHS *Rule20TK_INTTK_PLUS = NULL;
    Rule20TK_INTTK_PLUS = insertIntoRule(Rule20TK_INTTK_PLUS, eps, true);
    G[2] = insertRuleList(G[2], Rule20TK_INTTK_PLUS);

    G[3] = NULL;
    g_RHS *RuleTK_INT8TK_ENDIF2 = NULL;
    RuleTK_INT8TK_ENDIF2 = insertIntoRule(RuleTK_INT8TK_ENDIF2, TK_END, true);
    RuleTK_INT8TK_ENDIF2 = insertIntoRule(RuleTK_INT8TK_ENDIF2, 11, false);
    RuleTK_INT8TK_ENDIF2 = insertIntoRule(RuleTK_INT8TK_ENDIF2, TK_SEM, true);
    RuleTK_INT8TK_ENDIF2 = insertIntoRule(RuleTK_INT8TK_ENDIF2, 5, false);
    RuleTK_INT8TK_ENDIF2 = insertIntoRule(RuleTK_INT8TK_ENDIF2, 4, false);
    RuleTK_INT8TK_ENDIF2 = insertIntoRule(RuleTK_INT8TK_ENDIF2, TK_FUNID, true);
    G[3] = insertRuleList(G[3], RuleTK_INT8TK_ENDIF2);

    G[4] = NULL;
    g_RHS *RuleTK_ENDWHILETK_LISTTK_DIV = NULL;
    RuleTK_ENDWHILETK_LISTTK_DIV = insertIntoRule(RuleTK_ENDWHILETK_LISTTK_DIV, TK_SQR, true);
    RuleTK_ENDWHILETK_LISTTK_DIV = insertIntoRule(RuleTK_ENDWHILETK_LISTTK_DIV, 6, false);
    RuleTK_ENDWHILETK_LISTTK_DIV = insertIntoRule(RuleTK_ENDWHILETK_LISTTK_DIV, TK_SQL, true);
    RuleTK_ENDWHILETK_LISTTK_DIV = insertIntoRule(RuleTK_ENDWHILETK_LISTTK_DIV, TK_LIST, true);
    RuleTK_ENDWHILETK_LISTTK_DIV = insertIntoRule(RuleTK_ENDWHILETK_LISTTK_DIV, TK_PARAMETER, true);
    RuleTK_ENDWHILETK_LISTTK_DIV = insertIntoRule(RuleTK_ENDWHILETK_LISTTK_DIV, TK_INPUT, true);
    G[4] = insertRuleList(G[4], RuleTK_ENDWHILETK_LISTTK_DIV);

    G[5] = NULL;
    g_RHS *Rule6TK_NUM8TK_WRITE = NULL;
    Rule6TK_NUM8TK_WRITE = insertIntoRule(Rule6TK_NUM8TK_WRITE, TK_SQR, true);
    Rule6TK_NUM8TK_WRITE = insertIntoRule(Rule6TK_NUM8TK_WRITE, 6, false);
    Rule6TK_NUM8TK_WRITE = insertIntoRule(Rule6TK_NUM8TK_WRITE, TK_SQL, true);
    Rule6TK_NUM8TK_WRITE = insertIntoRule(Rule6TK_NUM8TK_WRITE, TK_LIST, true);
    Rule6TK_NUM8TK_WRITE = insertIntoRule(Rule6TK_NUM8TK_WRITE, TK_PARAMETER, true);
    Rule6TK_NUM8TK_WRITE = insertIntoRule(Rule6TK_NUM8TK_WRITE, TK_OUTPUT, true);
    G[5] = insertRuleList(G[5], Rule6TK_NUM8TK_WRITE);

    g_RHS *RuleTK_PARAMETERS8TK_CL = NULL;
    RuleTK_PARAMETERS8TK_CL = insertIntoRule(RuleTK_PARAMETERS8TK_CL, eps, true);
    G[5] = insertRuleList(G[5], RuleTK_PARAMETERS8TK_CL);

    G[6] = NULL;
    g_RHS *RuleTK_SQL2TK_SQR8 = NULL;
    RuleTK_SQL2TK_SQR8 = insertIntoRule(RuleTK_SQL2TK_SQR8, 10, false);
    RuleTK_SQL2TK_SQR8 = insertIntoRule(RuleTK_SQL2TK_SQR8, TK_ID, true);
    RuleTK_SQL2TK_SQR8 = insertIntoRule(RuleTK_SQL2TK_SQR8, 7, false);
    G[6] = insertRuleList(G[6], RuleTK_SQL2TK_SQR8);

    G[7] = NULL;
    g_RHS *Rule50TK_READ00 = NULL;
    Rule50TK_READ00 = insertIntoRule(Rule50TK_READ00, 8, false);
    G[7] = insertRuleList(G[7], Rule50TK_READ00);

    g_RHS *RuleTK_ENDRECORDTK_MULTK_DIV = NULL;
    RuleTK_ENDRECORDTK_MULTK_DIV = insertIntoRule(RuleTK_ENDRECORDTK_MULTK_DIV, 9, false);
    G[7] = insertRuleList(G[7], RuleTK_ENDRECORDTK_MULTK_DIV);

    G[8] = NULL;
    g_RHS *Rule38TK_COMMENTTK_WITH = NULL;
    Rule38TK_COMMENTTK_WITH = insertIntoRule(Rule38TK_COMMENTTK_WITH, TK_INT, true);
    G[8] = insertRuleList(G[8], Rule38TK_COMMENTTK_WITH);

    g_RHS *Rule1126TK_MAIN = NULL;
    Rule1126TK_MAIN = insertIntoRule(Rule1126TK_MAIN, TK_REAL, true);
    G[8] = insertRuleList(G[8], Rule1126TK_MAIN);

    G[9] = NULL;
    g_RHS *Rule2TK_ELSE0TK_GLOBAL = NULL;
    Rule2TK_ELSE0TK_GLOBAL = insertIntoRule(Rule2TK_ELSE0TK_GLOBAL, TK_RUID, true);
    Rule2TK_ELSE0TK_GLOBAL = insertIntoRule(Rule2TK_ELSE0TK_GLOBAL, TK_RECORD, true);
    G[9] = insertRuleList(G[9], Rule2TK_ELSE0TK_GLOBAL);

    g_RHS *RuleTK_MUL8TK_IF9 = NULL;
    RuleTK_MUL8TK_IF9 = insertIntoRule(RuleTK_MUL8TK_IF9, TK_RUID, true);
    RuleTK_MUL8TK_IF9 = insertIntoRule(RuleTK_MUL8TK_IF9, TK_UNION, true);
    G[9] = insertRuleList(G[9], RuleTK_MUL8TK_IF9);

    g_RHS *RuleTK_IDTK_PARAMETERTK_ASSIGNOP = NULL;
    RuleTK_IDTK_PARAMETERTK_ASSIGNOP = insertIntoRule(RuleTK_IDTK_PARAMETERTK_ASSIGNOP, TK_RUID, true);
    G[9] = insertRuleList(G[9], RuleTK_IDTK_PARAMETERTK_ASSIGNOP);

    G[10] = NULL;
    g_RHS *RuleTK_LETK_CLTK_PLUS = NULL;
    RuleTK_LETK_CLTK_PLUS = insertIntoRule(RuleTK_LETK_CLTK_PLUS, 6, false);
    RuleTK_LETK_CLTK_PLUS = insertIntoRule(RuleTK_LETK_CLTK_PLUS, TK_COMMA, true);
    G[10] = insertRuleList(G[10], RuleTK_LETK_CLTK_PLUS);

    g_RHS *Rule1TK_PARAMETER8TK_PLUS = NULL;
    Rule1TK_PARAMETER8TK_PLUS = insertIntoRule(Rule1TK_PARAMETER8TK_PLUS, eps, true);
    G[10] = insertRuleList(G[10], Rule1TK_PARAMETER8TK_PLUS);

    G[11] = NULL;
    g_RHS *RuleTK_FUNID3TK_COMMA3 = NULL;
    RuleTK_FUNID3TK_COMMA3 = insertIntoRule(RuleTK_FUNID3TK_COMMA3, 47, false);
    RuleTK_FUNID3TK_COMMA3 = insertIntoRule(RuleTK_FUNID3TK_COMMA3, 20, false);
    RuleTK_FUNID3TK_COMMA3 = insertIntoRule(RuleTK_FUNID3TK_COMMA3, 17, false);
    RuleTK_FUNID3TK_COMMA3 = insertIntoRule(RuleTK_FUNID3TK_COMMA3, 12, false);
    G[11] = insertRuleList(G[11], RuleTK_FUNID3TK_COMMA3);

    G[12] = NULL;
    g_RHS *Rule7TK_NUM1TK_CL = NULL;
    Rule7TK_NUM1TK_CL = insertIntoRule(Rule7TK_NUM1TK_CL, 12, false);
    Rule7TK_NUM1TK_CL = insertIntoRule(Rule7TK_NUM1TK_CL, 24, false);
    G[12] = insertRuleList(G[12], Rule7TK_NUM1TK_CL);

    g_RHS *Rule4TK_ASSIGNOP6TK_END = NULL;
    Rule4TK_ASSIGNOP6TK_END = insertIntoRule(Rule4TK_ASSIGNOP6TK_END, eps, true);
    G[12] = insertRuleList(G[12], Rule4TK_ASSIGNOP6TK_END);

    G[24] = NULL;
    g_RHS *RuleTK_TYPE02TK_READ = NULL;
    RuleTK_TYPE02TK_READ = insertIntoRule(RuleTK_TYPE02TK_READ, 13, false);
    G[24] = insertRuleList(G[24], RuleTK_TYPE02TK_READ);

    g_RHS *RuleTK_CALL6TK_MAIN1 = NULL;
    RuleTK_CALL6TK_MAIN1 = insertIntoRule(RuleTK_CALL6TK_MAIN1, definetypestmt, false);
    G[24] = insertRuleList(G[24], RuleTK_CALL6TK_MAIN1);

    G[13] = NULL;
    g_RHS *Rule9TK_FUNIDTK_PARAMETERS5 = NULL;
    Rule9TK_FUNIDTK_PARAMETERS5 = insertIntoRule(Rule9TK_FUNIDTK_PARAMETERS5, TK_ENDRECORD, true);
    Rule9TK_FUNIDTK_PARAMETERS5 = insertIntoRule(Rule9TK_FUNIDTK_PARAMETERS5, 14, false);
    Rule9TK_FUNIDTK_PARAMETERS5 = insertIntoRule(Rule9TK_FUNIDTK_PARAMETERS5, TK_RUID, true);
    Rule9TK_FUNIDTK_PARAMETERS5 = insertIntoRule(Rule9TK_FUNIDTK_PARAMETERS5, TK_RECORD, true);
    G[13] = insertRuleList(G[13], Rule9TK_FUNIDTK_PARAMETERS5);

    g_RHS *Rule24TK_CLTK_NOT = NULL;
    Rule24TK_CLTK_NOT = insertIntoRule(Rule24TK_CLTK_NOT, TK_ENDUNION, true);
    Rule24TK_CLTK_NOT = insertIntoRule(Rule24TK_CLTK_NOT, 14, false);
    Rule24TK_CLTK_NOT = insertIntoRule(Rule24TK_CLTK_NOT, TK_RUID, true);
    Rule24TK_CLTK_NOT = insertIntoRule(Rule24TK_CLTK_NOT, TK_UNION, true);
    G[13] = insertRuleList(G[13], Rule24TK_CLTK_NOT);

    G[14] = NULL;
    g_RHS *RuleTK_ORTK_ASSIGNOP34 = NULL;
    RuleTK_ORTK_ASSIGNOP34 = insertIntoRule(RuleTK_ORTK_ASSIGNOP34, 16, false);
    RuleTK_ORTK_ASSIGNOP34 = insertIntoRule(RuleTK_ORTK_ASSIGNOP34, 15, false);
    RuleTK_ORTK_ASSIGNOP34 = insertIntoRule(RuleTK_ORTK_ASSIGNOP34, 15, false);
    G[14] = insertRuleList(G[14], RuleTK_ORTK_ASSIGNOP34);

    G[15] = NULL;
    g_RHS *Rule42TK_REAL9 = NULL;
    Rule42TK_REAL9 = insertIntoRule(Rule42TK_REAL9, TK_SEM, true);
    Rule42TK_REAL9 = insertIntoRule(Rule42TK_REAL9, TK_FIELDID, true);
    Rule42TK_REAL9 = insertIntoRule(Rule42TK_REAL9, TK_COLON, true);
    Rule42TK_REAL9 = insertIntoRule(Rule42TK_REAL9, 32, false);
    Rule42TK_REAL9 = insertIntoRule(Rule42TK_REAL9, TK_TYPE, true);
    G[15] = insertRuleList(G[15], Rule42TK_REAL9);

    G[32] = NULL;
    g_RHS *Rule9TK_RNUM2TK_CL = NULL;
    Rule9TK_RNUM2TK_CL = insertIntoRule(Rule9TK_RNUM2TK_CL, 8, false);
    G[32] = insertRuleList(G[32], Rule9TK_RNUM2TK_CL);

    g_RHS *RuleTK_READ30TK_ENDIF = NULL;
    RuleTK_READ30TK_ENDIF = insertIntoRule(RuleTK_READ30TK_ENDIF, TK_RUID, true);
    G[32] = insertRuleList(G[32], RuleTK_READ30TK_ENDIF);

    G[16] = NULL;
    g_RHS *RuleTK_AS043 = NULL;
    RuleTK_AS043 = insertIntoRule(RuleTK_AS043, 16, false);
    RuleTK_AS043 = insertIntoRule(RuleTK_AS043, 15, false);
    G[16] = insertRuleList(G[16], RuleTK_AS043);

    g_RHS *Rule43TK_ENDWHILETK_WITH = NULL;
    Rule43TK_ENDWHILETK_WITH = insertIntoRule(Rule43TK_ENDWHILETK_WITH, eps, true);
    G[16] = insertRuleList(G[16], Rule43TK_ENDWHILETK_WITH);

    G[17] = NULL;
    g_RHS *RuleepsTK_PLUSTK_PARAMETER = NULL;
    RuleepsTK_PLUSTK_PARAMETER = insertIntoRule(RuleepsTK_PLUSTK_PARAMETER, 17, false);
    RuleepsTK_PLUSTK_PARAMETER = insertIntoRule(RuleepsTK_PLUSTK_PARAMETER, 18, false);
    G[17] = insertRuleList(G[17], RuleepsTK_PLUSTK_PARAMETER);

    g_RHS *Rule33TK_WHILETK_END = NULL;
    Rule33TK_WHILETK_END = insertIntoRule(Rule33TK_WHILETK_END, eps, true);
    G[17] = insertRuleList(G[17], Rule33TK_WHILETK_END);

    G[18] = NULL;
    g_RHS *Rule9TK_INTTK_RNUM3 = NULL;
    Rule9TK_INTTK_RNUM3 = insertIntoRule(Rule9TK_INTTK_RNUM3, TK_SEM, true);
    Rule9TK_INTTK_RNUM3 = insertIntoRule(Rule9TK_INTTK_RNUM3, 19, false);
    Rule9TK_INTTK_RNUM3 = insertIntoRule(Rule9TK_INTTK_RNUM3, TK_ID, true);
    Rule9TK_INTTK_RNUM3 = insertIntoRule(Rule9TK_INTTK_RNUM3, TK_COLON, true);
    Rule9TK_INTTK_RNUM3 = insertIntoRule(Rule9TK_INTTK_RNUM3, 7, false);
    Rule9TK_INTTK_RNUM3 = insertIntoRule(Rule9TK_INTTK_RNUM3, TK_TYPE, true);
    G[18] = insertRuleList(G[18], Rule9TK_INTTK_RNUM3);

    G[19] = NULL;
    g_RHS *Rule4eps1TK_THEN = NULL;
    Rule4eps1TK_THEN = insertIntoRule(Rule4eps1TK_THEN, TK_GLOBAL, true);
    Rule4eps1TK_THEN = insertIntoRule(Rule4eps1TK_THEN, TK_COLON, true);
    G[19] = insertRuleList(G[19], Rule4eps1TK_THEN);

    g_RHS *RuleTK_FIELDIDTK_DOTTK_WHILE = NULL;
    RuleTK_FIELDIDTK_DOTTK_WHILE = insertIntoRule(RuleTK_FIELDIDTK_DOTTK_WHILE, eps, true);
    G[19] = insertRuleList(G[19], RuleTK_FIELDIDTK_DOTTK_WHILE);

    G[20] = NULL;
    g_RHS *Rule21TK_PARAMETERS42 = NULL;
    Rule21TK_PARAMETERS42 = insertIntoRule(Rule21TK_PARAMETERS42, 20, false);
    Rule21TK_PARAMETERS42 = insertIntoRule(Rule21TK_PARAMETERS42, 21, false);
    G[20] = insertRuleList(G[20], Rule21TK_PARAMETERS42);

    g_RHS *Rule48TK_IDTK_OUTPUT = NULL;
    Rule48TK_IDTK_OUTPUT = insertIntoRule(Rule48TK_IDTK_OUTPUT, eps, true);
    G[20] = insertRuleList(G[20], Rule48TK_IDTK_OUTPUT);

    G[21] = NULL;
    g_RHS *Rule42TK_COMMENTTK_NOT = NULL;
    Rule42TK_COMMENTTK_NOT = insertIntoRule(Rule42TK_COMMENTTK_NOT, 22, false);
    G[21] = insertRuleList(G[21], Rule42TK_COMMENTTK_NOT);

    g_RHS *RuleTK_OReps49 = NULL;
    RuleTK_OReps49 = insertIntoRule(RuleTK_OReps49, 28, false);
    G[21] = insertRuleList(G[21], RuleTK_OReps49);

    g_RHS *RuleepsTK_FIELDIDTK_END = NULL;
    RuleepsTK_FIELDIDTK_END = insertIntoRule(RuleepsTK_FIELDIDTK_END, 29, false);
    G[21] = insertRuleList(G[21], RuleepsTK_FIELDIDTK_END);

    g_RHS *Rule8TK_LISTTK_PARAMETERS4 = NULL;
    Rule8TK_LISTTK_PARAMETERS4 = insertIntoRule(Rule8TK_LISTTK_PARAMETERS4, 31, false);
    G[21] = insertRuleList(G[21], Rule8TK_LISTTK_PARAMETERS4);

    g_RHS *RuleTK_ENDWHILETK_CLTK_NUM = NULL;
    RuleTK_ENDWHILETK_CLTK_NUM = insertIntoRule(RuleTK_ENDWHILETK_CLTK_NUM, 25, false);
    G[21] = insertRuleList(G[21], RuleTK_ENDWHILETK_CLTK_NUM);

    G[22] = NULL;
    g_RHS *Rule8TK_FIELDID326 = NULL;
    Rule8TK_FIELDID326 = insertIntoRule(Rule8TK_FIELDID326, TK_SEM, true);
    Rule8TK_FIELDID326 = insertIntoRule(Rule8TK_FIELDID326, 34, false);
    Rule8TK_FIELDID326 = insertIntoRule(Rule8TK_FIELDID326, TK_ASSIGNOP, true);
    Rule8TK_FIELDID326 = insertIntoRule(Rule8TK_FIELDID326, 23, false);
    G[22] = insertRuleList(G[22], Rule8TK_FIELDID326);

    G[23] = NULL;
    g_RHS *RuleTK_LETK_SQL35 = NULL;
    RuleTK_LETK_SQL35 = insertIntoRule(RuleTK_LETK_SQL35, 33, false);
    RuleTK_LETK_SQL35 = insertIntoRule(RuleTK_LETK_SQL35, TK_ID, true);
    G[23] = insertRuleList(G[23], RuleTK_LETK_SQL35);

    G[33] = NULL;
    g_RHS *Rule49TK = NULL;
    Rule49TK = insertIntoRule(Rule49TK, eps, true);
    G[33] = insertRuleList(G[33], Rule49TK);

    g_RHS *RuleTK_FIELDID01TK_MUL = NULL;
    RuleTK_FIELDID01TK_MUL = insertIntoRule(RuleTK_FIELDID01TK_MUL, 42, false);
    RuleTK_FIELDID01TK_MUL = insertIntoRule(RuleTK_FIELDID01TK_MUL, 41, false);
    G[33] = insertRuleList(G[33], RuleTK_FIELDID01TK_MUL);

    G[41] = NULL;
    g_RHS *Rule3148TK_ASSIGNOP = NULL;
    Rule3148TK_ASSIGNOP = insertIntoRule(Rule3148TK_ASSIGNOP, TK_FIELDID, true);
    Rule3148TK_ASSIGNOP = insertIntoRule(Rule3148TK_ASSIGNOP, TK_DOT, true);
    G[41] = insertRuleList(G[41], Rule3148TK_ASSIGNOP);

    G[42] = NULL;
    g_RHS *Rule2TK_MINUSTK_MUL2 = NULL;
    Rule2TK_MINUSTK_MUL2 = insertIntoRule(Rule2TK_MINUSTK_MUL2, 42, false);
    Rule2TK_MINUSTK_MUL2 = insertIntoRule(Rule2TK_MINUSTK_MUL2, 41, false);
    G[42] = insertRuleList(G[42], Rule2TK_MINUSTK_MUL2);

    g_RHS *RuleTK_INPUTTK_DOT = NULL;
    RuleTK_INPUTTK_DOT = insertIntoRule(RuleTK_INPUTTK_DOT, eps, true);
    G[42] = insertRuleList(G[42], RuleTK_INPUTTK_DOT);

    G[25] = NULL;
    g_RHS *Rule3TK_IFTK_OUTPUT9 = NULL;
    Rule3TK_IFTK_OUTPUT9 = insertIntoRule(Rule3TK_IFTK_OUTPUT9, TK_SEM, true);
    Rule3TK_IFTK_OUTPUT9 = insertIntoRule(Rule3TK_IFTK_OUTPUT9, 27, false);
    Rule3TK_IFTK_OUTPUT9 = insertIntoRule(Rule3TK_IFTK_OUTPUT9, TK_PARAMETERS, true);
    Rule3TK_IFTK_OUTPUT9 = insertIntoRule(Rule3TK_IFTK_OUTPUT9, TK_WITH, true);
    Rule3TK_IFTK_OUTPUT9 = insertIntoRule(Rule3TK_IFTK_OUTPUT9, TK_FUNID, true);
    Rule3TK_IFTK_OUTPUT9 = insertIntoRule(Rule3TK_IFTK_OUTPUT9, TK_CALL, true);
    Rule3TK_IFTK_OUTPUT9 = insertIntoRule(Rule3TK_IFTK_OUTPUT9, 26, false);
    G[25] = insertRuleList(G[25], Rule3TK_IFTK_OUTPUT9);

    G[26] = NULL;
    g_RHS *Rule37TK_COMMA16 = NULL;
    Rule37TK_COMMA16 = insertIntoRule(Rule37TK_COMMA16, TK_ASSIGNOP, true);
    Rule37TK_COMMA16 = insertIntoRule(Rule37TK_COMMA16, TK_SQR, true);
    Rule37TK_COMMA16 = insertIntoRule(Rule37TK_COMMA16, 49, false);
    Rule37TK_COMMA16 = insertIntoRule(Rule37TK_COMMA16, TK_SQL, true);
    G[26] = insertRuleList(G[26], Rule37TK_COMMA16);

    g_RHS *Rule4TK_ENDWHILE0TK_OP = NULL;
    Rule4TK_ENDWHILE0TK_OP = insertIntoRule(Rule4TK_ENDWHILE0TK_OP, eps, true);
    G[26] = insertRuleList(G[26], Rule4TK_ENDWHILE0TK_OP);

    G[27] = NULL;
    g_RHS *RuleTK_READ3TK_DIV1 = NULL;
    RuleTK_READ3TK_DIV1 = insertIntoRule(RuleTK_READ3TK_DIV1, TK_SQR, true);
    RuleTK_READ3TK_DIV1 = insertIntoRule(RuleTK_READ3TK_DIV1, 49, false);
    RuleTK_READ3TK_DIV1 = insertIntoRule(RuleTK_READ3TK_DIV1, TK_SQL, true);
    G[27] = insertRuleList(G[27], RuleTK_READ3TK_DIV1);

    G[28] = NULL;
    g_RHS *Rule30TK_OP32 = NULL;
    Rule30TK_OP32 = insertIntoRule(Rule30TK_OP32, TK_ENDWHILE, true);
    Rule30TK_OP32 = insertIntoRule(Rule30TK_OP32, 20, false);
    Rule30TK_OP32 = insertIntoRule(Rule30TK_OP32, 21, false);
    Rule30TK_OP32 = insertIntoRule(Rule30TK_OP32, TK_CL, true);
    Rule30TK_OP32 = insertIntoRule(Rule30TK_OP32, 43, false);
    Rule30TK_OP32 = insertIntoRule(Rule30TK_OP32, TK_OP, true);
    Rule30TK_OP32 = insertIntoRule(Rule30TK_OP32, TK_WHILE, true);
    G[28] = insertRuleList(G[28], Rule30TK_OP32);

    G[29] = NULL;
    g_RHS *Rule2TK_CL4TK_REAL = NULL;
    Rule2TK_CL4TK_REAL = insertIntoRule(Rule2TK_CL4TK_REAL, 30, false);
    Rule2TK_CL4TK_REAL = insertIntoRule(Rule2TK_CL4TK_REAL, 20, false);
    Rule2TK_CL4TK_REAL = insertIntoRule(Rule2TK_CL4TK_REAL, 21, false);
    Rule2TK_CL4TK_REAL = insertIntoRule(Rule2TK_CL4TK_REAL, TK_THEN, true);
    Rule2TK_CL4TK_REAL = insertIntoRule(Rule2TK_CL4TK_REAL, TK_CL, true);
    Rule2TK_CL4TK_REAL = insertIntoRule(Rule2TK_CL4TK_REAL, 43, false);
    Rule2TK_CL4TK_REAL = insertIntoRule(Rule2TK_CL4TK_REAL, TK_OP, true);
    Rule2TK_CL4TK_REAL = insertIntoRule(Rule2TK_CL4TK_REAL, TK_IF, true);
    G[29] = insertRuleList(G[29], Rule2TK_CL4TK_REAL);

    G[30] = NULL;
    g_RHS *RuleepsTK_ENDTK_PARAMETER = NULL;
    RuleepsTK_ENDTK_PARAMETER = insertIntoRule(RuleepsTK_ENDTK_PARAMETER, TK_ENDIF, true);
    RuleepsTK_ENDTK_PARAMETER = insertIntoRule(RuleepsTK_ENDTK_PARAMETER, 20, false);
    RuleepsTK_ENDTK_PARAMETER = insertIntoRule(RuleepsTK_ENDTK_PARAMETER, 21, false);
    RuleepsTK_ENDTK_PARAMETER = insertIntoRule(RuleepsTK_ENDTK_PARAMETER, TK_ELSE, true);
    G[30] = insertRuleList(G[30], RuleepsTK_ENDTK_PARAMETER);

    g_RHS *Rule1TK_DIV139 = NULL;
    Rule1TK_DIV139 = insertIntoRule(Rule1TK_DIV139, TK_ENDIF, true);
    G[30] = insertRuleList(G[30], Rule1TK_DIV139);

    G[31] = NULL;
    g_RHS *RuleTK_MINUSTK_INT36 = NULL;
    RuleTK_MINUSTK_INT36 = insertIntoRule(RuleTK_MINUSTK_INT36, TK_SEM, true);
    RuleTK_MINUSTK_INT36 = insertIntoRule(RuleTK_MINUSTK_INT36, TK_CL, true);
    RuleTK_MINUSTK_INT36 = insertIntoRule(RuleTK_MINUSTK_INT36, 44, false);
    RuleTK_MINUSTK_INT36 = insertIntoRule(RuleTK_MINUSTK_INT36, TK_OP, true);
    RuleTK_MINUSTK_INT36 = insertIntoRule(RuleTK_MINUSTK_INT36, TK_READ, true);
    G[31] = insertRuleList(G[31], RuleTK_MINUSTK_INT36);

    g_RHS *Rule38TK_TYPETK_MINUS = NULL;
    Rule38TK_TYPETK_MINUS = insertIntoRule(Rule38TK_TYPETK_MINUS, TK_SEM, true);
    Rule38TK_TYPETK_MINUS = insertIntoRule(Rule38TK_TYPETK_MINUS, TK_CL, true);
    Rule38TK_TYPETK_MINUS = insertIntoRule(Rule38TK_TYPETK_MINUS, 44, false);
    Rule38TK_TYPETK_MINUS = insertIntoRule(Rule38TK_TYPETK_MINUS, TK_OP, true);
    Rule38TK_TYPETK_MINUS = insertIntoRule(Rule38TK_TYPETK_MINUS, TK_WRITE, true);
    G[31] = insertRuleList(G[31], Rule38TK_TYPETK_MINUS);

    G[34] = NULL;
    g_RHS *RuleTK_ELSETK_ENDWHILE20 = NULL;
    RuleTK_ELSETK_ENDWHILE20 = insertIntoRule(RuleTK_ELSETK_ENDWHILE20, 35, false);
    RuleTK_ELSETK_ENDWHILE20 = insertIntoRule(RuleTK_ELSETK_ENDWHILE20, 36, false);
    G[34] = insertRuleList(G[34], RuleTK_ELSETK_ENDWHILE20);

    G[35] = NULL;
    g_RHS *Rule6TK_FIELDID9TK_READ = NULL;
    Rule6TK_FIELDID9TK_READ = insertIntoRule(Rule6TK_FIELDID9TK_READ, 35, false);
    Rule6TK_FIELDID9TK_READ = insertIntoRule(Rule6TK_FIELDID9TK_READ, 36, false);
    Rule6TK_FIELDID9TK_READ = insertIntoRule(Rule6TK_FIELDID9TK_READ, 40, false);
    G[35] = insertRuleList(G[35], Rule6TK_FIELDID9TK_READ);

    g_RHS *RuleTK_FUNID7TK_PARAMETER9 = NULL;
    RuleTK_FUNID7TK_PARAMETER9 = insertIntoRule(RuleTK_FUNID7TK_PARAMETER9, eps, true);
    G[35] = insertRuleList(G[35], RuleTK_FUNID7TK_PARAMETER9);

    G[36] = NULL;
    g_RHS *Rule237TK_WHILE = NULL;
    Rule237TK_WHILE = insertIntoRule(Rule237TK_WHILE, 37, false);
    Rule237TK_WHILE = insertIntoRule(Rule237TK_WHILE, 38, false);
    G[36] = insertRuleList(G[36], Rule237TK_WHILE);

    G[37] = NULL;
    g_RHS *Rule8TK_READTK_INPUT8 = NULL;
    Rule8TK_READTK_INPUT8 = insertIntoRule(Rule8TK_READTK_INPUT8, 37, false);
    Rule8TK_READTK_INPUT8 = insertIntoRule(Rule8TK_READTK_INPUT8, 38, false);
    Rule8TK_READTK_INPUT8 = insertIntoRule(Rule8TK_READTK_INPUT8, 39, false);
    G[37] = insertRuleList(G[37], Rule8TK_READTK_INPUT8);

    g_RHS *RuleTK_ENDRECORD4TK_ELSE2 = NULL;
    RuleTK_ENDRECORD4TK_ELSE2 = insertIntoRule(RuleTK_ENDRECORD4TK_ELSE2, eps, true);
    G[37] = insertRuleList(G[37], RuleTK_ENDRECORD4TK_ELSE2);

    G[38] = NULL;
    g_RHS *Rule4TK_FUNIDTK_DEFINETYPE = NULL;
    Rule4TK_FUNIDTK_DEFINETYPE = insertIntoRule(Rule4TK_FUNIDTK_DEFINETYPE, TK_CL, true);
    Rule4TK_FUNIDTK_DEFINETYPE = insertIntoRule(Rule4TK_FUNIDTK_DEFINETYPE, 34, false);
    Rule4TK_FUNIDTK_DEFINETYPE = insertIntoRule(Rule4TK_FUNIDTK_DEFINETYPE, TK_OP, true);
    G[38] = insertRuleList(G[38], Rule4TK_FUNIDTK_DEFINETYPE);

    g_RHS *RuleTK_FUNID2TK_END3 = NULL;
    RuleTK_FUNID2TK_END3 = insertIntoRule(RuleTK_FUNID2TK_END3, 44, false);
    G[38] = insertRuleList(G[38], RuleTK_FUNID2TK_END3);

    G[39] = NULL;
    g_RHS *Rule8TK_NUM7TK_END = NULL;
    Rule8TK_NUM7TK_END = insertIntoRule(Rule8TK_NUM7TK_END, TK_MUL, true);
    G[39] = insertRuleList(G[39], Rule8TK_NUM7TK_END);

    g_RHS *Rule12TK_THEN28 = NULL;
    Rule12TK_THEN28 = insertIntoRule(Rule12TK_THEN28, TK_DIV, true);
    G[39] = insertRuleList(G[39], Rule12TK_THEN28);

    G[40] = NULL;
    g_RHS *Rule1TK_MULTK_ENDIF2 = NULL;
    Rule1TK_MULTK_ENDIF2 = insertIntoRule(Rule1TK_MULTK_ENDIF2, TK_PLUS, true);
    G[40] = insertRuleList(G[40], Rule1TK_MULTK_ENDIF2);

    g_RHS *Rule2TK_PARAMETERS401 = NULL;
    Rule2TK_PARAMETERS401 = insertIntoRule(Rule2TK_PARAMETERS401, TK_MINUS, true);
    G[40] = insertRuleList(G[40], Rule2TK_PARAMETERS401);

    G[43] = NULL;
    g_RHS *Rule1TK_NUMTK_PARAMETERS8 = NULL;
    Rule1TK_NUMTK_PARAMETERS8 = insertIntoRule(Rule1TK_NUMTK_PARAMETERS8, TK_CL, true);
    Rule1TK_NUMTK_PARAMETERS8 = insertIntoRule(Rule1TK_NUMTK_PARAMETERS8, 43, false);
    Rule1TK_NUMTK_PARAMETERS8 = insertIntoRule(Rule1TK_NUMTK_PARAMETERS8, TK_OP, true);
    Rule1TK_NUMTK_PARAMETERS8 = insertIntoRule(Rule1TK_NUMTK_PARAMETERS8, 45, false);
    Rule1TK_NUMTK_PARAMETERS8 = insertIntoRule(Rule1TK_NUMTK_PARAMETERS8, TK_CL, true);
    Rule1TK_NUMTK_PARAMETERS8 = insertIntoRule(Rule1TK_NUMTK_PARAMETERS8, 43, false);
    Rule1TK_NUMTK_PARAMETERS8 = insertIntoRule(Rule1TK_NUMTK_PARAMETERS8, TK_OP, true);
    G[43] = insertRuleList(G[43], Rule1TK_NUMTK_PARAMETERS8);

    g_RHS *Rule333TK_ENDRECORD4 = NULL;
    Rule333TK_ENDRECORD4 = insertIntoRule(Rule333TK_ENDRECORD4, 44, false);
    Rule333TK_ENDRECORD4 = insertIntoRule(Rule333TK_ENDRECORD4, 46, false);
    Rule333TK_ENDRECORD4 = insertIntoRule(Rule333TK_ENDRECORD4, 44, false);
    G[43] = insertRuleList(G[43], Rule333TK_ENDRECORD4);

    g_RHS *RuleTK_WRITE0TK_SQL = NULL;
    RuleTK_WRITE0TK_SQL = insertIntoRule(RuleTK_WRITE0TK_SQL, TK_CL, true);
    RuleTK_WRITE0TK_SQL = insertIntoRule(RuleTK_WRITE0TK_SQL, 43, false);
    RuleTK_WRITE0TK_SQL = insertIntoRule(RuleTK_WRITE0TK_SQL, TK_OP, true);
    RuleTK_WRITE0TK_SQL = insertIntoRule(RuleTK_WRITE0TK_SQL, TK_NOT, true);
    G[43] = insertRuleList(G[43], RuleTK_WRITE0TK_SQL);

    G[44] = NULL;
    g_RHS *Rule9TK_WITH415 = NULL;
    Rule9TK_WITH415 = insertIntoRule(Rule9TK_WITH415, 23, false);
    G[44] = insertRuleList(G[44], Rule9TK_WITH415);

    g_RHS *RuleTK_IDTK_NOTTK_INT = NULL;
    RuleTK_IDTK_NOTTK_INT = insertIntoRule(RuleTK_IDTK_NOTTK_INT, TK_NUM, true);
    G[44] = insertRuleList(G[44], RuleTK_IDTK_NOTTK_INT);

    g_RHS *Rule6TK_END9TK_COLON = NULL;
    Rule6TK_END9TK_COLON = insertIntoRule(Rule6TK_END9TK_COLON, TK_RNUM, true);
    G[44] = insertRuleList(G[44], Rule6TK_END9TK_COLON);

    G[45] = NULL;
    g_RHS *RuleTK_COMMA1429 = NULL;
    RuleTK_COMMA1429 = insertIntoRule(RuleTK_COMMA1429, TK_AND, true);
    G[45] = insertRuleList(G[45], RuleTK_COMMA1429);

    g_RHS *Rule9TK_SQRTK_THEN7 = NULL;
    Rule9TK_SQRTK_THEN7 = insertIntoRule(Rule9TK_SQRTK_THEN7, TK_OR, true);
    G[45] = insertRuleList(G[45], Rule9TK_SQRTK_THEN7);

    G[46] = NULL;
    g_RHS *Rule8eps0TK_OP = NULL;
    Rule8eps0TK_OP = insertIntoRule(Rule8eps0TK_OP, TK_LT, true);
    G[46] = insertRuleList(G[46], Rule8eps0TK_OP);

    g_RHS *RuleTK_CL4224 = NULL;
    RuleTK_CL4224 = insertIntoRule(RuleTK_CL4224, TK_LE, true);
    G[46] = insertRuleList(G[46], RuleTK_CL4224);

    g_RHS *Rule3245eps = NULL;
    Rule3245eps = insertIntoRule(Rule3245eps, TK_EQ, true);
    G[46] = insertRuleList(G[46], Rule3245eps);

    g_RHS *Rule23TK_DOTTK_END = NULL;
    Rule23TK_DOTTK_END = insertIntoRule(Rule23TK_DOTTK_END, TK_GT, true);
    G[46] = insertRuleList(G[46], Rule23TK_DOTTK_END);

    g_RHS *Rule41TK_SQL36 = NULL;
    Rule41TK_SQL36 = insertIntoRule(Rule41TK_SQL36, TK_GE, true);
    G[46] = insertRuleList(G[46], Rule41TK_SQL36);

    g_RHS *Rule8TK_INTTK_ENDRECORD3 = NULL;
    Rule8TK_INTTK_ENDRECORD3 = insertIntoRule(Rule8TK_INTTK_ENDRECORD3, TK_NE, true);
    G[46] = insertRuleList(G[46], Rule8TK_INTTK_ENDRECORD3);

    G[47] = NULL;
    g_RHS *Rule14TK_PARAMETERS10 = NULL;
    Rule14TK_PARAMETERS10 = insertIntoRule(Rule14TK_PARAMETERS10, TK_SEM, true);
    Rule14TK_PARAMETERS10 = insertIntoRule(Rule14TK_PARAMETERS10, 48, false);
    Rule14TK_PARAMETERS10 = insertIntoRule(Rule14TK_PARAMETERS10, TK_RETURN, true);
    G[47] = insertRuleList(G[47], Rule14TK_PARAMETERS10);

    G[48] = NULL;
    g_RHS *Rule214012 = NULL;
    Rule214012 = insertIntoRule(Rule214012, TK_SQR, true);
    Rule214012 = insertIntoRule(Rule214012, 49, false);
    Rule214012 = insertIntoRule(Rule214012, TK_SQL, true);
    G[48] = insertRuleList(G[48], Rule214012);

    g_RHS *Rule2TK_INTTK_LIST7 = NULL;
    Rule2TK_INTTK_LIST7 = insertIntoRule(Rule2TK_INTTK_LIST7, eps, true);
    G[48] = insertRuleList(G[48], Rule2TK_INTTK_LIST7);

    G[49] = NULL;
    g_RHS *Rule29TK_ELSE35 = NULL;
    Rule29TK_ELSE35 = insertIntoRule(Rule29TK_ELSE35, 50, false);
    Rule29TK_ELSE35 = insertIntoRule(Rule29TK_ELSE35, TK_ID, true);
    G[49] = insertRuleList(G[49], Rule29TK_ELSE35);

    G[50] = NULL;
    g_RHS *RuleTK_MINUS9TK_RNUM8 = NULL;
    RuleTK_MINUS9TK_RNUM8 = insertIntoRule(RuleTK_MINUS9TK_RNUM8, 49, false);
    RuleTK_MINUS9TK_RNUM8 = insertIntoRule(RuleTK_MINUS9TK_RNUM8, TK_COMMA, true);
    G[50] = insertRuleList(G[50], RuleTK_MINUS9TK_RNUM8);

    g_RHS *Rule506TK_WITH3 = NULL;
    Rule506TK_WITH3 = insertIntoRule(Rule506TK_WITH3, eps, true);
    G[50] = insertRuleList(G[50], Rule506TK_WITH3);

    G[definetypestmt] = NULL; // definetypestmt
    g_RHS *Rule2TK_MINUSTK_FUNID5 = NULL;
    // Rule2TK_MINUSTK_FUNID5 = insertIntoRule(Rule2TK_MINUSTK_FUNID5,TK_SEM, true);
    Rule2TK_MINUSTK_FUNID5 = insertIntoRule(Rule2TK_MINUSTK_FUNID5, TK_RUID, true);
    Rule2TK_MINUSTK_FUNID5 = insertIntoRule(Rule2TK_MINUSTK_FUNID5, TK_AS, true);
    Rule2TK_MINUSTK_FUNID5 = insertIntoRule(Rule2TK_MINUSTK_FUNID5, TK_RUID, true);
    Rule2TK_MINUSTK_FUNID5 = insertIntoRule(Rule2TK_MINUSTK_FUNID5, A, false);
    Rule2TK_MINUSTK_FUNID5 = insertIntoRule(Rule2TK_MINUSTK_FUNID5, TK_DEFINETYPE, true);
    G[definetypestmt] = insertRuleList(G[definetypestmt], Rule2TK_MINUSTK_FUNID5);

    G[A] = NULL;
    g_RHS *Rule2042TK_CALL = NULL;
    Rule2042TK_CALL = insertIntoRule(Rule2042TK_CALL, TK_RECORD, true);
    G[A] = insertRuleList(G[A], Rule2042TK_CALL);

    g_RHS *Rule23TK_PARAMETERSTK_THEN = NULL;
    Rule23TK_PARAMETERSTK_THEN = insertIntoRule(Rule23TK_PARAMETERSTK_THEN, TK_UNION, true);
    G[A] = insertRuleList(G[A], Rule23TK_PARAMETERSTK_THEN);
}

void printRule(g_RHS *head)
{ // Print 1 single rule

    g_RHS *temp = head;
    while (temp != NULL)
    {
        // printf("A%d ", temp->symbol); // For number printing
        if (temp->isTerminal)
            printToken(temp->symbol);
        else
            printNonTerminal(temp->symbol);
        temp = temp->next;
    }
    printf("\n");
}

void printRules(Grammar G, int i, bool needArrow)
{ // Print All Rules for a NT
    ruleHead *temp = G[i];
    while (temp != NULL)
    {
        printNonTerminal(i);
        // printf("A%d ", i); // For number printing
        if (needArrow)
        {
            printf(" -> ");
        }
        printRule(temp->listHead);
        temp = temp->next;
    }
}

void printGrammar(Grammar G, int len)
{
    for (int i = 0; i < len; i++)
    {
        printRules(G, i, true);
    }
}

void printToken(int token)
{
    switch (token)
    {
    case eps:
        printf("<eps> ");
        break;
    case TK_ASSIGNOP:
        printf("TK_ASSIGNOP ");
        break;
    case TK_COMMENT:
        printf("TK_COMMENT ");
        break;
    case TK_FIELDID:
        printf("TK_FIELDID ");
        break;
    case TK_ID:
        printf("TK_ID ");
        break;
    case TK_NUM:
        printf("TK_NUM ");
        break;
    case TK_RNUM:
        printf("TK_RNUM ");
        break;
    case TK_FUNID:
        printf("TK_FUNID ");
        break;
    case TK_WITH:
        printf("TK_WITH ");
        break;
    case TK_PARAMETERS:
        printf("TK_PARAMETERS ");
        break;
    case TK_END:
        printf("TK_END ");
        break;
    case TK_WHILE:
        printf("TK_WHILE ");
        break;
    case TK_TYPE:
        printf("TK_TYPE ");
        break;
    case TK_MAIN:
        printf("TK_MAIN ");
        break;
    case TK_GLOBAL:
        printf("TK_GLOBAL ");
        break;
    case TK_PARAMETER:
        printf("TK_PARAMETER ");
        break;
    case TK_LIST:
        printf("TK_LIST ");
        break;
    case TK_SQL:
        printf("TK_SQL ");
        break;
    case TK_SQR:
        printf("TK_SQR ");
        break;
    case TK_INPUT:
        printf("TK_INPUT ");
        break;
    case TK_OUTPUT:
        printf("TK_OUTPUT ");
        break;
    case TK_INT:
        printf("TK_INT ");
        break;
    case TK_REAL:
        printf("TK_REAL ");
        break;
    case TK_COMMA:
        printf("TK_COMMA ");
        break;
    case TK_SEM:
        printf("TK_SEM ");
        break;
    case TK_COLON:
        printf("TK_COLON ");
        break;
    case TK_DOT:
        printf("TK_DOT ");
        break;
    case TK_ENDWHILE:
        printf("TK_ENDWHILE ");
        break;
    case TK_OP:
        printf("TK_OP ");
        break;
    case TK_CL:
        printf("TK_CL ");
        break;
    case TK_IF:
        printf("TK_IF ");
        break;
    case TK_THEN:
        printf("TK_THEN ");
        break;
    case TK_ENDIF:
        printf("TK_ENDIF ");
        break;
    case TK_READ:
        printf("TK_READ ");
        break;
    case TK_WRITE:
        printf("TK_WRITE ");
        break;
    case TK_RETURN:
        printf("TK_RETURN ");
        break;
    case TK_PLUS:
        printf("TK_PLUS ");
        break;
    case TK_MINUS:
        printf("TK_MINUS ");
        break;
    case TK_MUL:
        printf("TK_MUL ");
        break;
    case TK_DIV:
        printf("TK_DIV ");
        break;
    case TK_CALL:
        printf("TK_CALL ");
        break;
    case TK_RECORD:
        printf("TK_RECORD ");
        break;
    case TK_ENDRECORD:
        printf("TK_ENDRECORD ");
        break;
    case TK_ELSE:
        printf("TK_ELSE ");
        break;
    case TK_AND:
        printf("TK_AND ");
        break;
    case TK_OR:
        printf("TK_OR ");
        break;
    case TK_NOT:
        printf("TK_NOT ");
        break;
    case TK_LT:
        printf("TK_LT ");
        break;
    case TK_LE:
        printf("TK_LE ");
        break;
    case TK_AS:
        printf("TK_AS ");
        break;
    case TK_EQ:
        printf("TK_EQ ");
        break;
    case TK_GT:
        printf("TK_GT ");
        break;
    case TK_GE:
        printf("TK_GE ");
        break;
    case TK_NE:
        printf("TK_NE ");
        break;
    case SENTINEL:
        printf("SENTINEL ");
        break;
    case TK_RUID:
        printf("TK_RUID ");
        break;
    case TK_UNION:
        printf("TK_UNION ");
        break;
    case TK_ENDUNION:
        printf("TK_ENDUNION ");
        break;
    case TK_DEFINETYPE:
        printf("TK_DEFINETYPE ");
        break;
    default:
        printf("<No Symbol found> ");
        break;
    }
}

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
        printf("<actualOrRedefined> "); // Changed to Support Grammar
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
        printf("<fieldtype> "); // Changed to Support Grammar
        break;
    case 33:
        printf("<option_single_constructed> "); // Changed to Support Grammar
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
        printf("<oneExpansion> "); // Changed to Support Grammar
        break;
    case 42:
        printf("<moreExpansions> "); // Changed to Support Grammar
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
    case definetypestmt:
        printf("<definetypestmt> ");
        break;
    case A:
        printf("<A> ");
        break;

    default:
        printf("<No Symbol found>");
        break;
    }
}

void printSymbol(int i){
    if(i>=eps){
        printToken(i);
    }
    else{
        printNonTerminal(i);
    }
}

// FOR TESTING PURPOSES

// int main(){
// 	initGrammar();
//     populateFirstFollow("Follow.txt", false);
//     printFollow();
// }
