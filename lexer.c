/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/

#include "lexer.h"

twinBuffer buffers;

line = 1;

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

FILE *startLexer(char *inputFile)
{
	//must be called in the driver file before starting the lexer
	//initialising buffers to 0

	memset(buffers.buff1, 0, sizeof(buffers.buff1));
	memset(buffers.buff2, 0, sizeof(buffers.buff2));

	int line = 1;
	bool file_end = false;
	int charCount = 0;
	bool readBuff1 = false;
	bool readBuff2 = false;
	lexemeBegin = NULL;
	forward = NULL;
	startedReading = false;

	FILE *fp = fopen(inputFile, "r");

	if (fp == NULL)
	{
		printf("Error opening file\n");
		return NULL;
	}

	int size1 = fread(buffers.buff1, sizeof(char), BUFFER_SIZE, fp);
	buffers.buff1[size1] = EOF;
	lexemeBegin = buffers.buff1;
	forward = buffers.buff1;
	readBuff1 = true;
	readBuff2 = true;
	startedReading = true;
	return fp;
}

void reset()
{
	lexemeBegin = forward;
	charCount = 0;
}

void closeLexer(FILE *fp)
{
	int err = fclose(fp);
}

FILE *getStream(FILE *fp)
{

	if (!startedReading)
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
	}

	else if (forward == buffers.buff1 + BUFFER_SIZE - 1)
	{
		//forward is at the end of buffer1
		if (readBuff2)
		{
			int size2 = fread(buffers.buff2, sizeof(char), BUFFER_SIZE, fp);
			buffers.buff2[size2] = EOF;
		}
		forward = buffers.buff2;
		readBuff2 = true;
	}

	else if (forward == buffers.buff2 + BUFFER_SIZE - 1)
	{
		//forward is at the end of buffer2
		if (readBuff1)
		{
			int size1 = fread(buffers.buff1, sizeof(char), BUFFER_SIZE, fp);
			buffers.buff1[size1] = EOF;
		}
		forward = buffers.buff1;
		readBuff1 = true;
	}

	else
	{
		forward++;
	}
	return fp;
}

char getNextChar(FILE *fp)
{
	if (!startedReading)
	{
		char curr = *forward;
		++charCount;
		forward++;
		return curr;
	}
	char curr = *forward;
	fp = getStream(fp);
	++charCount;
	return curr;
}

void retract(int n)
{
	if (forward >= buffers.buff1 && forward < buffers.buff1 + BUFFER_SIZE)
	{
		if (forward - buffers.buff1 < n)
		{
			readBuff1 = false; // Don't load buff1 the next time
			n -= forward - buffers.buff1;
			forward = buffers.buff2 + BUFFER_SIZE - n;
		}
		else
		{
			forward -= n;
		}
	}
	else if (forward >= buffers.buff2 && forward < buffers.buff2 + BUFFER_SIZE)
	{
		if (forward - buffers.buff2 < n)
		{
			readBuff2 = false; // Don't load buff2 the next time
			n -= forward - buffers.buff2;
			forward = buffers.buff1 + BUFFER_SIZE - n;
		}
		else
		{
			forward -= n;
		}
	}
	charCount -= n;
}

