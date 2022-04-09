#include "symbolTable.h"

//

funList* functionList = NULL;
int GLOBAL_WIDTH = 0;
int FUNCTION_RANK = 0;

int hashcode(char* key){
    int len = strlen(key);
    unsigned long hashed=7;
    for(int i=0; i<len; i++){
        hashed = hashed*31 + key[i];
    }
    return hashed%TABLE_SLOTS;
}

recordField* createFieldList(ast* ast, int offset){
    recordField* fields = (recordField*)malloc(sizeof(recordField));
    if(ast->nodeType == INTEGER){
        fields->offset = offset;
        offset += INT_WIDTH;
        fields->width = INT_WIDTH;
        fields->token->tid = ast->symbol;
        fields->token->lexeme = ast->lex;
        fields->token->lineNo = ast->line;
        fields->type = INT_TYPE;
    }
    else{
        fields->offset = offset;
        offset += REAL_WIDTH;
        fields->width = REAL_WIDTH;
        fields->token->tid = ast->symbol;
        fields->token->lexeme = ast->lex;
        fields->token->lineNo = ast->line;
        fields->type = REAL_TYPE;
    }
    return fields;
}

recordUnionNode* createRUNode(ast* ast, recordField* fields){
    recordUnionNode* ru = (recordUnionNode*)malloc(sizeof(recordUnionNode));
    ru->width = 0;
    ru->fieldList = fields;
    recordField* head = fields;
    ru->token->tid = ast->symbol;
    ru->token->lexeme = ast->lex;
    ru->token->lineNo = ast->line;
    ru->fieldList = NULL;

    while(head){
        ru->width += head->width;
        head = head->next;
    }
    return ru;
}

void* retrieve(symbolTable* st, void* node, NodeType type){
    if(!node) return NULL;
    int key;
    subTable* t;
    switch(type){
        case ID:{
            key = hashcode(((identifierNode*)node)->token->lexeme);
            t = st->IdentifierTable;
            break;
        }
        case RECORD_OR_UNION:{
            key = hashcode(((recordUnionNode*)node)->token->lexeme);
            t = st->RecordUnionTable;
            break;
        }
        case FUNCTION_SEQ:{
            key = hashcode(((functionNode*)node)->token->lexeme);
            t = st->FunctionTable;
            break;
        }
    }
    if(t->table[key].node){
        Entry* entry = &(t->table[key]);
        //??????? what is the error T_T
    }
}

void insert(symbolTable* st, void* node, NodeType type){
    int key;
    subTable* t;
    switch(type){
        case ID:{
            key = hashcode(((identifierNode*)node)->token->lexeme);
            t = st->IdentifierTable;
            break;
        }
        case RECORD_OR_UNION:{
            key = hashcode(((recordUnionNode*)node)->token->lexeme);
            t = st->RecordUnionTable;
            break;
        }
        case FUNCTION_SEQ:{
            key = hashcode(((functionNode*)node)->token->lexeme);
            t = st->FunctionTable;
            break;
        }
    }
    if(retrieve)
}