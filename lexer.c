#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#define COUNT_KEYWORDS 28
#define BUFFER_SIZE 100

typedef enum grammar_term {
	program, mainFunction, otherFunctions, function, input_par, output_par,
	parameter_list, dataType, primitiveDatatype, constructedDatatype,
	remaining_list, stmts, typeDefinitions, typeDefinition, fieldDefinitions,
	fieldDefinition, moreFields, declarations, declaration, global_or_not,
	otherStmts, stmt, assignmentStmt, singleOrRecId, new_24, funCallStmt,
	outputParameters, inputParameters, iterativeStmt, conditionalStmt, elsePart,
	ioStmt, allVar, allVarTemp, arithmeticExpression, expPrime, term, termPrime, factor,
	highPrecedenceOperators, lowPrecedenceOperators, all, temp, booleanExpression,
	var, logicalOp, relationalOp, returnStmt, optionalReturn, idList, more_ids, eps,
	TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_WITH, TK_PARAMETERS, TK_END, TK_WHILE, TK_TYPE, TK_MAIN,
	TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT,
	TK_REAL, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF,
	TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV,
	TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_AS,
	TK_EQ, TK_GT, TK_GE, TK_NE, SENTINEL, LEX_ERROR, TK_RUID, TK_UNION, TK_ENDUNION, TK_DEFINETYPE
} gterm;

typedef struct keyword {
	char key[20];
	gterm value;
}keyword;

typedef struct token {
	gterm tid; 
	char* lexeme;
	void* numVal; //Int, Float values for numbers
	int lineNo;
} tokenInfo;



typedef struct twin {
	char buff1[BUFFER_SIZE + 1];
	char buff2[BUFFER_SIZE + 1];
} twinBuffer;

FILE* getStream(FILE* fp);

keyword keywords[] = {
	{"with",TK_WITH},
	{"parameters",TK_PARAMETERS},
	{"end",TK_END},
	{"while",TK_WHILE},
	{"union",TK_UNION},
	{"endunion",TK_ENDUNION},
	{"definetype",TK_DEFINETYPE},
	{"as",TK_AS},
	{"type",TK_TYPE},
	{"main",TK_MAIN},
	{"global",TK_GLOBAL},
	{"parameter",TK_PARAMETER},
	{"list",TK_LIST},
	{"input",TK_INPUT},
	{"int",TK_INT},
	{"real",TK_REAL},
	{"endwhile",TK_ENDWHILE},
	{"if",TK_IF},
	{"then",TK_THEN},
	{"endif",TK_ENDIF},
	{"read",TK_READ},
	{"write",TK_WRITE},
	{"return",TK_RETURN},
	{"call",TK_CALL},
	{"record",TK_RECORD},
	{"endrecord",TK_ENDRECORD},
	{"else",TK_ELSE}
};

twinBuffer buffers;
int line = 1;
//helps to keep track of the line number

bool file_end;
//tells us if we have reached the end of file

bool startedReading;
//tells us if we have started reading or not

char *lexemeBegin, *forward;
//two pointers for the twin buffer

int charCount;
//keeps track of the size of lexeme

bool readBuff1, readBuff2;
//tells us if both these buffers have been read or not

FILE *fp;

void reset(){
	lexemeBegin = forward;
	charCount = 0;
}

void closeLexer(FILE* fp) {
	int err = fclose(fp);
}

