#include "symbolTable.h"

funList *functionList = NULL;
int GLOBAL_WIDTH = 0;
int FUNCTION_RANK = 0;

int hash(char *key)
{
    int len = strlen(key);
    unsigned long hashed = 7;
    for (int i = 0; i < len; i++)
    {
        hashed = hashed * 31 + key[i];
    }
    return hashed % TABLE_SLOTS;
}

recordField* createFieldList(ast *curr_ast, int *offset)
{
    
    ast *iterator = curr_ast;
    recordField *head = NULL;
    recordField *tail = NULL;

    while (iterator != NULL)
    {
        recordField *fields = (recordField *)malloc(sizeof(recordField));
        if (iterator->nodeType == INTEGER)
        {

            fields->offset = *offset;
            (*offset) += INT_WIDTH;
            fields->width = INT_WIDTH;
            fields->token = (tokenInfo *)malloc(sizeof(tokenInfo));
            fields->token->tid = iterator->symbol;
            fields->token->lexeme = iterator->firstChild->lex;
            fields->token->lineNo = iterator->line;
            fields->type = INT_TYPE;
            fields->recordName = NULL;
        }
        else if (iterator->nodeType == REAL)
        {
            fields->offset = *offset;
            (*offset) += REAL_WIDTH;
            fields->width = REAL_WIDTH;
            fields->token = (tokenInfo *)malloc(sizeof(tokenInfo));
            fields->token->tid = iterator->symbol;
            fields->token->lexeme = iterator->firstChild->lex;
            fields->token->lineNo = iterator->line;
            fields->type = REAL_TYPE;
            fields->recordName = NULL;
        }
        else if (iterator->nodeType == RECORD_OR_UNION)
        {
            fields->offset = *offset;
            //just creating a dummy node to cheEnterck entry in record table
            // recordUnionNode *ru = (recordUnionNode *)malloc(sizeof(recordUnionNode));
            // ru->width = 0;
            // ru->fieldList = NULL;
            // ru->token = (tokenInfo *)malloc(sizeof(tokenInfo));
            // ru->token->tid = -1;
            // ru->token->lexeme = iterator->lex;
            //ru->token->lineNo = curr_ast->line;
            identifierNode* temp = (identifierNode*)malloc(sizeof(identifierNode));
            temp->function = (tokenInfo *)malloc(sizeof(tokenInfo));
            temp->function->lexeme = iterator->lex;

            identifierNode *x = (identifierNode *)retrieveFake(aliasTable,temp,false,true);
            if (x == NULL)
            {
                printf("Using a record as field that has not been declared on line no. %d\n", iterator->line);
            }
            else
            {
                fields->token = (tokenInfo *)malloc(sizeof(tokenInfo));
                fields->token->tid = iterator->symbol;
                fields->token->lexeme = iterator->firstChild->lex;
                fields->token->lineNo = iterator->firstChild->line;
                fields->type = (iterator->is_union) ? UNION_TYPE : RECORD_TYPE;
                fields->recordName = x->token->lexeme;
               // printf("dont know what to do herererererere %s %s\n",fields->recordName,fields->token->lexeme);
            }
        }
        if (head)
        {
            tail->next = fields;
            tail = tail->next;
        }
        else
        {
            head = fields;
            tail = head;
        }
        iterator = iterator->nextSibling;
    }

    return head;
}

recordUnionNode *createRUNode(ast *curr_ast, recordField *fields)
{
    recordUnionNode *ru = (recordUnionNode *)malloc(sizeof(recordUnionNode));
    ru->width = 0;
    ru->fieldList = fields;
    recordField *head = fields;
    ru->token = (tokenInfo *)malloc(sizeof(tokenInfo));
    ru->token->tid = curr_ast->symbol;
    ru->token->lexeme = curr_ast->lex;
    ru->token->lineNo = curr_ast->line;
    ru->is_union = curr_ast->is_union;

    while (head)
    {
        ru->width += head->width;
        head = head->next;
    }
    return ru;
}

functionNode *createFNode(ast* root)
{
    functionNode *func = (functionNode *)malloc(sizeof(functionNode));
    func->rank = FUNCTION_RANK;
    FUNCTION_RANK++;
    func->token = (tokenInfo *)malloc(sizeof(tokenInfo));
    func->token->lexeme = root->lex;
    func->token->lineNo = root->line;
    func->token->tid = root->symbol;
    return func;
}

parameters *createIPParams(ast *ast, NodeType type)
{
    parameters *p = (parameters *)malloc(sizeof(parameters));
    p->nodeType = type;
    p->next = NULL;
    p->token = (tokenInfo *)malloc(sizeof(tokenInfo));
    p->token->lexeme = ast->lex;
    p->token->lineNo = ast->line;
    p->token->tid = ast->symbol;
    p->is_union = ast->is_union;
    return p;
}