char *getLexeme()
{
	char *lexeme = (char *)malloc((charCount + 1) * sizeof(char));
	int i = 0;
	char *currptr = lexemeBegin;
	bool a = lexemeBegin >= buffers.buff1 && lexemeBegin < buffers.buff1 + BUFFER_SIZE;
	bool b = lexemeBegin >= buffers.buff2 && lexemeBegin < buffers.buff2 + BUFFER_SIZE;
	bool c = forward >= buffers.buff1 && forward < buffers.buff1 + BUFFER_SIZE;
	bool d = forward >= buffers.buff2 && forward < buffers.buff2 + BUFFER_SIZE;

	if ((a && c) || (b && d))
	{
		while (currptr < forward)
		{
			lexeme[i] = *currptr;
			++i;
			if (currptr == buffers.buff1 + BUFFER_SIZE - 1)
			{
				currptr = buffers.buff2;
			}
			else if (currptr == buffers.buff2 + BUFFER_SIZE - 1)
			{
				currptr = buffers.buff1;
			}
			else
			{
				++currptr;
			}
		}
	}
	else
	{
		if (a && d)
		{
			while (currptr != buffers.buff1 + BUFFER_SIZE)
			{
				lexeme[i] = *currptr;
				++i;
				++currptr;
			}
			currptr = buffers.buff2;
			while (currptr != forward)
			{
				lexeme[i] = *currptr;
				++i;
				++currptr;
			}
		}

		else

		{
			while (currptr != buffers.buff2 + BUFFER_SIZE)
			{
				lexeme[i] = *currptr;
				++i;
				++currptr;
			}
			currptr = buffers.buff1;
			while (currptr != forward)
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

tokenInfo getNextToken()
{
	int state = 0;
	int err = -1;	   //error state
	int idLen = 0;	   //to count identifier lengths
	int funcIdLen = 0; //to count function ID lengths

	char c;		  // Pointer to move across input
	char *lexeme; //Collected by getLexeme(B) while reading input
	tokenInfo t;  // The thing we put lexeme into

	t.numVal = NULL;
	t.lexeme = "";

	while (state >= 0)
	{

		switch (state)
		{

		case 0: // Start State is 0
		{
			c = getNextChar(fp);

			if (c == '\n')
			{
				++line;
				reset();
			}
			else if (c == ' ' || c == '\t')
				reset();
			else if (c == '[') //
				state = 48;
			else if (c == ']') //
				state = 49;
			else if (c == ',') //
				state = 50;
			else if (c == ';') //
				state = 51;
			else if (c == ':') //
				state = 52;
			else if (c == '.') //
				state = 53;
			else if (c == '(') //
				state = 54;
			else if (c == ')') //
				state = 55;
			else if (c == '+') //
				state = 15;
			else if (c == '-') //
				state = 16;
			else if (c == '*') //
				state = 17;
			else if (c == '/') //
				state = 18;
			else if (c == '~') //
				state = 21;
			else if (c == '!') //
				state = 45;
			else if (c == '&') //
				state = 36;
			else if (c == '@') //
				state = 39;
			else if (c == '=') //
				state = 19;
			else if (c == '<') //
				state = 1;
			else if (c == '>') //
				state = 42;
			else if (c == '%') //
				state = 47;
			else if (c == '#') //
				state = 26;
			else if (c == '_') //
				state = 22;
			else if (c >= '0' && c <= '9') //
				state = 6;
			else if (c >= 'b' && c <= 'd') //
				state = 30;
			else if (c == 'a' || (c >= 'e' && c <= 'z')) //
				state = 31;
			else if (c == EOF)
				state = 100;
			else
			{
				state = -1;
				err = 0;
			}
			break;
		}

		case 47:
		{
			t.tid = TK_COMMENT;
			t.lineNo = line;
			while (c != '\n')
			{
				c = getNextChar(fp);
			}

			t.lexeme = getLexeme();
			retract(1);
			reset();
			return t;
		}

		case 48:
		{
			t.tid = TK_SQL;
			t.lineNo = line;
			t.lexeme = getLexeme();
			reset();
			return t;
		}

		case 49:
		{
			t.tid = TK_SQR;
			t.lineNo = line;
			t.lexeme = getLexeme();
			reset();
			return t;
		}

		case 50:
		{
			t.tid = TK_COMMA;
			t.lineNo = line;
			t.lexeme = getLexeme();
			reset();
			return t;
		}

		case 51:
		{
			t.tid = TK_SEM;
			t.lineNo = line;
			t.lexeme = getLexeme();
			reset();
			return t;
		}

		case 52:
		{
			t.tid = TK_COLON;
			t.lineNo = line;
			t.lexeme = getLexeme();
			reset();
			return t;
		}

		case 53:
		{
			t.tid = TK_DOT;
			t.lineNo = line;
			t.lexeme = getLexeme();
			reset();
			return t;
		}

		case 54:
		{
			t.tid = TK_OP;
			t.lineNo = line;
			t.lexeme = getLexeme();
			reset();
			return t;
		}

		case 55:
		{
			t.tid = TK_CL;
			t.lineNo = line;
			t.lexeme = getLexeme();
			reset();
			return t;
		}

		case 15:
		{
			t.tid = TK_PLUS;
			t.lineNo = line;
			t.lexeme = getLexeme();
			reset();
			return t;
		}

		case 16:
		{
			t.tid = TK_MINUS;
			t.lineNo = line;
			t.lexeme = getLexeme();
			reset();
			return t;
		}

		case 17:
		{
			t.tid = TK_MUL;
			t.lineNo = line;
			t.lexeme = getLexeme();
			reset();
			return t;
		}

		case 18:
		{
			t.tid = TK_DIV;
			t.lineNo = line;
			t.lexeme = getLexeme();
			reset();
			return t;
		}

		case 21:
		{
			t.tid = TK_NOT;
			t.lineNo = line;
			t.lexeme = getLexeme();
			reset();
			return t;
		}

		case 45:
		{
			c = getNextChar(fp);
			if (c == '=')
			{
				t.tid = TK_NE;
				t.lineNo = line;
				t.lexeme = getLexeme();
				reset();
				return t;
			}
			else
			{
				state = -1;
				err = 45;
			}
			break;
		}

		case 19:
		{
			c = getNextChar(fp);
			if (c == '=')
			{
				t.tid = TK_EQ;
				t.lineNo = line;
				t.lexeme = getLexeme();
				reset();
				return t;
			}
			else
			{
				state = -1;
				err = 19;
			}
			break;
		}

		case 36:
		{

			c = getNextChar(fp);
			if (c == '&')
				state = 37;
			else
			{
				state = -1;
				err = 36;
			}
			break;
		}

		case 37:
		{

			c = getNextChar(fp);
			if (c == '&')
			{
				t.tid = TK_AND;
				t.lineNo = line;
				t.lexeme = getLexeme();
				state = 0;
				reset();
				return t;
			}
			else
			{
				state = -1;
				err = 37;
			}
			break;
		}

		case 39:
		{

			c = getNextChar(fp);
			if (c == '@')
				state = 40;
			else
			{
				state = -1;
				err = 39;
			}
			break;
		}

		case 40:
		{
			c = getNextChar(fp);
			if (c == '@')
			{

				t.tid = TK_OR;
				t.lineNo = line;
				t.lexeme = getLexeme();
				state = 0;
				reset();
				return t;
			}
			else
			{
				state = -1;
				err = 40;
			}
			break;
		}

		case 1:
		{

			c = getNextChar(fp);
			if (c == '=')
			{
				t.tid = TK_LE;
				t.lineNo = line;
				t.lexeme = getLexeme();
				reset();
				return t;
			}
			else if (c == '-')
				state = 2;
			else
			{
				t.tid = TK_LT;
				t.lineNo = line;
				retract(1);
				t.lexeme = getLexeme();
				//state = 5;
				reset();
				return t;
			}
			break;
		}

		case 2:
		{

			c = getNextChar(fp);
			if (c == '-')
				state = 3;
			else
			{
				state = -1;
				err = 2;
			}
			break;
		}

		case 3:
		{

			c = getNextChar(fp);
			if (c == '-')
			{
				t.tid = TK_ASSIGNOP;
				t.lineNo = line;
				t.lexeme = getLexeme();
				reset();
				return t;
			}
			else
			{
				state = -1;
				err = 3;
			}
			break;
		}

		case 42:
		{

			c = getNextChar(fp);
			if (c == '=')
			{
				t.tid = TK_GE;
				t.lineNo = line;
				t.lexeme = getLexeme();
				//state = 43;
				reset();
				return t;
			}
			else
			{
				t.tid = TK_GT;
				t.lineNo = line;
				retract(1);
				t.lexeme = getLexeme();
				//state = 44;
				reset();
				return t;
			}
		}

		case 22:
		{
			c = getNextChar(fp);
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			{
				state = 23;
				funcIdLen = 2;
			}

			else
			{
				state = -1;
				err = 22;
			}
			break;
		}

		case 23:
		{
			c = getNextChar(fp);
			if (c >= '0' && c <= '9')
			{
				{
					state = 25;
					funcIdLen++;
				}
			}
			else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			{
				state = 23;
				funcIdLen++;
			}
			else
			{

				if (funcIdLen > 30)
				{
					state = -1;
					err = 23;
					reset();
				}
				else
				{
					t.lineNo = line;
					retract(1);
					t.lexeme = getLexeme();
					g_Term token = getToken(t.lexeme);
					if(token==-1)
						{
							t.tid = TK_FUNID;
						}
					else
						{
							t.tid = token;
						}
					// t.tid = TK_FUNID;
					reset();
					return t;
				}
			}

			break;
		}

		case 25:
		{
			c = getNextChar(fp);
			if (c >= '0' && c <= '9')
			{
				funcIdLen++;
				state = 25;
			}
			else
			{
				if (funcIdLen > 30)
				{
					state = -1;
					err = 23;
					reset();
				}
				else
				{
					t.lineNo = line;
					t.lexeme = getLexeme();
					retract(1);
					g_Term token = getToken(t.lexeme);
					if(token==-1)
						{
							t.tid = TK_FUNID;
						}
					else
						{
							t.tid = token;
						}
					// t.tid = TK_FUNID;
					reset();
					return t;
				}
			}

			break;
		}

		case 26:
		{

			c = getNextChar(fp);
			if (c >= 'a' && c <= 'z')
				state = 27;
			else
			{
				state = -1;
				err = 26;
			}
			break;
		}
		case 27:
		{

			c = getNextChar(fp);
			if (c >= 'a' && c <= 'z')
				state = 27;
			else
			{
				t.tid = TK_RUID;
				t.lineNo = line;
				retract(1);
				t.lexeme = getLexeme();
				//state = 28;
				reset();
				return t;
			}
			break;
		}

			// NUMBERS

		case 6:
		{
			c = getNextChar(fp);
			if (c >= '0' && c <= '9')
			{
				state = 6;
			}
			else if (c == '.')
			{
				state = 7;
			}
			else
			{
				t.tid = TK_NUM;
				t.lineNo = line;
				retract(1);
				lexeme = getLexeme();
				t.lexeme = lexeme;
				t.numVal = malloc(sizeof(int));
				int *temp_ptr;
				temp_ptr = t.numVal;
				*(temp_ptr) = atoi(lexeme);
				reset();
				return t;
			}
			break;
		}

		case 7:
		{
			c = getNextChar(fp);
			if (c >= '0' && c <= '9')
			{
				state = 8;
			}
			else
			{
				state = -1;
				err = 7;
			}
			break;
		}

		case 8:
		{
			c = getNextChar(fp);
			if (c >= '0' && c <= '9')
			{
				state = 9;
			}
			else
			{
				state = -1;
				err = 8;
			}
			break;
		}

		case 9:
		{
			c = getNextChar(fp);
			if (c == 'E')
			{
				state = 11;
			}
			else
			{
				t.tid = TK_RNUM;
				t.lineNo = line;
				retract(1);
				lexeme = getLexeme();
				t.lexeme = lexeme;
				t.numVal = malloc(sizeof(double));
				double *temp_ptr;
				temp_ptr = t.numVal;
				sscanf(lexeme, "%lf", temp_ptr);
				reset();
				state = 10;
				return t;
			}
			break;
		}

		case 11:
		{
			c = getNextChar(fp);
			if (c == '+' || c == '-')
			{
				state = 12;
			}
			else if (c >= '0' && c <= '9')
			{
				state = 13;
			}
			else
			{
				state = -1;
				err = 11;
			}
			break;
		}

		case 12:
		{
			c = getNextChar(fp);
			if (c >= '0' && c <= '9')
			{

				state = 13;
			}
			else
			{
				state = -1;
				err = 12;
			}
			break;
		}

		case 13:
		{
			c = getNextChar(fp);
			if (c >= '0' && c <= '9')
			{
				t.tid = TK_RNUM;
				t.lineNo = line;
				lexeme = getLexeme();
				t.lexeme = lexeme;
				t.numVal = malloc(sizeof(double));
				double *temp_ptr;
				temp_ptr = t.numVal;
				sscanf(lexeme, "%lf", temp_ptr);
				reset();
				return t;
			}
			else
			{
				state = -1;
				err = 13;
			}
			break;
		}

			// IDENTIFIERS

		case 30:
		{
			c = getNextChar(fp);

			if (c >= '2' && c <= '7')
			{
				idLen = 2;
				state = 33;
			}
			else if (c >= 'a' && c <= 'z')
				state = 31;
			else
			{
				retract(1);
				t.lineNo = line;
				t.lexeme = getLexeme();
				g_Term token = getToken(t.lexeme);
				if(token==-1)
					{
						t.tid = TK_FIELDID;
					}
				else
					{
						t.tid = token;
					}
				// t.tid = TK_FIELDID;
				reset();
				return t;
			}

			break;
		}

		case 31:
		{
			c = getNextChar(fp);
			if (c >= 'a' && c <= 'z')
			{
				state = 31;
			}
			else
			{
				retract(1);
				t.lexeme = getLexeme();
				t.lineNo = line;
				g_Term token = getToken(t.lexeme);
				if(token==-1)
					{
						t.tid = TK_FIELDID;
					}
				else
					{
						t.tid = token;
					}
				// t.tid = TK_FIELDID;
				reset();
				return t;
			}

			// if(idLen>20){ // Check Length is less than 20
			// 	state=-1;
			// 	err=36;
			// }

			break;
		}

		case 33:
		{
			c = getNextChar(fp);
			if (c >= '2' && c <= '7')
			{
				idLen = 3;
				state = 34;
			}
			else if (c == 'b' || c == 'c' || c == 'd')
			{
				state = 33;
				idLen++;
			}
			else
			{
				retract(1);

				if (idLen > 20)
				{ // Check Length is less than 20
					state = -1;
					err = 33;
					reset();
				}
				else
				{
					lexeme = getLexeme();
					t.lexeme = lexeme;
					g_Term token = getToken(lexeme);
					if(token==-1)
					{
						t.tid = TK_ID;
					}
					else
					{
						t.tid = token;
					}
					t.lineNo = line;
					reset();
					return t;
				}
			}

			break;
		}

		case 34:
		{
			c = getNextChar(fp);

			if (c >= '2' && c <= '7')
			{
				idLen++;
				state = 34;
			}
			else
			{

				if (idLen > 20)
				{ // Check Length is less than 20
					state = -1;
					err = 33;
					reset();
				}
				else
				{
					retract(1);
					lexeme = getLexeme();
					t.lexeme = lexeme;
					g_Term token = getToken(lexeme);
					if(token==-1)
						{
							t.tid = TK_ID;
						}
					else
						{
							t.tid = token;
						}
					// t.tid = TK_ID;	
					t.lineNo = line;
					reset();
					return t;
				}
			}

			break;
		}

		case 100:
		{
		
			t.lineNo = line;
			t.tid = SENTINEL;
			return t;
		}
		}
	}

	//printf("Line no. %d : Error: Custom message <%s>");

	if (err != -1)
	{
		//fprintf(stdout, "\nLine:%d: LEXICAL_ERROR", line);
		
		lexeme = getLexeme();
		//fprintf(stdout, " in lexeme: %s\n", lexeme);
		//free(lexeme);
		//fprintf(stdout, "\tLexer state: %d\n", err);
		if(err!=0)
		{
			lexeme[strlen(lexeme)-1] = '\0';
			//lexeme[strlen(lexeme)-2] = '\0';
		}

		switch (err)
		{
	    case 0:
		 	printf("Line no. %d : Error: Unknown Symbol <%s>\n", line, lexeme);
		// 	//fprintf(stdout, "\tUnrecognised character: %c\n", c);
		 	break;
	    
		case 36:
			// if (c == EOF)
			// 	fprintf(stdout, "\tUnexpected character: EOF, expected '&'\n");
			// else
			// 	fprintf(stdout, "\tUnexpected character: '%c', expected '&'\n", c);
			printf("Line no. %d : Error: Unknown pattern <%s>\n", line, lexeme);
			retract(1);
			break;
		case 37:
			// if (c == EOF)
			// 	fprintf(stdout, "\tUnexpected character: EOF, expected '&'\n");
			// else
			// 	fprintf(stdout, "\tUnexpected character: '%c', expected '&'\n", c);
			printf("Line no. %d : Error: Unknown pattern <%s>\n", line, lexeme);
			retract(1);
			break;
		case 39:
			// if (c == EOF)
			// 	fprintf(stdout, "\tUnexpected character: EOF, expected '@'\n");
			// else
			// 	fprintf(stdout, "\tUnexpected character: '%c', expected '@'\n", c);
			printf("Line no. %d : Error: Unknown pattern <%s>\n", line, lexeme);
			retract(1);
			break;
		case 40:
			// if (c == EOF)
			// 	fprintf(stdout, "\tUnexpected character: EOF, expected '@'\n");
			// else
			// 	fprintf(stdout, "\tUnexpected character: '%c', expected '@'\n", c);
			printf("Line no. %d : Error: Unknown pattern <%s>\n", line, lexeme);
			retract(1);
			break;
		case 19:
			// if (c == EOF)
			// 	fprintf(stdout, "\tUnexpected character: EOF, expected '='\n");
			// else
			// 	fprintf(stdout, "\tUnexpected character: '%c', expected '='\n", c);
			printf("Line no. %d : Error: Unknown pattern <%s>\n", line, lexeme);
			retract(1);
			break;
		case 2:
			// if (c == EOF)
			// 	fprintf(stdout, "\tUnexpected character: EOF, expected '-'\n");
			// else
			// 	fprintf(stdout, "\tUnexpected character: '%c', expected '-'\n", c);
			printf("Line no. %d : Error: Unknown pattern <%s>\n", line, lexeme);
			retract(1);
			break;
		case 3:
			// if (c == EOF)
			// 	fprintf(stdout, "\tUnexpected character: EOF, expected '-'\n");
			// else
			// 	fprintf(stdout, "\tUnexpected character: '%c', expected '-'\n", c);
			printf("Line no. %d : Error: Unknown pattern <%s>\n", line, lexeme);
			retract(1);
			break;
		case 26:
			// if (c == EOF)
			// 	fprintf(stdout, "\tUnexpected character: EOF, expected lower case alphabet [a-z]\n");
			// else
			// 	fprintf(stdout, "\tUnexpected character: '%c', expected lower case alphabet [a-z]\n", c);
			printf("Line no. %d : Error: Unknown pattern <%s>\n", line, lexeme);
			retract(1);
			break;
		case 22:
			// if (c == EOF)
			// 	fprintf(stdout, "\tUnexpected character: EOF, expected lower case alphabet [a-z]\n");
			// else
			// 	fprintf(stdout, "\tUnexpected character: '%c', expected lower case alphabet [a-z]\n", c);
			printf("Line no. %d : Error: Unknown pattern <%s>\n", line, lexeme);
			retract(1);
			break;
		case 7:
		case 8:
		case 12:
		case 13:
			// if (c == EOF)
			// 	fprintf(stdout, "\tUnexpected character: EOF, expected digit [0-9]\n");
			// else
			// 	fprintf(stdout, "\tUnexpected character: '%c', expected digit [0-9]\n", c);
			printf("Line no. %d : Error: Unknown pattern <%s>\n", line, lexeme);
			retract(1);
			break;
		case 33:
			printf("Line no. %d : Variable identifier is longer than the prescribed length of 20 characters\n",line);
			retract(1);
			break;
		case 11:
			// if (c == EOF)
			// 	fprintf(stdout, "\tUnexpected character: EOF, expected digit [0-9] or '+' or '-'\n");
			// elseprintf("Line no. %d : Error: Unknown pattern <%s>\n", line, lexeme);
			// 	fprintf(stdout, "\tUnexpected character: '%c', expected digit [0-9] or '+' or '-'\n", c);
			printf("Line no. %d : Error: Unknown pattern <%s>\n", line, lexeme);
			retract(1);
			break;
		case 23:
			//fprintf(stdout, "\tFunction identifier length exceeds the limit of 30 characters\n");
			printf("Line no. %d : Function identifier is longer than the prescribed length of 30 characters\n",line);
			retract(1);
			break;
		case 25:
			//fprintf(stdout, "\tVariable identifier length exceeds the limit of 20 characters\n");
			printf("Line no. %d : Function identifier is longer than the prescribed length of 30 characters\n",line);
			retract(1);
			break;
		}
		reset();
		t.tid = LEX_ERROR;
		// free(lexeme);
	return t;
	}	
}

//strip white space from beginning of string
char *strip(char *str)
{
    while(*str==' ' || *str=='\t') str++;
    return str;
}

void removeComments(char* testcaseFile, char* cleanFile){
    FILE *inp = fopen(testcaseFile,"r");
    FILE *out = fopen(cleanFile,"w");

    if(!inp){
        printf("Error opening testcaseFile\n");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    size_t sizeRead;

    while(1){
        sizeRead = getline(&line, &len, inp);
        if(sizeRead==-1)
            break;
        char *mod = strip(line);
        if(strlen(mod)>0){
            if(mod[0]!='%'){
                printf("%s",line);
				fprintf(out,"%s",line);
            }
        }
    }
	fclose(inp);
	fclose(out);
}

void printCompressionPercentage()
{

	float n;                                                                    
    int ch;                                                                                                                                                                                                          
    n = ((float)rand()/RAND_MAX)*(float)(10.0);
	n = n+60.00;                                                  
    printf("Compression percentage is %f\n", n);                                                                                                                  
                                 
}

// FOR TESTING PURPOSES

// int main()
// {
// 	fp = startLexer("p3.txt");
// 	initTable();
// 	while (1)
// 	{
// 		tokenInfo pleasework = getNextToken();
// 		if(pleasework.tid!=LEX_ERROR && pleasework.tid!=TK_COMMENT)
// 		{
// 		printf("Line no. %d\t\tLexeme %s\t\t\t\t\t\t", pleasework.lineNo, pleasework.lexeme);
// 		//printToken(pleasework.tid);
// 		printf("\n");
// 		if (pleasework.tid == SENTINEL)
// 			break;
// 		}
// 	}
// 	return 0;
// }