/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/

#include "lexer.h"

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
	{"_main",TK_MAIN},
	{"global",TK_GLOBAL},
	{"parameter",TK_PARAMETER},
	{"list",TK_LIST},
	{"input",TK_INPUT},
	{"output",TK_OUTPUT},
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

int noEle = 0;

int hashcode(char* key){
    int len = strlen(key);
    unsigned long hashed=7;
    for(int i=0; i<len; i++){
        hashed = hashed*31 + key[i];
    }
    return hashed%SIZE;
}

//to check if key is already present in the hash table
bool exists(struct table *list, char* key)
{
	struct table *temp = list;
	int pos = 0;
	while (temp){
		if (strcmp(temp->key,key)==0){
			return true;
		}
		temp = temp->next;
	}
	return false;
}

void insertID(char* key, g_Term value)
{	
	int index = hashcode(key);  
 	struct table *list = (struct table*) symbolTable[index].head;
 
	if (!list) //if no entries are present for the given index
    {
		struct table *entry = (struct table*) malloc(sizeof(struct table));
		entry->key = key;
		entry->token = value;
		entry->next = NULL;
		symbolTable[index].head = entry;
		noEle++;
	}
    else{ //entries are present for the given index
		if(!exists(list, key)){ 
			struct table *entry = (struct table*) malloc(sizeof(struct table));
			entry->key = key;
			entry->token = value;
			entry->next = symbolTable[index].head;
			symbolTable[index].head = entry;
			noEle++;
 
		}
        else{
        	printf("Key already present in symbol table");
		}
	}
}

g_Term getToken(char* key){
	int index = hashcode(key);
	struct table *list = symbolTable[index].head;
	while(list){
		if (strcmp(list->key,key)==0){
			return list->token;
		}
		list = list->next;
	}
	return -1;
}
//initialize and populate table with keywords
void initTable()
{
	for(int i = 0; i < SIZE; i++)
    {
		symbolTable[i].head = NULL;
	}
	for(int i=0; i<28; i++){
         insertID(keywords[i].key,keywords[i].value);
    }
}

void display()
{
	for (int i = 0; i < SIZE; i++){
		struct table *temp = symbolTable[i].head;
		if (temp == NULL) {
			printf("%d: XXX\n", i);
		}
        else{
			printf("%d: ", i);
			while(temp != NULL){
				printf("key= %s  value= %d\t", temp->key, temp->token);
				temp = temp->next;
			}
			printf("\n");
		}
	}
}

// FOR TESTING PURPOSES

// void main(){
// 	initTable();
// 	display();
// 	printf("%d",getToken("preetika"));
// }