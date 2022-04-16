#include "ast.h"

#define REAL_WIDTH 4
#define INT_WIDTH 2
#define TABLE_SLOTS 100

//TODO IMPLEMENT ERROR HANDLING VIA FILE
//TODO check if record has min 2 fields
//REMINDER feildlist from sankalp is similar to our RecordField

typedef enum Type{
    INT_TYPE,
    REAL_TYPE,
    RECORD_TYPE,
    UNION_TYPE
}Type;

typedef struct entry{
    int key;
	void* node; 
    struct entry* next;
}Entry;

typedef struct subTable{
	Entry* table;
}subTable;

typedef struct symbol_table{
    subTable* IdentifierTable;
    subTable* FunctionTable;
    subTable* RecordUnionTable; 
}symbol_Table;

typedef struct recordField{
    int width;
    int offset;
    Type type;
    tokenInfo* token;  //token->lexeme has identifier name
    struct recordField* next;
    char* recordName;   //have the record name here 
}recordField;

typedef struct record_or_union_node{
    bool is_union;
    int width;
    tokenInfo* token;
    struct recordField* fieldList;
    char* recordName;
    bool isVariant; 
}recordUnionNode;

typedef struct identifierNode{
    int width;
    int offset;
    Type type;
    tokenInfo* token;
    tokenInfo* function; //check if needed
    recordUnionNode* recordList;
    recordUnionNode* unionList;
    bool global;
    bool assigned;
    struct identifierNode* next;
    bool isRecordField;
    char* recordName;
    bool isUnion;
    struct identifierNode* aliasList;
    struct identifierNode* idList;
    bool isIP;
    bool isOP;
}identifierNode;

typedef struct parameters{
    tokenInfo* token;
    NodeType nodeType;
    struct parameters* next;
    bool is_union;
}parameters;

typedef struct function_node{
    tokenInfo* token;
    parameters* ipParams;
    parameters* opParams;
    int width;
    int tmpVars; //keep track of no. of temp vars needed for code gen
    int rank;
    int numOp;
    identifierNode* idList;
}functionNode;

typedef struct function_list{
    tokenInfo* funToken;
    identifierNode* idList;
    struct function_list* next;
}funList;

symbol_Table* SymbolTable;

subTable* firstPass;
subTable* aliasTable;

char redeclaration_error_array[100][200];

int redeclaration_error_index;