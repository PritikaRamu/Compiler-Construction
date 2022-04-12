#include "ast.h"

#define REAL_WIDTH 8
#define INT_WIDTH 4
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
    tokenInfo* token;
    struct recordField* next; 
}recordField;

typedef struct record_or_union_node{
    bool isRecord;
    int width;
    tokenInfo* token;
    struct recordField* fieldList;
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
}identifierNode;

typedef struct parameters{
    tokenInfo* token;
    NodeType nodeType;
    struct parameters* next;
}parameters;

typedef struct function_node{
    tokenInfo* token;
    parameters* ipParams;
    parameters* opParams;
    int width;
    int tmpVars; //keep track of no. of temp vars needed for code gen
    int rank;
    int numOp;
}functionNode;

typedef struct function_list{
    tokenInfo* funToken;
    identifierNode* idList;
    struct function_list* next;
}funList;

symbol_Table* SymbolTable;