FILE* getStream(FILE *fp) {
	
	
	if(!startedReading) 
	{
	//start case
	//read into buff1 and set readBuff1 as true
		int size1 = fread(buffers.buff1, sizeof(char), BUFFER_SIZE, fp);
		buffers.buff1[size1] = EOF;
		lexemeBegin = buffers.buff1;
		forward = buffers.buff1;

		readBuff1 = true;
		readBuff2 = true;
		startedReading = true;
		//printf("check %c check",*forward);
	}
	
	else if(forward == buffers.buff1 + BUFFER_SIZE - 1)	{
		//forward is at the end of buffer1
		if(readBuff2) 
		{
			int size2 = fread(buffers.buff2, sizeof(char), BUFFER_SIZE, fp);
			buffers.buff2[size2] = EOF;

		}
		forward = buffers.buff2;
		readBuff2 = true;
	}

	else if (forward == buffers.buff2 + BUFFER_SIZE - 1) {
		//forward is at the end of buffer2
		if(readBuff1) 
		{
			int size1 = fread(buffers.buff1, sizeof(char), BUFFER_SIZE, fp);
			buffers.buff1[size1] = EOF;
		}
		forward = buffers.buff1;
		readBuff1 = true;
	}

	else {
		//normal case where we have space in the buffer
		forward++;
	}
	return fp;
}


char getNextChar(FILE *fp) {
	
	if(!startedReading)
		{char curr = *forward; ++charCount;  forward++; return curr;}
	
	//printf("buffer 1: %s buffer 2: %s forward: %c \n", buffers.buff1, buffers.buff2, *forward);

	char curr = *forward;
	fp = getStream(fp);	
	++charCount;
	//printf("\n check %c check\n",curr);
	return curr;
}

void retract(int n) {
	if (forward >= buffers.buff1 && forward < buffers.buff1 + BUFFER_SIZE) {
		if (forward - buffers.buff1 < n) {
			readBuff1 = false; // Don't load buff1 the next time
			n -= forward - buffers.buff1;
			forward = buffers.buff2 + BUFFER_SIZE - n;
		}
		else {
			forward -= n;
		}
	}
	else if (forward >= buffers.buff2 && forward < buffers.buff2 + BUFFER_SIZE){
		if (forward - buffers.buff2 < n) {
			readBuff2 = false; // Don't load buff2 the next time
			n -= forward - buffers.buff2;
			forward = buffers.buff1 + BUFFER_SIZE - n;
		}
		else {
			forward -= n;
		}
	}
	charCount -= n;
}

FILE* startLexer(char* inputFile) {
	//must be called in the driver file before starting the lexer
	//initialising buffers to 0
	
	memset(buffers.buff1, 0, sizeof(buffers.buff1));
	memset(buffers.buff2, 0, sizeof(buffers.buff2));

	line = 1;
	file_end = false;
	charCount = 0;
	readBuff1 = false;
	readBuff2 = false;
	lexemeBegin = NULL;
	forward = NULL;
	startedReading = false;

	FILE* fp = fopen(inputFile, "r");

	if(fp == NULL) {
		printf("Error opening file\n");
		return NULL;
	}

	else {
		printf("\n Successfully opened the code file\n");
	}

	
	int size1 = fread(buffers.buff1, sizeof(char), BUFFER_SIZE, fp);
	buffers.buff1[size1] = EOF;
	lexemeBegin = buffers.buff1;
	forward = buffers.buff1;
	readBuff1 = true;
	readBuff2 = true;
	startedReading = true;	
	//printf("\n checking %s", buffers.buff1);
	return fp;
	
}

char * getLexeme() {
	char *lexeme = (char *)malloc((charCount + 1) * sizeof(char));
	int i = 0;
	char *currptr = lexemeBegin;
	bool a = lexemeBegin >= buffers.buff1 && lexemeBegin < buffers.buff1 + BUFFER_SIZE;
	bool b = lexemeBegin >= buffers.buff2 && lexemeBegin < buffers.buff2 + BUFFER_SIZE;
	bool c = forward >= buffers.buff1 && forward < buffers.buff1 + BUFFER_SIZE;
	bool d = forward >= buffers.buff2 && forward < buffers.buff2 + BUFFER_SIZE;

	if((a&&c) || (b&&d))
	{
	while (currptr < forward) {
		lexeme[i] = *currptr;
		++i;
		if (currptr == buffers.buff1 + BUFFER_SIZE-1) {
			currptr = buffers.buff2;
		}
		else if (currptr == buffers.buff2 + BUFFER_SIZE-1) {
			currptr = buffers.buff1;
		}
		else {
			++currptr;
		}
	}
	}
	else
	{
		if(a&&d)
		{
			while(currptr!= buffers.buff1 + BUFFER_SIZE){
				lexeme[i] = *currptr; 
				++i;
				++currptr;
			}
			currptr = buffers.buff2;
			while(currptr!=forward)
			{
				lexeme[i] = *currptr;
				++i;
				++currptr;
			}

		}

		else 

		{
			while(currptr!= buffers.buff2 + BUFFER_SIZE){
				lexeme[i] = *currptr; 
				++i;
				++currptr;
			}
			currptr = buffers.buff1;
			while(currptr!=forward)
			{
				lexeme[i] = *currptr;
				++i;
				++currptr;
			}

		}
	}

	lexeme[i] = '\0';
	return lexeme;
}