bool runode_check(void *node1, void *node2)
{
    // if (strcmp(((recordUnionNode *)node1)->token->lexeme, ((recordUnionNode *)node2)->token->lexeme) == 0)
    // {
    //     return true;
    // }
    // else
    // {
    //     return false;
    // }

    return (strcmp(((recordUnionNode *)node1)->token->lexeme, ((recordUnionNode *)node2)->token->lexeme) == 0) ? true : false;
}

bool first_check(char *node1, char *node2)
{
    // if (strcmp(node1,node2) == 0)
    // {
    //     return true;
    // }
    // else
    // {
    //     return false;
    // }

    return (strcmp(node1,node2) == 0) ? true : false;
}

bool fnode_check(void *node1, void *node2)
{
    // if (strcmp(((functionNode *)node1)->token->lexeme, ((functionNode *)node2)->token->lexeme) == 0)
    // {
    //     return true;
    // }
    // else
    // {
    //     return false;
    // }

    return (strcmp(((functionNode *)node1)->token->lexeme, ((functionNode *)node2)->token->lexeme) == 0) ? true:false;
}

//need to check this function
bool inode_check(void *node1, void *node2)
{
    identifierNode *temp1 = (identifierNode *)node1;
    identifierNode *temp2 = (identifierNode *)node2;
    if (temp2->global && temp1->global)
    {
        //both are global and lexemes are same
        if (strcmp(temp2->token->lexeme, temp1->token->lexeme) == 0)
        {
            return true;    //node already exists in symbol table
        }
    }
    // else if ((!(temp2->global) && (temp1->global))||(!(temp1->global) && (temp2->global)))
    // {
        //one is global and the other is local
        // if (strcmp(temp1->token->lexeme, temp2->token->lexeme))
        // {
            // return false;
        // }
    // }
    else if (strcmp(temp1->function->lexeme,temp2->function->lexeme)==0)
    { 
        if (strcmp(temp1->token->lexeme, temp2->token->lexeme) == 0)
        {
            return true;
        }
    }
    return false;
}

void *retrieve(symbol_Table *st, void *node, NodeType type)
{
    if (node==NULL)
        return NULL;
    int key;
    subTable *t;
    //return NULL;
    switch (type)
    {
    case ID:
    {
        key = hash(((identifierNode *)node)->token->lexeme);
        //printf("Lexeme for retrieve is %s\n",((identifierNode *)node)->token->lexeme);
        t = st->IdentifierTable;
        break;
    }
    case RECORD_OR_UNION:
    {
        key = hash(((recordUnionNode *)node)->token->lexeme);
        // printf("Lexeme for retrieve is %s\n",((recordUnionNode *)node)->token->lexeme);
        t = st->RecordUnionTable;
        break;
    }
    case FUNCTION_SEQ:
    {
        key = hash(((functionNode *)node)->token->lexeme);
        //printf("Lexeme for retrieve is %s\n",((functionNode *)node)->token->lexeme);
        t = st->FunctionTable;
        break;
    }
    }
 
    if (t->table[key].node)
    {
        
        Entry *entry = &(t->table[key]);
        switch (type)
        {
        case ID:
        {
            while (entry)
            {
                if (inode_check(entry->node, node))
                {
                    return entry->node;
                }
                entry = entry->next;
               
            }
            break;
        }
        case RECORD_OR_UNION:
        {
            while (entry)
            {
                if (runode_check(entry->node, node))
                {   
                    return entry->node;
                }
                entry = entry->next;
            }
        }
        case FUNCTION_SEQ:
        {
            while (entry)
            {
                if (fnode_check(entry->node, node))
                {
                    return entry->node;
                }
                entry = entry->next;
            }
        }
        }
        return NULL;
    }

}

void insert(symbol_Table *st, void *node, NodeType type)
{
    int key;
    subTable *t;
    switch (type)
    {
        case ID:
        {
            key = hash(((identifierNode *)node)->token->lexeme);            
            t = st->IdentifierTable;
            break;
        }
        case RECORD_OR_UNION:
        {
            key = hash(((recordUnionNode *)node)->token->lexeme);
            // printf("key is %d\n",key);
            t = st->RecordUnionTable;
            break;
        }
        case FUNCTION_SEQ:
        {
            key = hash(((functionNode *)node)->token->lexeme);
            t = st->FunctionTable;
            break;
        }
    }
    
    if (retrieve(st, node, type))
    {
        return;
    }
    void *e = t->table->node;
    void* check = t->table[key].node;
    if (!check)
    {
        //printf("inserting NULL %s\n",((identifierNode *)node)->token->lexeme);
        t->table[key].next = NULL;
        t->table[key].key = key;
        t->table[key].node = node;
    }
    else
    {
        Entry *entry = &(t->table[key]);
        while (entry->next)
        {
            entry = entry->next;
        }
        Entry* temp = (Entry *)malloc(sizeof(Entry));
        temp->key = key;
        temp->node = node;
        temp->next = NULL;
        entry->next = temp;
        //printf("inserting NOT NULL%s\n",((identifierNode *)node)->token->lexeme);
    }
}


