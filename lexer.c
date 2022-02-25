#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lexer.h"

typedef struct keyword {
	char key[20];
	char value[20];
}keyword;

//we are hard coding the lookup table 
//maybe change this part of implementation  
#define COUNT_KEYWORDS 28

keyword keywords[] = {
	{"with","TK_WITH"},
	{"parameters","TK_PARAMETERS"},
	{"end","TK_END"},
	{"while","TK_WHILE"},
	{"union","TK_UNION"},
	{"endunion","TK_ENDUNION"},
	{"definetype","TK_DEFINETYPE"},
	{"as","TK_AS"},
	{"type","TK_TYPE"},
	{"main","TK_MAIN"},
	{"global","TK_GLOBAL"},
	{"parameter","TK_PARAMETER"},
	{"list","TK_LIST"},
	{"input","TK_INPUT"},
	{"int","TK_INT"},
	{"real","TK_REAL"},
	{"endwhile","TK_ENDWHILE"},
	{"if","TK_IF"},
	{"then","TK_THEN"},
	{"endif","TK_ENDIF"},
	{"read","TK_READ"},
	{"write","TK_WRITE"},
	{"return","TK_RETURN"},
	{"call","TK_CALL"},
	{"record","TK_RECORD"},
	{"endrecord","TK_ENDRECORD"},
	{"else","TK_ELSE"}
};

twinBuffer buffers;
int line = 1;
//helps to keep track of the line number

bool file_end;
//tells us if we have reached the end of file

char *lexemeBegin, *forward;
//two pointers for the twin buffer


int charCount;
//keeps track of the size of lexeme

bool readBuff1, readBuff2;
//true means that the buffer has to be

//helper functions

//strip white space from beginning of string
char *strip(char *str)
{
    while(*str==' ') str++;
    return str;
}

