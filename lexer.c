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