identifierNode* createINode(ast* id, ast* func, Type type, bool is_global, int*offset){
    identifierNode* iden = (identifierNode*)malloc(sizeof(identifierNode));
    iden->function = (tokenInfo*)malloc(sizeof(tokenInfo));
    iden->function->lexeme = func->lex;
    iden->function->lineNo = func->line;
    iden->function->tid = func->symbol;
    iden->token = (tokenInfo *)malloc(sizeof(tokenInfo));
    iden->token->lexeme = id->lex;
    iden->token->lineNo = id->line;
    iden->token->tid = id->symbol;
    iden->type = type;
    iden->assigned = false;
    iden->offset = *offset;
    iden->isRecordField = false;
    if (iden->type == RECORD_TYPE || iden->type == UNION_TYPE)
    {
        recordUnionNode *temp = (recordUnionNode *)malloc(sizeof(recordUnionNode));
        temp->token = (tokenInfo *)malloc(sizeof(tokenInfo));
        temp->token->lexeme = id->parent->lex;
        iden->recordList = (recordUnionNode *)retrieve(SymbolTable, temp, RECORD_OR_UNION);
        if (iden->recordList)
        {
            iden->width = iden->recordList->width;
            (*offset) += iden->recordList->width;
        }
    }
    else
    {
        if (iden->type == INT_TYPE)
        {
            iden->width = INT_WIDTH;
            (*offset) += INT_WIDTH;
        }
        else
        {
            iden->width = REAL_WIDTH;
            (*offset) += REAL_WIDTH;
        }
    }
    iden->global = is_global;
    return iden;
}

void createRUtable(ast *root)
{
    //taking our third pass through the record table 
    root = root->firstChild;    //first node in FUNCTION_SEQ
    recordField *curr_field = NULL; 
    recordField *fields = NULL;
    ast *curr_ast = NULL;
    while (root)
    {
        //root always points to one of the functions
        ast *child = root->firstChild;
        //child is the first child of the function pointed to by root
        while (child)
        {
            int offset = 0;
            if (child->nodeType == RECORD_OR_UNION && child->firstChild->nodeType != ID)
            {
                // printf("Child node is %s and type is %d\n", child->lex, child->nodeType);
                curr_ast = child->firstChild;
                // printf("current node's lexeme: %s\n", curr_ast->lex);
                fields = createFieldList(curr_ast, &offset);    //iterates and returns a linked list of field nodes
                recordUnionNode* new = createRUNode(child, fields);
                //check if the name exists in the first pass table
                //recordUnionNode* check = retrieve(SymbolTable, new, RECORD_OR_UNION);
                identifierNode* new1 = (identifierNode*)malloc(sizeof(identifierNode));
                new1->token = (tokenInfo*)malloc(sizeof(tokenInfo));
                new1->token->lexeme = curr_ast->lex;
                identifierNode* check = retrieveFake(firstPass, new1, true, false);
                // if(check)
                // {
                    insert(SymbolTable, new, RECORD_OR_UNION);
                  
                // }
            }
            // else if (child->nodeType == DEFINETYPE)
            // {
            //     printf("In definetype, Child node is %s and type is %d\n", child->lex, child->nodeType);
            //     recordUnionNode *ru = (recordUnionNode *)malloc(sizeof(recordUnionNode));
            //     ru->width = 0;
            //     ru->fieldList = NULL;
            //     ru->token = (tokenInfo *)malloc(sizeof(tokenInfo));
            //     ru->token->tid = -1;
            //     ru->token->lexeme = child->firstChild->nextSibling->lex;
            //     // ru->token->lineNo = ast->line;
            //     ru->fieldList = NULL;
            //     recordUnionNode *existing = (recordUnionNode *)retrieve(SymbolTable, ru, RECORD_OR_UNION);
            //     if (existing == NULL)
            //     {
            //         printf("Implicit declaration of %s on line no. %d\n", child->firstChild->nextSibling->lex, child->firstChild->nextSibling->line);
            //     }
            //     else
            //     {
            //         if ((existing->is_union == true && child->firstChild->symbol == TK_RECORD) || (existing->is_union == false && child->firstChild->symbol == TK_UNION))
            //         {
            //             printf("Type Mismatch on line no. %d\n", child->firstChild->line);
            //         }
            //         else
            //         {
            //             recordUnionNode *tdefNode = (recordUnionNode *)malloc(sizeof(recordUnionNode));
            //             tdefNode->is_union = existing->is_union;
            //             tdefNode->width = existing->width;
            //             tdefNode->token = (tokenInfo *)malloc(sizeof(tokenInfo));
            //             tdefNode->token->tid = existing->token->tid;
            //             tdefNode->token->numVal = existing->token->numVal;
            //             tdefNode->token->lineNo = existing->token->lineNo;
            //             tdefNode->token->lexeme = child->firstChild->nextSibling->nextSibling->lex;
            //             printf("In definetype, lexeme being inserted: %s\n", tdefNode->token->lexeme);
            //             insert(SymbolTable, tdefNode, RECORD_OR_UNION);
            //         }
            //     }
            // }
            child = child->nextSibling;
        }
        root = root->nextSibling;
    }
}

