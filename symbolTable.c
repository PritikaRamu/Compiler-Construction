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

bool runode_check(void* node1, void* node2){
    if(strcmp(((recordUnionNode*)node1)->token->lexeme,((recordUnionNode*)node2)->token->lexeme)==0){
        return true;
    }
    else{
        return false;
    }
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
        switch(type){
            case ID:{
                while(entry){
                    if(inode_check(entry->node, node)){
                        return entry->node;
                    }
                    entry = entry->next;
                }
                break;
            }
            case RECORD_OR_UNION:{
                while(entry){
                    if(runode_check(entry->node, node)){
                        return entry->node;
                    }
                    entry = entry->next;
                }
            }
            case FUNCTION_SEQ:{
                while(entry){
                    if(fnode_check(entry->node, node)){
                        return entry->node;
                    }
                    entry = entry->next;
                }
            }
        }
        return NULL;
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
    if(retrieve(st, node, type)){
        return;
    }
    void* e = t->table->node;
    void* check = t->table->node;
    if(!check){
        t->table[key].next = NULL;
        t->table[key].key = key;
        t->table[key].node = node;
    }
    else{
        Entry* entry = &(t->table[key]);
        while(entry->next){
            entry = entry->next;
        }
        entry->next = (Entry*)malloc(sizeof(Entry));
        entry->next->key = key;
        entry->next->node = node;
        entry->next->next = NULL;
    }
}

void createRUtable(ast* root){
    root = root->firstChild;
    recordField* curr_field = NULL;
    recordField* fields = NULL;
    ast* curr_ast = NULL;
    while(root){
        ast* child = root->firstChild;
        while(child){
            int offset = 0;
            if(child->nodeType == RECORD_OR_UNION && child->firstChild!=ID){
                curr_ast = child->firstChild;
                fields = createFieldList(curr_ast, offset);
                curr_ast = curr_ast->nextSibling;
                curr_field = fields;
                while(curr_ast){
                    curr_field->next = createFieldList(curr_ast, offset);
                    curr_ast = curr_ast->nextSibling;
                    curr_field = curr_field->next;
                }
                curr_field->next = NULL;
                recordUnionNode* new = createRUNode(child, fields);
                recordUnionNode* check = retrieve(SymbolTable, new, RECORD_OR_UNION);
                if(check){
                    printf("redeclaration");
                    //TODO FILE STUFF
                }
                else{
                    insert(SymbolTable, new, RECORD_OR_UNION);
                }
            }
            child = child->nextSibling;
        }
        root = root->nextSibling;
    }
}

subTable* initSubTable(){
    subTable* st = (subTable*)malloc(sizeof(subTable));
    st->table = (Entry*)malloc(TABLE_SLOTS*sizeof(Entry));
    return st;
}

void initializeSymbolTable(ast* ast) {
    SymbolTable = (symbolTable*)malloc(sizeof(symbolTable));
    SymbolTable->IdentifierTable = initSubTable();
    SymbolTable->FunctionTable = initSubTable();
    SymbolTable->RecordUnionTable = initSubTable();
	generate_R_table(ast, NULL);
	printf("record table done\n");
	// generate_F_table(ast, NULL);
	// printf("function table done\n");
	// generate_I_table(ast, NULL);
	// printf("identifier table done\n");
}