void removeComments(char* testcaseFile, char* cleanFile){
    printf("%s\n",testcaseFile);
    FILE *inp = fopen(testcaseFile,"r");
    FILE *out = fopen(cleanFile,"w");

    if(!inp){
        printf("Error opening testcaseFile\n");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t sizeRead;

    sizeRead = getline(&line, &len, inp);

    while(sizeRead != -1) {
        char *mod = strip(line);
        if(strlen(mod)>0){
            if(mod[0]=='%'){
                sizeRead = getline(&line, &len, inp);
                continue;
            }
        }
        fprintf(out,"%s",line);
        sizeRead = getline(&line, &len, inp);
    }
}

char * getLexeme(twinBuffer B) {
	char *lexeme = (char *)malloc((lexemeSize + 1) * sizeof(char));
	int i = 0;
	char *currptr = lexemeBegin;
	while (currptr != forward) {
		lexeme[i] = *currptr;
		++i;
		if (currptr == B.buff1 + BUFFER_SIZE - 1) {
			currptr = B.buff2;
		}
		else if (currptr == B.buff2 + BUFFER_SIZE - 1) {
			currptr = B.buff1;
		}
		else {
			++currptr;
		}
	}
	lexeme[i] = '\0';
	return lexeme;
}

char getNextChar(twinBuffer B) {
	char curr = *forward;
	if (forward == B.buff1 + BUFFER_SIZE - 1) {
		if (read2) {
			int size2 = fread(B.buff2, sizeof(char), BUFFER_SIZE, fptr);
			B.buff2[size2] = EOF;
		}
		forward = B.buff2;
		read2 = true; // Resetting the flag if it was false
	}
	else if (forward == B.buff2 + BUFFER_SIZE - 1) {
		if (read1) {
			int size1 = fread(B.buff1, sizeof(char), BUFFER_SIZE, fptr);
			B.buff1[size1] = EOF;
		}
		forward = B.buff1;
		read1 = true; // Resetting the flag if it was false
	}
	else {
		++forward;
	}
	++lexemeSize;
	return curr;
}

void reset(){
	lexemeBegin = forward;
	lexemeSize = 0;
}

tokenInfo getNextToken(twinBuffer B){
    int state=0;
	int err=-1; //error state
	int idLen=0; //to count identifier lengths
	char c;
	char *lexeme;
	tokenInfo t;
    
	t.numVal=NULL;
	t.lexeme="";

    while(state >=0){
        switch(state){

            case 0: // Start State is 0
            {
                c=getNextChar();
                // fprintf(stderr, "%c",c );
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
                    state=40;
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
			t.lexeme=getLexeme(B);
			reset();
			return t;
            } 

            case 48:
            {
			t.tid=TK_SQL;
			t.lineNo=line;
			t.lexeme=getLexeme(B);
			reset();
			return t;
            } 

            case 49:
            {
			t.tid=TK_SQR;
			t.lineNo=line;
			t.lexeme=getLexeme(B);
			reset();
			return t;
            }

            case 50:
            {
			t.tid=TK_COMMA;
			t.lineNo=line;
			t.lexeme=getLexeme(B);
			reset();
			return t;
            }
            
            case 51:
            {
			t.tid=TK_SEM;
			t.lineNo=line;
			t.lexeme=getLexeme(B);
			reset();
			return t;
            }
            
            case 52:
            {
			t.tid=TK_COLON;
			t.lineNo=line;
			t.lexeme=getLexeme(B);
			reset();
			return t;
            }
            
            case 53:
            {
			t.tid=TK_DOT;
			t.lineNo=line;
			t.lexeme=getLexeme(B);
			reset();
			return t;
            }
            
            case 54:
            {
			t.tid=TK_OPEN;
			t.lineNo=line;
			t.lexeme=getLexeme(B);
			reset();
			return t;
            }
            
            case 55:
            {
			t.tid=TK_CLOSE;
			t.lineNo=line;
			t.lexeme=getLexeme(B);
			reset();
			return t;
            }

             case 15:
            {
			t.tid=TK_PLUS;
			t.lineNo=line;
			t.lexeme=getLexeme(B);
			reset();
			return t;
            }
            
            case 16:
            {
			t.tid=TK_MINUS;
			t.lineNo=line;
			t.lexeme=getLexeme(B);
			reset();
			return t;
            }
            
            case 17:
            {
			t.tid=TK_MUL;
			t.lineNo=line;
			t.lexeme=getLexeme(B);
			reset();
			return t;
            }
            
            case 18:
            {
			t.tid=TK_DIV;
			t.lineNo=line;
			t.lexeme=getLexeme(B);
			reset();
			return t;
            }

            
            case 21:
            {
			t.tid=TK_NOT;
			t.lineNo=line;
			t.lexeme=getLexeme(B);
			reset();
			return t;
            }
            
            case 45:
            {                
                if((c=getNextChar()) == '=') {
                    t.tid = TK_NE;
                    t.lineNo = line;
                    t.lexeme=getLexeme(B);
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
			if((c=getNextChar()) == '=') {
				t.tid=TK_EQ;
				t.lineNo=line;
				t.lexeme=getLexeme(B);
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
                
			if((c=getNextChar()) == '&')
				state=37;
			else {
				state=-1;
				err=36;
			}
			break;
            }

		    case 37:{
                if((c=getNextChar())=='&') {
				t.tid=TK_AND;
				t.lineNo=line;
				t.lexeme=getLexeme(B);
                state = 38;
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
                
			if((c=getNextChar()) == '&')
				state=40;
			else {
				state=-1;
				err=39;
			}
			break;
            }

		    case 40:{
                if((c=getNextChar())=='&') {
				t.tid=TK_OR;
				t.lineNo=line;
				t.lexeme=getLexeme(B);
                state = 41;
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
                c=getNextChar();
                if(c=='=') {
                    t.tid=TK_LE;
                    t.lineNo=line;
                    t.lexeme=getLexeme(B);
                    state = 29;
                    reset();
                    return t;
                }
                else if(c=='-')
                    state=2;
                else {
                    t.tid=TK_LT;
                    t.lineNo=line;
                    retract(1);
                    t.lexeme=getLexeme(B);
                    state = 5;
                    reset();
                    return t;
                }
                break;
            }

            case 2:{
                
			if((c=getNextChar())=='-')
				state=3;
			else {
				state=-1;
				err=2;
			}
			break;
            }

            case 3:{
                
			if((c=getNextChar())=='-'){
				t.tid=TK_ASSIGNOP;
				t.lineNo=line;
				t.lexeme=getLexeme(B);
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
                
			c=getNextChar();
			if(c=='=') {
				t.tid=TK_GE;
				t.lineNo=line;
				t.lexeme=getLexeme(B);
                state = 43;
				reset();
				return t;
			}
			else {
				t.tid=TK_GT;
				t.lineNo=line;
				t.lexeme=getLexeme(B);
                state = 44;
				reset();
				return t;
			}
            }

            case 22:{
                c=getNextChar();
                if( (c >=	'a' && c <= 'z') || (c >=	'A' && c <= 'Z') )
                    state=23;
                else {
                    state=-1;
                    err=22;
                }
                break;
            }

            case 23:{
                c=getNextChar();
                if(c >= '0' && c <= '9'){
                    state = 25;
                }
                else if( (c >=	'a' && c <= 'z') || (c >=	'A' && c <= 'Z') )
                    state=23;
                else{
                    t.tid=TK_FUNID;
                    t.lineNo=line;
                    retract(1);
                    t.lexeme=getLexeme(B);
                    state = 24;
                    reset();
                    // Insert ID Please???
                    return t;
                }               
                
                break;
            }
			
			case 25:{
                c=getNextChar();
                if(c >= '0' && c <= '9'){
                    state = 25;
                }
                else{
                    t.tid=TK_FUNID;
                    t.lineNo=line;
                    retract(1);
                    t.lexeme=getLexeme(B);
                    state = 24;
                    reset();
                    // Insert ID Please???
                    return t;
                }               
                
                break;
            }

            case 26:{
                 c=getNextChar();
                if(c >=	'a' && c <= 'z')
                    state=27;
                else {
                    state=-1;
                    err=26;
                }
                break;
            }
            case 27:{
                 c=getNextChar();
                if(c >=	'a' && c <= 'z')
                    state=27;
                else {
                    t.tid=TK_RUID;
                    t.lineNo=line;
                    retract(1);
                    t.lexeme=getLexeme(B);
                    state = 28;
                    reset();
                    return t;
                }
                break;
            }

            // NUMBERS

            case 6:{
                c = getNextChar();
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
                    state = 56;
                    reset();
                    return t;
                    }
			break;
            }

            case 7:{
                c = getNextChar();
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
                c = getNextChar();
                if(c >= '0' && c <= '9'){
                    state = 9;
                }
                else {
                    state=-1;
                    err=8;
                }
                break;

            }

            case 9:{
                c = getNextChar();
                if (c == 'E'){
                    state = 11;
                }
                else {
                    t.tid=TK_RNUM;
                    t.lineNo=line;
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
                c = getNextChar();
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
                c = getNextChar();
                if(c >= '0' && c <= '9'){
                    
                    state = 13;
                }
                else {
                    state=-1;
                    err=12;
                }
                break;
            }

            case 13:{
                c = getNextChar();
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
                    err=13;
                }
                break;
            }


            // IDENTIFIERS

            case 30:{
                c=getNextChar();
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
                c=getNextChar();
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
                c=getNextChar();
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
                c=getNextChar();
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












        }
    }
    
}

FILE* startLexer(char* inputFile) {
	//must be called in the driver file before starting the lexer

	//initialising buffers to 0
	memset(buffers.buff1, 0, sizeof(buffers.buff1));
	memset(buffers.buff2, 0, sizeof(buffers.buff2));

	line = 1;

	charCount = 0;

	lexemeBegin = NULL;
	forward = NULL;

	FILE* fp = fopen(inputFile, "r");


	if(fp == NULL) {
		printf("Error opening file\n");
		return NULL;
	}

	else {
		printf("\n Successfully opened the code file\n");
	}

	return fp;
}

void closeLexer(FILE* fp) {
	int err = fclose(fp);
}

FILE* getStream(FILE *fp) {
	
	if(forward == NULL) {
			//start case
			//read into buff1 and set readBuff1 as true
		int size1 = fread(buffers.buff1, sizeof(char), BUFFER_SIZE, fp);
		buffers.buff1[size1] = EOF;
		lexemeBegin = buffers.buff1;
		forward = buffers.buff1;

		readBuff1 = true;
		readBuff2 = true;
	}
	
	if(forward == buffers.buff1 + BUFFER_SIZE - 1)	{
		//forward is at the end of buffer1
		if(readBuff2) {
			int size2 = fread(buffers.buff2, sizeof(char), BUFFER_SIZE, fp);
			buffers.buff2[size2] = EOF;
		}
		forward = buffers.buff2;
		readBuff2 = true;
	}

	else if (forward == buffers.buff2 + BUFFER_SIZE - 1) {
		//forward is at the end of buffer2
		if(readBuff1) {
			int size1 = fread(buffers.buff1, sizeof(char), BUFFER_SIZE, fp);
			buffers.buff2[size1] = EOF;
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

char getChar(FILE* fp) {
	charCount++;
	char c = *forward;
	fp = getStream(fp);
    return c;
}

void retract(int n){
	//used for retracting n characters  
	if(forward >= buffers.buff1 && forward < buffers.buff1 + BUFFER_SIZE) {
		if(forward - buffers.buff1 < n) {
			readBuff1 = false;
			n -= forward - buffers.buff1;
			forward = buffers.buff2 + BUFFER_SIZE - n;
		}

		else {
			forward -= n;
		}
	} 

	else {
		if(forward - buffers.buff2 < n) {
			readBuff2 = false;
			n -= forward - buffers.buff2;
			forward = buffers.buff1 + BUFFER_SIZE - n;
		}

		else {
			forward -= n;
		}
	}
	charCount -= n;
}

/*
tokenInfo getNextToken(twinBuffer B) {

}
*/
void main(){
    removeComments("testComments.txt","output.txt");
	FILE* fp = startLexer("testComments.txt");
	//printf("%ld\n", sizeof(buffers.buff1));
	closeLexer(fp);
}