identifierNode* retrieveFake(subTable* st, identifierNode* id, bool token, bool alias){

    if(!alias){
        if(token){
            int key = hash(id->token->lexeme);
            if (st->table[key].node)
            {
                Entry *entry = &(st->table[key]);
                while (entry)
                    {
                        if (first_check(((identifierNode*)(entry->node))->token->lexeme, id->token->lexeme))
                        {   
                            return entry->node;
                        }
                        entry = entry->next;
                    }
            }
        }
        else{
            int key = hash(id->function->lexeme);
            if (st->table[key].node)
            {
                Entry *entry = &(st->table[key]);
                while (entry)
                    {
                        if (first_check(((identifierNode*)(entry->node))->token->lexeme, id->function->lexeme))
                        {   
                            return entry->node;
                        }
                        entry = entry->next;
                    }
            }
            return NULL;
        }
    }
    else{
        if(token){
            int key = hash(id->token->lexeme);
            if (st->table[key].node)
            {
                Entry *entry = &(st->table[key]);
                while (entry)
                    {
                        if (first_check(((identifierNode*)(entry->node))->function->lexeme, id->token->lexeme))
                        {   
                            return entry->node;
                        }
                        entry = entry->next;
                    }
            }
        }
        else{
            int key = hash(id->function->lexeme);
            if (st->table[key].node)
            {
                Entry *entry = &(st->table[key]);
                while (entry)
                    {
                        if (first_check(((identifierNode*)(entry->node))->function->lexeme, id->function->lexeme))
                        {   
                            return entry->node;
                        }
                        entry = entry->next;
                    }
            }
        }
    }

    
    
    return NULL;
}

void insertFake(subTable* st, identifierNode* id, bool token){
    int key;
    if(token){
        key = hash(id->token->lexeme);
    }
    else{
        key = hash(id->function->lexeme);
    }
    
    void* check = st->table[key].node;
    if (check==NULL)
    {
        st->table[key].next = NULL;
        st->table[key].key = key;
        st->table[key].node = id;
    }
    else
    {
        Entry *entry = &(st->table[key]);
        while (entry->next!=NULL)
        {
            entry = entry->next;
            
        }
        
        Entry* temp = (Entry *)malloc(sizeof(Entry));
        temp->key = key;
        temp->node = id;
        temp->next = NULL;
        entry->next = temp;
    }
}

void createFirstPass(ast *root)
{
    root = root->firstChild;
    ast *curr_ast = NULL;
    while (root)
    {
        ast *child = root->firstChild;
        while (child)
        {
            if (child->nodeType == RECORD_OR_UNION && child->firstChild->nodeType != ID)
            {
                //printf("Child node is %s and type is %d\n", child->lex, child->nodeType);
                curr_ast = child->firstChild;
                //printf("current node's lexeme: %s\n", curr_ast->lex);
                identifierNode* new = (identifierNode*)malloc(sizeof(identifierNode));
                new->token = (tokenInfo*)malloc(sizeof(tokenInfo));
                new->token->lexeme = child->lex;
                identifierNode* check = retrieveFake(firstPass, new, true, false);
                if(check)
                {
                    printf("Redeclaration of Record on line %d\n", curr_ast->line);
                }

                else
                {
                    // printf("lexeme being inserted: %s\n", new->token->lexeme);
                    insertFake(firstPass, new, true);
                }
            }
            child = child->nextSibling;
        }
        root = root->nextSibling;
    }
}

