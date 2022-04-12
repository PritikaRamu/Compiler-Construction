#include "symbolTable.h"

//

funList* functionList = NULL;
int GLOBAL_WIDTH = 0;
int FUNCTION_RANK = 0;

int hash(char* key){
    int len = strlen(key);
    unsigned long hashed=7;
    for(int i=0; i<len; i++){
        hashed = hashed*31 + key[i];
    }
    return hashed%TABLE_SLOTS;
}

recordField* createFieldList(ast* ast, int* offset){
    recordField* fields = (recordField*)malloc(sizeof(recordField));
    if(ast->nodeType == INTEGER){
        fields->offset = *offset;
        (*offset) += INT_WIDTH;
        fields->width = INT_WIDTH;
        fields->token = (tokenInfo*)malloc(sizeof(tokenInfo));
        fields->token->tid = ast->symbol;
        fields->token->lexeme = ast->lex;
        fields->token->lineNo = ast->line;
        fields->type = INT_TYPE;
    }
    else{
        fields->offset = *offset;
        (*offset) += REAL_WIDTH;
        fields->width = REAL_WIDTH;
        fields->token = (tokenInfo*)malloc(sizeof(tokenInfo));
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
    ru->token = (tokenInfo*)malloc(sizeof(tokenInfo));
    ru->token->tid = ast->symbol;
    ru->token->lexeme = ast->nextSibling->lex;
    ru->token->lineNo = ast->line;
    ru->fieldList = NULL;

    while(head){
        ru->width += head->width;
        head = head->next;
    }
    return ru;
}

identifierNode* createINode(ast* id, ast* func, NodeType type, bool is_global, int*offset){
    identifierNode* iden = (identifierNode*)malloc(sizeof(identifierNode));
    iden->function = (tokenInfo*)malloc(sizeof(tokenInfo));
    iden->function->lexeme = func->lex;
    iden->function->lineNo = func->line;
    iden->function->tid = func->symbol;
    iden->type = type;
    if(iden->type == RECORD_TYPE || iden->type == UNION_TYPE){
        iden->offset = *offset;
        if(iden->recordList){
            iden->width = iden->recordList->width;
            (*offset) += iden->recordList->width;
        }
    }
    else{
        iden->offset = *offset;
        if(iden->type == INT_TYPE){
            iden->width = INT_WIDTH;
            (*offset) += INT_WIDTH;
        }
        else{
            iden->width = REAL_WIDTH;
            (*offset) += REAL_WIDTH;
        }
    }
    iden->global = is_global;
    return iden;
}

parameters* createIPParams(ast* ast, NodeType type){
    parameters* p = (parameters*)malloc(sizeof(parameters));
    p->nodeType = type;
    p->next = NULL;
    p->token = (tokenInfo*)malloc(sizeof(tokenInfo));
    p->token->lexeme = ast->lex;
    p->token->lineNo = ast->line;
    p->token->tid = ast->symbol;
    return p;
}

bool runode_check(void* node1, void* node2){
    if(strcmp(((recordUnionNode*)node1)->token->lexeme,((recordUnionNode*)node2)->token->lexeme)==0){
        return true;
    }
    else{
        return false;
    }
}

void* retrieve(symbol_Table* st, void* node, NodeType type){
    if(!node) return NULL;
    int key;
    subTable* t;
    switch(type){
        case ID:{
            key = hash(((identifierNode*)node)->token->lexeme);
            t = st->IdentifierTable;
            break;
        }
        case RECORD_OR_UNION:{
            key = hash(((recordUnionNode*)node)->token->lexeme);
            t = st->RecordUnionTable;
            break;
        }
        case FUNCTION_SEQ:{
            key = hash(((functionNode*)node)->token->lexeme);
            t = st->FunctionTable;
            break;
        }
    }
    if(t->table[key].node){
        Entry* entry = &(t->table[key]);
        switch(type){
            case ID:{
                while(entry){
                    // if(inode_check(entry->node, node)){
                    //     return entry->node;
                    // }
                    printf("TODO in retrv fun\n");
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
                    // if(fnode_check(entry->node, node)){
                    //     return entry->node;
                    // }
                    // entry = entry->next;
                }
            }
        }
        return NULL;
    }
}

void insert(symbol_Table* st, void* node, NodeType type){
    int key;
    subTable* t;
    switch(type){
        case ID:{
            key = hash(((identifierNode*)node)->token->lexeme);
            t = st->IdentifierTable;
            break;
        }
        case RECORD_OR_UNION:{
            key = hash(((recordUnionNode*)node)->token->lexeme);
            t = st->RecordUnionTable;
            break;
        }
        case FUNCTION_SEQ:{
            key = hash(((functionNode*)node)->token->lexeme);
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
        //printf("inside root while %s\n",root->lex);
        ast* child = root->firstChild;
        while(child){
            //printf("inside child while %s\n",child->lex);
            int offset = 0;
            if(child->nodeType == RECORD_OR_UNION && child->firstChild->nodeType!=ID){
                curr_ast = child->firstChild;
                fields = createFieldList(curr_ast, &offset);
                curr_ast = curr_ast->nextSibling;
                curr_field = fields;
                while(curr_ast){
                    curr_field->next = createFieldList(curr_ast, &offset);
                    curr_ast = curr_ast->nextSibling;
                    curr_field = curr_field->next;
                }
                curr_field->next = NULL;
                recordUnionNode* new = createRUNode(child, fields);
                recordUnionNode* check = retrieve(SymbolTable, new, RECORD_OR_UNION);
                if(check){
                    printf("redeclaration\n");
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

void createFTable(ast* root){
    root = root->firstChild;
    while(root){
        int offset = 0;
        functionNode* func = (functionNode*)malloc(sizeof(functionNode));
        ast* child = root->firstChild;
        func->rank = FUNCTION_RANK;
        FUNCTION_RANK++;
        func->token = (tokenInfo*)malloc(sizeof(tokenInfo));
        func->token->lexeme = root->lex;
        func->token->lineNo = root->line;
        func->token->tid = root->symbol;    
        parameters* curr_ip = NULL;
        parameters* curr_op = NULL;
        ast* pars = NULL;
        int width = 0;
        while(child){
            if(child->nodeType == INPUT_PARAMETERS){
                printf("inside if\n");
                pars = child->firstChild;
                while(pars){
                    identifierNode* id;
                    switch(pars->symbol){
                        case TK_INT:{
                            id = createINode(pars->firstChild, child->parent, INT_TYPE, false, &offset); //TODO
                            printf("IN TK INT\n");
                            break;
                        }
                        case TK_REAL:{
                            id = createINode(pars->firstChild, child->parent, REAL_TYPE, false, &offset); //TODO
                            printf("IN TK REAL\n");
                            break;
                        }
                        case TK_RECORD:{
                            id = createINode(pars->firstChild, child->parent, RECORD_TYPE, false, &offset); //TODO
                            printf("IN TK RECORD\n");
                            break;
                        }
                        case TK_UNION:{
                            id = createINode(pars->firstChild, child->parent, UNION_TYPE, false, &offset); //TODO
                            printf("IN TK UNION\n");
                            break;
                        }
                    }
                    identifierNode* check = (identifierNode*)retrieve(SymbolTable,id,ID);
                    // if(check){
                    //     printf("redeclaration");
                    // }
                    // else{
                    //     printf("insert\n");
                    //     insert(SymbolTable, id, ID);
                    // }
        //             if(!curr_ip){
        //                 func->ipParams = createIPParams(pars->firstChild,pars->nodeType); //TODO
        //                 curr_ip = func->ipParams;
        //             }
        //             else{
        //                 curr_ip->next = createIPParams(pars->firstChild,pars->nodeType);
        //                 curr_ip = curr_ip->next;
        //             }
        //             width +=id->width;
                    pars = pars->nextSibling;                 
                }
            }
        //     else if(child->nodeType == OUTPUT_PARAMETERS){
        //         pars = child->firstChild;
        //         while(pars){
        //             identifierNode* id;
        //             switch(pars->symbol){
        //                 case TK_INT:{
        //                     id = createINode(pars->firstChild, child->parent, INT_TYPE, false, &offset); //TODO
        //                     break;
        //                 }
        //                 case TK_REAL:{
        //                     id = createINode(pars->firstChild, child->parent, REAL_TYPE, false, &offset); //TODO
        //                     break;
        //                 }
        //                 case TK_RECORD:{
        //                     id = createINode(pars->firstChild, child->parent, RECORD_TYPE, false, &offset); //TODO
        //                     break;
        //                 }
        //                 case TK_UNION:{
        //                     id = createINode(pars->firstChild, child->parent, UNION_TYPE, false, &offset); //TODO
        //                     break;
        //                 }
        //             }
        //             identifierNode* check = (identifierNode*)retrieve(SymbolTable,id,ID);
        //             if(check){
        //                 printf("redeclaration");
        //             }
        //             else{
        //                 insert(SymbolTable, id, ID);
        //             }
        //             if(!curr_op){
        //                 func->opParams = createIPParams(pars->firstChild,pars->nodeType); //TODO
        //                 curr_ip = func->opParams;
        //             }
        //             else{
        //                 curr_op->next = createIPParams(pars->firstChild,pars->nodeType);
        //                 curr_op = curr_op->next;
        //             }
        //             width +=id->width;
        //             pars = pars->nextSibling;
        //         }
        //     }
            child = child->nextSibling;
        //     functionNode* check = (functionNode*)retrieve(SymbolTable, func, FUNCTION_SEQ);
        //     if(check){
        //         printf("redecl");
        //     }
        //     else{
        //         func->tmpVars = 0;
        //         insert(SymbolTable, func, FUNCTION_SEQ);
        //     }
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
    SymbolTable = (symbol_Table*)malloc(sizeof(symbolTable));
    SymbolTable->IdentifierTable = initSubTable();
    SymbolTable->FunctionTable = initSubTable();
    SymbolTable->RecordUnionTable = initSubTable();
	createRUtable(ast);
	printf("record table done\n");
	createFTable(ast);
	printf("function table done\n");
	// generate_I_table(ast, NULL);
	// printf("identifier table done\n");
}

void printRecordTable(subTable* rec_table){
    int i;
    Entry* entry;
    recordUnionNode* record;
	printf("#%-30s %-30s %-30s\n", "Lexeme", "Type", "Width");
	for(i=0; i<TABLE_SLOTS; i++){
		entry = &(rec_table->table[i]);
		while(entry != NULL){
			record = (recordUnionNode*)(entry->node);
			if(record != NULL) {
                printf("%-30s",record->token->lexeme);
                recordField* fields = record->fieldList;
				char fieldsstring[100] = "";
				while (fields != NULL) {
					if(fields->width/2 == 2)
					{
						strcat(fieldsstring,"int,");
					}
					else
					{
						strcat(fieldsstring,"real,");
					}
                    fields = fields -> next;
                }
				printf("%-30s",fieldsstring);
				printf("%d\n",record->width/2);
                printf("-------------------------------\n");
			}
			entry = entry->next;
		}
	}
}

void printFunctionTable(subTable* fun_table){
    int i;
    Entry* entry;
    functionNode* fun_node;
	for(i=0; i<TABLE_SLOTS; i++){
		entry = &(fun_table->table[i]);
		while(entry != NULL){
			fun_node = (functionNode*)(entry->node);
			if(fun_node!=NULL){
                printf("%-30s %d\n",fun_node->token->lexeme,fun_node->width/2);
                printf("-------------------------------\n");
			}
			entry = entry->next;
		}
	}
}