tokenInfo getNextToken(){
    	int state=0;
	int err=-1; //error state
	int idLen=0; //to count identifier lengths

	char c; // Pointer to move across input
	char *lexeme; //Collected by getLexeme(B) while reading input
	tokenInfo t; // The thing we put lexeme into
    
	t.numVal=NULL;
	t.lexeme="";

    while(state >=0){
	    
        switch(state){
	    
            case 0: // Start State is 0
            {
                c = getNextChar(fp);
               
                if(c=='\n') {
                    ++line;
                    reset();
                }
                else if(c==' '||c=='\t')
                    reset();
                else if(c=='[')//
                    state=48;
                else if(c==']')//
                    state=49;
                else if(c==',')//
                    state=50;
                else if(c==';')//
                    state=51;
                else if(c==':')//
                    state=52;
                else if(c=='.')//
                    state=53;
                else if(c=='(')//
                    state=54;
                else if(c==')')//
                    state=55;
                else if(c=='+')//
                    state=15;
                else if(c=='-')//
                    state=16;
                else if(c=='*')//
                    state=17;
                else if(c=='/')//
                    state=18;
                else if(c=='~')//
                    state=21;
                else if(c=='!')//
                    state=45;
                else if(c=='&')//
                    state=36;
                else if(c=='@')//
                    state=39;
                else if(c=='=')//
                    state=19;
                else if(c=='<')//
                    state=1;
                else if(c=='>')//
                    state=42;
                else if(c=='%')//
                    state=47;
                else if(c=='#')//
                    state=26;
                else if(c=='_')//
                    state=22;
                else if(c >= '0' && c <= '9')//
                    state=6;
                else if(c >= 'b' && c <= 'd')//
                    state=30;
                else if(c == 'a' || (c >= 'e' && c <= 'z'))//
                    state=31;
                else if(c==EOF)
                    state=100;
                else {
                    state=-1;
                    err=0;
                }
                break;
            }
            
            case 47:
            {
			t.tid=TK_COMMENT;
			t.lineNo=line;
			while(c!='\n')
				{c = getNextChar(fp);}
			
			t.lexeme=getLexeme();
			retract(1);
			reset();
			return t;
            } 

            case 48:
            {
			t.tid=TK_SQL;
			t.lineNo=line;
			t.lexeme=getLexeme();
			reset();
			return t;
            } 

            case 49:
            {
			t.tid=TK_SQR;
			t.lineNo=line;
			t.lexeme=getLexeme();
			reset();
			return t;
            }

            case 50:
            {
			t.tid=TK_COMMA;
			t.lineNo=line;
			t.lexeme=getLexeme();
			reset();
			return t;
            }
            
            case 51:
            {
			t.tid=TK_SEM;
			t.lineNo=line;
			t.lexeme=getLexeme();
			reset();
			return t;
            }
            
            case 52:
            {
			t.tid=TK_COLON;
			t.lineNo=line;
			t.lexeme=getLexeme();
			reset();
			return t;
            }
            
            case 53:
            {
			t.tid=TK_DOT;
			t.lineNo=line;
			t.lexeme=getLexeme();
			reset();
			return t;
            }
            
            case 54:
            {
			t.tid=TK_OP;
			t.lineNo=line;
			t.lexeme=getLexeme();
			reset();
			return t;
            }
            
            case 55:
            {
			t.tid=TK_CL;
			t.lineNo=line;
			t.lexeme=getLexeme();
			reset();
			return t;
            }

             case 15:
            {
			t.tid=TK_PLUS;
			t.lineNo=line;
			t.lexeme=getLexeme();
			reset();
			return t;
            }
            
            case 16:
            {
			t.tid=TK_MINUS;
			t.lineNo=line;
			t.lexeme=getLexeme();
			reset();
			return t;
            }
            
            case 17:
            {
			t.tid=TK_MUL;
			t.lineNo=line;
			t.lexeme=getLexeme();
			reset();
			return t;
            }
            
            case 18:
            {
			t.tid=TK_DIV;
			t.lineNo=line;
			t.lexeme=getLexeme();
			reset();
			return t;
            }

            
            case 21:
            {
			t.tid=TK_NOT;
			t.lineNo=line;
			t.lexeme=getLexeme();
			reset();
			return t;
            }
            
            case 45:
            {                
                if(c == '=') {
                    t.tid = TK_NE;
                    t.lineNo = line;
                    t.lexeme=getLexeme();
                    reset();
                    return t;
                }
                else {
                    state=-1;
                    err=45;
                }
                break;
            }

            case 19:
            {
			c = getNextChar(fp);
			if(c == '=') {
				t.tid=TK_EQ;
				t.lineNo=line;
				t.lexeme=getLexeme();
				reset();
				return t;
			}
			else {
				state=-1;
				err=19;
			}
			break;
            }

            case 36:{
                	
			c = getNextChar(fp);
			if(c == '&')
				state=37;
			else {
				state=-1;
				err=36;
			}
			break;
            }

		    case 37:{

				c = getNextChar(fp);
                if(c =='&') 
				{
					t.tid=TK_AND;
					t.lineNo=line;
					t.lexeme=getLexeme();
					state = 0;
					reset();
					return t;
				}
			else {
				state=-1;
				err=37;
				}
			break;
            }

            case 39:{
                
			c = getNextChar(fp);
			if(c== '@')
				state=40;
			else {
				state=-1;
				err=39;
			}
			break;
            }

		    case 40:{
				c = getNextChar(fp);
                if(c =='@') {
				
				t.tid=TK_OR;
				t.lineNo=line;
				t.lexeme=getLexeme();
                state = 0;
				reset();
				return t;
			}
			else {
				state=-1;
				err=40;
			}
			break;
            }

            case 1:{
                
				c = getNextChar(fp);
                if(c=='=') {
                    t.tid=TK_LE;
                    t.lineNo=line;
                    t.lexeme=getLexeme();
                    reset();
                    return t;
                }
                else if(c=='-')
                    state=2;
                else {
                    t.tid=TK_LT;
                    t.lineNo=line;
                    retract(1);
                    t.lexeme=getLexeme();
                    //state = 5;
                    reset();
                    return t;
                }
                break;
            }

            case 2:{
            
			c = getNextChar(fp);
			if(c=='-')
				state=3;
			else {
				state=-1;
				err=2;
			}
			break;
            }

            case 3:{
                
			c = getNextChar(fp);
			if(c=='-'){
				t.tid=TK_ASSIGNOP;
				t.lineNo=line;
				t.lexeme=getLexeme();
				reset();
				return t;
			}
			else {
				state=-1;
				err=3;
			}
			break;
            }

            case 42:{
                
			c = getNextChar(fp);
			if(c=='=') {
				t.tid=TK_GE;
				t.lineNo=line;
				t.lexeme=getLexeme();
                //state = 43;
				reset();
				return t;
			}
			else {
				t.tid=TK_GT;
				t.lineNo=line;
				retract(1);
				t.lexeme=getLexeme();
                //state = 44;
				reset();
				return t;
			}
            }

            case 22:{
                c = getNextChar(fp);
                if( (c >=	'a' && c <= 'z') || (c >=	'A' && c <= 'Z') )
                    state=23;
                else {
                    state=-1;
                    err=22;
                }
                break;
            }

            case 23:{
                c = getNextChar(fp); 
                if(c >= '0' && c <= '9'){
                    state = 25;
                }
                else if( (c >=	'a' && c <= 'z') || (c >=	'A' && c <= 'Z') )
                    state=23;
                else{
                    t.tid=TK_FUNID;
                    t.lineNo=line;
                    retract(1);
                    t.lexeme=getLexeme();
                    //state = 24;
                    reset();
                    // Insert ID Please???
                    return t;
                }               
                
                break;
            }
			
			case 25:{
				c = getNextChar(fp);
                if(c >= '0' && c <= '9'){
                    state = 25;
                }
                else{
                    t.tid=TK_FUNID;
                    t.lineNo=line;
                    retract(1);
                    t.lexeme=getLexeme();
                    //state = 24;
                    reset();
                    // Insert ID Please???
                    return t;
                }               
                
                break;
            }

            case 26:{
                
				c = getNextChar(fp);
                if(c >=	'a' && c <= 'z')
                    state=27;
                else {
                    state=-1;
                    err=26;
                }
                break;
            }
            case 27:{
                 
				c = getNextChar(fp);
                if(c >=	'a' && c <= 'z')
                    state=27;
                else {
                    t.tid=TK_RUID;
                    t.lineNo=line;
                    retract(1);
                    t.lexeme=getLexeme();
                    //state = 28;
                    reset();
                    return t;
                }
                break;
            }

            // NUMBERS

            case 6:{
				c = getNextChar(fp);
                if(c >= '0' && c <= '9'){
                    state = 6;
                }
                else if(c == '.'){
                    state = 7;
                }
                else {
                    t.tid=TK_NUM;
                    t.lineNo=line;
                    retract(1);
                    lexeme=getLexeme();
                    t.lexeme=lexeme;
                    t.numVal=malloc(sizeof(int));
                    int *temp_ptr;
                    temp_ptr=t.numVal;
                    *(temp_ptr)=atoi(lexeme);
                    //state = 56;
                    reset();
                    return t;
                    }
			break;
            }

            case 7:{
                c = getNextChar(fp);
                if(c >= '0' && c <= '9'){
                    state = 8;
                }
                else {
                    state=-1;
                    err=7;
                }
                break;

            }

            case 8:{
               c = getNextChar(fp);
                if(c >= '0' && c <= '9'){
                    state = 9;
                }
                else {
                    state=-1;
                    err=7;
                }
                break;

            }

            case 9:{
                c = getNextChar(fp);
                if (c == 'E'){
                    state = 11;
                }
                else {
                    t.tid=TK_RNUM;
                    t.lineNo=line;
					retract(1);
                    lexeme=getLexeme();
                    t.lexeme=lexeme;
                    t.numVal=malloc(sizeof(double));
                    double *temp_ptr;
                    temp_ptr=t.numVal;
                    sscanf(lexeme,"%lf",temp_ptr);
                    reset();
                    state = 10;
                    return t;
                }
                break;

            }

            case 11:{
                c = getNextChar(fp);
                if (c == '+' || c== '-'){
                    state = 12;
                }
                else if(c >= '0' && c <= '9'){
                    state = 13;
                }
                else {
                    state=-1;
                    err=11;
                }
                break;

            }

            case 12:{
                c = getNextChar(fp);
                if(c >= '0' && c <= '9'){
                    
                    state = 13;
                }
                else {
                    state=-1;
                    err=7;
                }
                break;
            }

            case 13:{
                c = getNextChar(fp);
                if(c >= '0' && c <= '9'){
                    state = 14;
                    t.tid=TK_RNUM;
                    t.lineNo=line;
                    lexeme=getLexeme();
                    t.lexeme=lexeme;
                    t.numVal=malloc(sizeof(double));
                    double *temp_ptr;
                    temp_ptr=t.numVal;
                    sscanf(lexeme,"%lf",temp_ptr);
                    reset();
                    return t;
                }
                else {
                    state=-1;
                    err=7;
                }
                break;
            }


            // IDENTIFIERS

            case 30:{
            //    c=getNextChar(fp);
                idLen=2;
                if(c >= '2' && c <= '7'){
                    state=33;
                }
                else if(c >= 'a' && c <= 'z')
                    state = 31;
                else{
                    retract(1);
                    lexeme=getLexeme();
                    t.lexeme=lexeme;
                    t.tid=TK_FIELDID;
                    t.lineNo=line;
                    state=32;
                    // Lookup Table for Tokens!!!
                    reset();
                    return t;
                }
                    
                break;
            }

            case 31:{
         //       c=getNextChar(fp);
                idLen=2;
			if(c >= 'a' && c <= 'z')
                state = 31;
			else{
                retract(1);
                lexeme=getLexeme();
                t.lexeme=lexeme;
                t.tid=TK_FIELDID;
                t.lineNo=line;
                state=32;
                // Lookup Table for Tokens!!!
                reset();
                return t;
            }

            if(idLen>20){ // Check Length is less than 20
				state=-1;
				err=36;
			}
				
			break;

            }

            case 33:{
              ///  c=getNextChar(fp);
                idLen=2;
                if(c >= '2' && c <= '7'){
                    state=34;
                }
                else if(c == 'b' || c=='c' || c == 'd')
                    state = 33;
                else{
                    retract(1);
                    lexeme=getLexeme();
                    t.lexeme=lexeme;
                    t.tid=TK_ID;
                    t.lineNo=line;
                    state=35;
                    // Lookup Table for Tokens!!!
                    reset();
                    return t;
                }

                if(idLen>20){ // Check Length is less than 20
                    state=-1;
                    err=36;
                }
                    
                break;
            }

            case 34:{
            //    c=getNextChar(fp);
                idLen=2;
                if(c >= '2' && c <= '7'){
                    state=34;
                }
                else{
                    retract(1);
                    lexeme=getLexeme();
                    t.lexeme=lexeme;
                    t.tid=TK_ID;
                    t.lineNo=line;
                    state=35;
                    // Lookup Table for Tokens!!!
                    reset();
                    return t;
                }
                if(idLen>20){ // Check Length is less than 20
                    state=-1;
                    err=36;
                }
                    
                break;
            }

            case 100:{                    
                t.tid=SENTINEL;
                return t;
            }

        }
    }


	if(err!=-1) {
		fprintf(stdout,"\nLine:%d: LEXICAL_ERROR",line);
		lexeme=getLexeme();
		fprintf(stdout," in lexeme: %s\n",lexeme);
		free(lexeme);
		fprintf(stdout,"\tLexer state: %d\n",err);
		switch(err){
		case 0:
			fprintf(stdout, "\tUnrecognised character: %c\n",c);
			break;
		case 36:
			if(c==EOF)
				fprintf(stdout, "\tUnexpected character: EOF, expected '&'\n");
			else
				fprintf(stdout, "\tUnexpected character: '%c', expected '&'\n",c);
			retract(1);
			break;
		case 37:
			if(c==EOF)
				fprintf(stdout, "\tUnexpected character: EOF, expected '&'\n");
			else
				fprintf(stdout, "\tUnexpected character: '%c', expected '&'\n",c);
			retract(1);
			break;
		case 39:
			if(c==EOF)
				fprintf(stdout, "\tUnexpected character: EOF, expected '@'\n");
			else
				fprintf(stdout, "\tUnexpected character: '%c', expected '@'\n",c);
			retract(1);
			break;
		case 40:
			if(c==EOF)
				fprintf(stdout, "\tUnexpected character: EOF, expected '@'\n");
			else
				fprintf(stdout, "\tUnexpected character: '%c', expected '@'\n",c);
			retract(1);
			break;
		case 19:
			if(c==EOF)
				fprintf(stdout, "\tUnexpected character: EOF, expected '='\n");
			else
				fprintf(stdout, "\tUnexpected character: '%c', expected '='\n",c);
			retract(1);
			break;
		case 2:
			if(c==EOF)
				fprintf(stdout, "\tUnexpected character: EOF, expected '-'\n");
			else
				fprintf(stdout, "\tUnexpected character: '%c', expected '-'\n",c);
			retract(1);
			break;
		case 3:
			if(c==EOF)
				fprintf(stdout, "\tUnexpected character: EOF, expected '-'\n");
			else
				fprintf(stdout, "\tUnexpected character: '%c', expected '-'\n",c);
			retract(1);
			break;
		case 26:
			if(c==EOF)
				fprintf(stdout, "\tUnexpected character: EOF, expected lower case alphabet [a-z]\n");
			else
				fprintf(stdout, "\tUnexpected character: '%c', expected lower case alphabet [a-z]\n",c);
			retract(1);
			break;
		case 22:
			if(c==EOF)
				fprintf(stdout, "\tUnexpected character: EOF, expected lower case alphabet [a-z]\n");
			else
				fprintf(stdout, "\tUnexpected character: '%c', expected lower case alphabet [a-z]\n",c);
			retract(1);
			break;
		case 7:
			if(c==EOF)
				fprintf(stdout, "\tUnexpected character: EOF, expected digit [0-9]\n");
			else
				fprintf(stdout, "\tUnexpected character: '%c', expected digit [0-9]\n",c);
			retract(1);
		case 11:
			if(c==EOF)
				fprintf(stdout, "\tUnexpected character: EOF, expected digit [0-9] or '+' or '-'\n");
			else
				fprintf(stdout, "\tUnexpected character: '%c', expected digit [0-9] or '+' or '-'\n",c);
			retract(1);
			break;

		}
		reset();
		t.tid=LEX_ERROR;
		return t;
	}
    
}