void createAliasTable(ast* root){
    root = root->firstChild;
    ast *curr_ast = NULL;
    while (root)
    {
        ast *child = root->firstChild;
        while (child)
        {
            if (child->nodeType == DEFINETYPE)
            {
                //printf("In definetype, Child node is %s and type is %d\n", child->lex, child->nodeType);
                identifierNode *ru = (identifierNode *)malloc(sizeof(identifierNode));
                ru->token = (tokenInfo *)malloc(sizeof(tokenInfo));
                ru->token->lexeme = child->firstChild->nextSibling->lex;
                ru->function = (tokenInfo *)malloc(sizeof(tokenInfo));
                ru->function->lexeme = child->firstChild->nextSibling->nextSibling->lex;

                identifierNode *existing = (identifierNode*)retrieveFake(firstPass, ru, true, false); //check if record exists
                if (existing == NULL)
                {
                    printf("Record %s does not exist on line no. %d\n", child->firstChild->nextSibling->lex, child->firstChild->nextSibling->line);
                }
                else
                {   
                    identifierNode* check =(identifierNode*) retrieveFake(aliasTable, ru, false, true); //check if alias exists
                    if(check){
                        printf("Redeclaration of alias on line %d\n", child->firstChild->nextSibling->line);
                    }
                    else{
                        identifierNode* check1 = (identifierNode*)retrieveFake(firstPass, ru, false, false);//check if alias is record name
                        if(check1){
                            printf("Redeclaration of record name as alias on line %d\n", child->firstChild->nextSibling->line);
                        }
                        else{
                            insertFake(aliasTable, ru, false);
                        }
                    }
                }
            }
            child = child->nextSibling;
        }
        root = root->nextSibling;
    }
}

void createFTable(ast *root)
{
    if(root==NULL)
        return;
    root = root->firstChild;
    while (root!=NULL)
    {
        int offset = 0;
        functionNode *func = createFNode(root);
        ast *child = root->firstChild;
        parameters *curr_ip = NULL;
        parameters *curr_op = NULL;
        ast *pars = NULL;
        int width = 0;
        while (child)
        {
            if (child->nodeType == INPUT_PARAMETERS)
            {
                pars = child->firstChild;
                while (pars!=NULL)
                {
                    identifierNode *id;
                    switch (pars->symbol)
                    {
                    case TK_INT:
                    {
                        id = createINode(pars->firstChild, child->parent, INT_TYPE, false, &offset); // TODO
                        break;
                    }
                    case TK_REAL:
                    {
                        id = createINode(pars->firstChild, child->parent, REAL_TYPE, false, &offset); // TODO
                        break;
                    }
                    case TK_RUID:
                    {
                        if (pars->is_union)
                        {
                            id = createINode(pars->firstChild, child->parent, UNION_TYPE, false, &offset); // TODO
                            int a = GodHelpMe(pars->lex,pars->firstChild->lex,false,child->parent);

                        }
                        else
                        {
                            id = createINode(pars->firstChild, child->parent, RECORD_TYPE, false, &offset); // TODO
                            int a = GodHelpMe(pars->lex,pars->firstChild->lex,false,child->parent);
                        }
                        break;
                    }
                    }
                    identifierNode *check = (identifierNode *)retrieve(SymbolTable, id, ID);
                    if (check)
                    {
                        printf("Redeclaration of %s in input parameter list on line no. %d\n",child->firstChild->lex,child->firstChild->line);
                    }
                    else
                    {
                        insert(SymbolTable, id, ID);
                    }
                    if (!curr_ip)
                    {
                        func->ipParams = createIPParams(pars->firstChild, pars->nodeType); // TODO
                        curr_ip = func->ipParams;
                    }
                    else
                    {
                        curr_ip->next = createIPParams(pars->firstChild, pars->nodeType);
                        curr_ip = curr_ip->next;
                    }
                    width += id->width;
                    pars = pars->nextSibling;
                }
            }
            else if (child->nodeType == OUTPUT_PARAMETERS)
            {
                pars = child->firstChild;
                while (pars)
                {
                    identifierNode *id;
                    switch (pars->symbol)
                    {
                    case TK_INT:
                    {
                        id = createINode(pars->firstChild, child->parent, INT_TYPE, false, &offset); // TODO
                        break;
                    }
                    case TK_REAL:
                    {
                        id = createINode(pars->firstChild, child->parent, REAL_TYPE, false, &offset); // TODO
                        break;
                    }
                    case TK_RUID:
                    {
                        if (pars->is_union)
                        {
                            id = createINode(pars->firstChild, child->parent, UNION_TYPE, false, &offset); // TODO
                            int a = GodHelpMe(pars->lex,pars->firstChild->lex,false,child->parent);
                        }
                        else
                        {
                            id = createINode(pars->firstChild, child->parent, RECORD_TYPE, false, &offset); // TODO
                            int a = GodHelpMe(pars->lex,pars->firstChild->lex,false,child->parent);
                        }
                        break;
                    }
                    }
                    identifierNode *check = (identifierNode *)retrieve(SymbolTable, id, ID);
                    if (check)
                    {
                        printf("redeclaration");
                    }
                    else
                    {
                        insert(SymbolTable, id, ID);
                    }
                    if (!curr_op)
                    {
                        func->opParams = createIPParams(pars->firstChild, pars->nodeType); // TODO
                        curr_ip = func->opParams;
                    }
                    else
                    {
                        curr_op->next = createIPParams(pars->firstChild, pars->nodeType);
                        curr_op = curr_op->next;
                    }
                    width += id->width;
                    pars = pars->nextSibling;
                }
            }
            child = child->nextSibling;
        }
        func->width = width;
        functionNode *check = (functionNode *)retrieve(SymbolTable, func, FUNCTION_SEQ);
        if (check)
        {
            printf("redecl of function name");
        }
        else
        {
            //func->tmpVars = 0;
            insert(SymbolTable, func, FUNCTION_SEQ);
        }
        root = root->nextSibling;
    }
}

int GodHelpMe(char* recordName, char* dotName, bool global, ast* func){
    recordUnionNode* temp = (recordUnionNode*)malloc(sizeof(recordUnionNode));
    temp->token = (tokenInfo*)malloc(sizeof(tokenInfo));
    temp->token->lexeme = recordName;
    int width = 0;

    recordUnionNode* ru = (recordUnionNode*)retrieve(SymbolTable,temp,RECORD_OR_UNION);
    recordField* head = ru->fieldList;
    while(head){
        // char concatString[100];
        // strcpy(concatString,dotName);
        // strcat(concatString,".");
        // strcat(concatString,head->token->lexeme);
        int x = strlen(dotName);
        int y = strlen(head->token->lexeme);
        int z = x+y+1;
        char* concatString = (char *)malloc(sizeof(char) * z);
        for(int i = 0; i < x; i++)
            concatString[i] = dotName[i];
        concatString[x] = '.';
        for(int j = 0; j < y; j++)
            concatString[j+x+1] = (head->token->lexeme)[j];

        // strcpy(concatString,dotName);
        // strcat(concatString,".");
        // strcat(concatString, head->token->lexeme)

        identifierNode* id = (identifierNode*)malloc(sizeof(identifierNode));
        id->token  = (tokenInfo*)malloc(sizeof(tokenInfo));
        id->global = global;
        id->function = (tokenInfo*)malloc(sizeof(tokenInfo));
        id->function->lexeme = func->lex;

        if(head->type == INT_TYPE || head->type == REAL_TYPE){
            width += head->width;
            id->width = head->width;
        }
        else{
            width += GodHelpMe(head->recordName, concatString, false, func);
            id->width = width;
        }

        id->token->lexeme = concatString;

        identifierNode* check = (identifierNode*)retrieve(SymbolTable,id,ID);
        if(check!=NULL){
            printf("Redeclaration of field ID %s at line no. %d\n",head->token->lexeme, head->token->lineNo);
        }
        insert(SymbolTable,id,ID);
        head = head->next;
    }
    return width;

}