void printToken(int token) {
	//gterm base=eps-1;
	//token -= base;
	switch(token)
	{
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
		case 107: printf("TK_RUID");
				break;
	}
}




// int main()
// {

// 	fp = startLexer("aadit.txt");

// 	/*
// 	printf("\nSIZE OF TWIN BUFFER %lu %lu %lu \n",sizeof(buffers), sizeof(buffers.buff1), sizeof(buffers.buff2));
	
// 	printf("Checking if twin buffers have been initialized or not \n %d %c %c %c \n", buffers.buff1[0],buffers.buff1[BUFFER_SIZE], buffers.buff2[0], buffers.buff2[BUFFER_SIZE]);	

// 	printf("Startlexer working fine\n");

	
		
// // 	for(int i = 0; i < 40; i++)
// // 		{
// // 		char c = getNextChar(fp);
// // 		printf("%c %d\n",c,c);
// // 		}
	
// // 	printf("Retraction\n");
// // 	retract(4);
	
// // 	for(int i = 0; i < 20; i++)
// // 		{
// // 		char c = getNextChar(fp);
// // 		printf("%c %d\n",c, c);
// // 		}
// // 	*/
// // 	for(int i = 0; i < 15; i++)
// // 	{	
// // 	tokenInfo pleasework = getNextToken();
	
// // 	printf(" %s %d",pleasework.lexeme, pleasework.lineNo);
// // 	printToken(pleasework.tid);
// // 	printf("\n");
// // 	}
// // 	/*
// // 	printf("\nFirst call to getStream function\n");	

 	
		
// // 	printf("\nContents of buffer 1\n");		

// // 	printf("\n%s", buffers.buff1);

// // 	printf("\nContents of buffer 2\n");

// // 	printf("\n%s", buffers.buff2);

// // 	printf("\nSecond call to getStream function\n");	

// //  	fp = getStream(fp);
		
// // 	printf("\nContents of buffer 1\n");		

// // 	printf("\n%s", buffers.buff1);

// // 	printf("\nContents of buffer 2\n");

// // 	printf("\n%s", buffers.buff2);
// // 	*/
	

// // 	return 0;
// }