void createITable(ast *root)
{
    int globalOffset = 0;
    root = root->firstChild;    //root points to FUNCTION_SEQ nodes
    while (root)
    {
        ast *child = root->firstChild;  //first child of the function
        functionNode *func = (functionNode *)malloc(sizeof(functionNode));
        func->token = (tokenInfo *)malloc(sizeof(tokenInfo));
        func->token->lexeme = root->lex;
        func->token->lineNo = root->line;
        func->token->tid = root->symbol;
        func = (functionNode *)retrieve(SymbolTable, func, FUNCTION_SEQ);
        int localOffset = func->width;
        while (child)
        {
            
           if (child->nodeType == RECORD_OR_UNION && child->firstChild->nodeType == ID)  //DECLARATION
            {
                identifierNode *id = (identifierNode *)malloc(sizeof(identifierNode));
                if (child->firstChild->nextSibling)
                {
                    // It is a global identifier
                    // printf("%s\n", child->firstChild->nextSibling->lex);
                    if (!child->is_union)
                    {
                        id = createINode(child->firstChild, child->parent, RECORD_TYPE, true, &globalOffset);
                        int a = GodHelpMe(child->lex,child->firstChild->lex,false,child->parent);

                    }
                    else
                    {
                        id = createINode(child->firstChild, child->parent, UNION_TYPE, true, &globalOffset);
                        int a = GodHelpMe(child->lex,child->firstChild->lex,false,child->parent);

                    }
                }
                else
                {
                    // it is local id
                    if (!child->is_union)
                    {
                        id = createINode(child->firstChild, child->parent, RECORD_TYPE, false, &localOffset);
                        int a = GodHelpMe(child->lex,child->firstChild->lex,false,child->parent);
                        // printf("FINAL WIDTH %s %d\n",child->lex,a);
                        
                    }
                    else
                    {
                        id = createINode(child->firstChild, child->parent, UNION_TYPE, false, &localOffset);
                        int a = GodHelpMe(child->lex,child->firstChild->lex,false,child->parent);

                    }
                }
                if(child->parent->nodeType != OUTPUT_PARAMETERS && child->parent->nodeType != INPUT_PARAMETERS){
                    identifierNode *check = (identifierNode *)retrieve(SymbolTable, id, ID);
                    if (check)
                    {
                        if (check->global)
                        {
                            printf("redeclr of global var bad\n");
                            // TODO file stuff
                        }
                        else
                        {
                            printf("redcl 1\n");
                            printf("%d %d %s\n",child->nodeType, child->parent->nodeType, child->lex);
                        }
                    }
                    else
                    {
                        insert(SymbolTable, id, ID);
                        //insertFields()
                    }
                }
            }
           else if (child->nodeType == INTEGER && child->firstChild->nodeType == ID)
            {
               printf("%d", child->parent->nodeType);
                //printf("here 2 %d %d  %d\n",child->nodeType,child->parent->nodeType, child->line);
                identifierNode *id = (identifierNode *)malloc(sizeof(identifierNode));
                
               // identifierNode* id;
                
                if (child->firstChild->nextSibling)
                {
                    id = createINode(child->firstChild, child->parent, INT_TYPE, true, &globalOffset);
                }
                else
                {
                    
                    id = createINode(child->firstChild, child->parent, INT_TYPE, false, &localOffset);
                }
                if(child->parent->nodeType != OUTPUT_PARAMETERS && child->parent->nodeType != INPUT_PARAMETERS){

                    
                    printf("%d %d %d %s %s %d",id->width, id->offset,id->type,id->token->lexeme,id->function->lexeme,id->global);
                    identifierNode *check = (identifierNode *)retrieve(SymbolTable, id, ID);
                    //seg fault here

                    if (check!=NULL)
                    {
                        // printf("huhu\n");
                        if (check->global)
                        {
                            printf("redeclr of global var bad\n");
                            // TODO file stuff
                        }
                        else
                        {
                            
                            printf("redcl 2\n");
                            printf("%d %d %s\n",child->nodeType, child->parent->nodeType, child->lex);
                        }
                    }
                    else{
                        insert(SymbolTable,id,ID);
                    }
                }
            }
            else if(child->nodeType == REAL && child->firstChild->nodeType == ID)
            {
                // printf("3 %d %d %s %s %d\n",child->nodeType,child->parent->nodeType,child->lex, child->parent->lex, child->line);
                identifierNode *id = (identifierNode *)malloc(sizeof(identifierNode));
                if (child->firstChild->nextSibling)
                {
                    id = createINode(child->firstChild, child->parent, REAL, true, &globalOffset);
                }
                else
                {
                    id = createINode(child->firstChild, child->parent, REAL, false, &localOffset);
                }
                if(child->parent->nodeType != OUTPUT_PARAMETERS && child->parent->nodeType != INPUT_PARAMETERS){
                    identifierNode *check = (identifierNode *)retrieve(SymbolTable, id, ID);
                    if (check)
                    {
                        if (check->global)
                        {
                            printf("redeclr of global var bad\n");
                            // TODO file stuff
                        }
                        else
                        {
                            printf("redcl 3\n");
                            printf("%d %d %s\n",child->nodeType, child->parent->nodeType, child->lex);
                        }
                    }
                    else{
                        insert(SymbolTable,id,ID);
                    }
                }
            }
            child = child->nextSibling;
        }
        func->width = localOffset;
        root = root->nextSibling;
    }
    GLOBAL_WIDTH = globalOffset;
}

subTable *initSubTable()
{
    subTable *st = (subTable *)malloc(sizeof(subTable));
    st->table = (Entry *)malloc(TABLE_SLOTS * sizeof(Entry));
    for (int i = 0; i < TABLE_SLOTS; i++)
    {
        st->table[i].node = NULL;
        st->table[i].next = NULL;
    }
    return st;
}

void initializeSymbolTable(ast *ast)
{
    SymbolTable = (symbol_Table *)malloc(sizeof(symbolTable));
    firstPass = initSubTable();
    aliasTable = initSubTable();
    SymbolTable->IdentifierTable = initSubTable();
    SymbolTable->FunctionTable = initSubTable();
    SymbolTable->RecordUnionTable = initSubTable();
    createFirstPass(ast);
    printFPTable(firstPass);
    createAliasTable(ast);
    printAliasTable(aliasTable);
    createRUtable(ast);
    printf("record table done\n");
    createFTable(ast);
    printf("function table done\n");
    createITable(ast);
    printf("identifier table done\n");
}

void printRecordTable(subTable *rec_table)
{
    int i;
    Entry *entry;
    recordUnionNode *record;
    printf("#%-30s %-30s %-30s\n", "Lexeme", "Type", "Width");
    for (i = 0; i < TABLE_SLOTS; i++)
    {
        entry = &(rec_table->table[i]);
        while (entry != NULL)
        {
            record = (recordUnionNode *)(entry->node);
            if (record != NULL)
            {
                printf("%-30s", record->token->lexeme);
                recordField *fields = record->fieldList;
                char fieldsstring[100] = "";
                while (fields != NULL)
                {
                    if (fields->width / 2 == 2)
                    {
                        strcat(fieldsstring, "int,");
                    }
                    else
                    {
                        strcat(fieldsstring, "real,");
                    }
                    fields = fields->next;
                }
                printf("%-30s", fieldsstring);
                printf("%d\n", record->width);
                printf("-------------------------------\n");
            }
            entry = entry->next;
        }
    }
}

void printFunctionTable(subTable *fun_table)
{
    int i;
    Entry *entry;
    functionNode *fun_node;
    printf("#%-30s %-30s\n", "Lexeme", "Width");
    for (i = 0; i < TABLE_SLOTS; i++)
    {
        entry = &(fun_table->table[i]);
        while (entry != NULL)
        {
            fun_node = (functionNode *)(entry->node);
            if (fun_node != NULL)
            {
                printf("%-30s %d\n", fun_node->token->lexeme, fun_node->width);
                printf("-------------------------------\n");
            }
            entry = entry->next;
        }
    }
}

void printSymbolTable(symbol_Table* st){
    subTable* t = st->IdentifierTable;
    int i;
    Entry * entry;
    identifierNode *node;
    printf("%-20s %-15s %-10s %-15s %-10s %-10s %-10s %-10s\n", "Variable Name", "Scope", "Type", "Type Expression", "Width", "isGlobal", "Offset", "VariableUsage");
    for (i = 0; i < TABLE_SLOTS; i++)
    {
        entry = &(t->table[i]);
        while (entry != NULL)
        {
            node = (identifierNode *)(entry->node);
            if (node != NULL)
            {
                printf("%-30s %d %s \n", node->token->lexeme, node->width, node->function->lexeme);
                printf("-------------------------------\n");
            }
            entry = entry->next;
        }
    }
}

void printIDTable(subTable *fun_table)
{
    int i;
    Entry * entry;
    identifierNode *fun_node;
    printf("#%-30s %-30s\n", "Lexeme", "Width");
    for (i = 0; i < TABLE_SLOTS; i++)
    {
        entry = &(fun_table->table[i]);
        while (entry != NULL)
        {
            fun_node = (identifierNode *)(entry->node);
            if (fun_node != NULL)
            {
                printf("%-30s %d\n", fun_node->token->lexeme, fun_node->width);
                printf("-------------------------------\n");
            }
            entry = entry->next;
        }
    }
}

void printFPTable(subTable *fun_table)
{
    int i;
    Entry * entry;
    identifierNode *fun_node;
    printf("#%-30s\n", "Lexeme");
    for (i = 0; i < TABLE_SLOTS; i++)
    {
        entry = &(fun_table->table[i]);
        while (entry != NULL)
        {
            fun_node = (identifierNode *)(entry->node);
            if (fun_node != NULL)
            {
                printf("%-30s \n", fun_node->token->lexeme);
                printf("-------------------------------\n");
            }
            entry = entry->next;
        }
    }
}

void printAliasTable(subTable *fun_table)
{
    int i;
    Entry * entry;
    identifierNode *fun_node;
    printf("#%-30s\n", "Lexeme");
    for (i = 0; i < TABLE_SLOTS; i++)
    {
        entry = &(fun_table->table[i]);
        while (entry != NULL)
        {
            fun_node = (identifierNode *)(entry->node);
            if (fun_node != NULL)
            {
                printf("%-30s \n", fun_node->function->lexeme);
                printf("-------------------------------\n");
            }
            entry = entry->next;
        }
    }
}

