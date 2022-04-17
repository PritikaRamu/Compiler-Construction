/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/
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

int max(int num1, int num2)
{
    return (num1 > num2) ? num1 : num2;
}

bool isValidVariant(recordField *fields)
{
    bool isVariant = false;

    while (fields != NULL)
    {
        if (fields->type == UNION_TYPE)
        {
            if (isVariant)
            {
                char string[100];
                sprintf(string, "Line %d: ERROR- Variant record cannot have more than one union\n", fields->token->lineNo);
                addErrorToArray(string);
                // printf();
                return false;
            }
            else
            {
                isVariant = true;
            }
        }
        fields = fields->next;
    }
    return isVariant;
}

bool isVariant(recordField *fields)
{
    bool variantCheck = false;

    while (fields != NULL)
    {
        if (fields->type == INT_TYPE)
        {
            if (strcmp(fields->token->lexeme, "tagvalue") == 0)
            {
                variantCheck = true;
            }
        }
        else if (fields->type == REAL_TYPE)
        {
            if (strcmp(fields->token->lexeme, "tagvalue") == 0)
            {
                variantCheck = true;
            }
        }
        fields = fields->next;
    }
    return variantCheck;
}

recordField *createFieldList(ast *curr_ast)
{

    ast *iterator = curr_ast;
    recordField *head = NULL;
    recordField *tail = NULL;

    while (iterator != NULL)
    {
        recordField *fields = (recordField *)malloc(sizeof(recordField));
        // fields->isVariant = false;
        if (iterator->nodeType == INTEGER)
        {
            fields->offset = 0;
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
            fields->offset = 0;
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
            identifierNode *temp = (identifierNode *)malloc(sizeof(identifierNode));
            temp->function = (tokenInfo *)malloc(sizeof(tokenInfo));
            temp->function->lexeme = iterator->lex;

            identifierNode *x = (identifierNode *)retrieveFake(aliasTable, temp, false, true);
            if (x == NULL)
            {
                char string[100];
                sprintf(string, "Line %d: ERROR Using a record as field that has not been declared\n", iterator->line);
                addErrorToArray(string);
                // printf();
            }
            else
            {
                fields->token = (tokenInfo *)malloc(sizeof(tokenInfo));
                fields->token->tid = iterator->symbol;
                fields->token->lexeme = iterator->firstChild->lex;
                fields->token->lineNo = iterator->firstChild->line;
                fields->type = x->isUnion ? UNION_TYPE : RECORD_TYPE;
                fields->recordName = x->token->lexeme;

                // printf("dont know what to do herererererere %s %s\n",fields->recordName,fields->token->lexeme);
            }

            // identifierNode *x = (identifierNode *)retrieveFake(aliasTable,temp,false,true);
            // identifierNode *y = (identifierNode *)retrieveFake(firstPass,temp,false,false);

            //     if (y!=NULL && y->isUnion)
            //     {
            //         fields->token = (tokenInfo *)malloc(sizeof(tokenInfo));
            //         fields->token->tid = iterator->symbol;
            //         fields->token->lexeme = iterator->firstChild->lex;
            //         fields->token->lineNo = iterator->firstChild->line;
            //         fields->type = UNION_TYPE;
            //         fields->recordName = y->token->lexeme;
            //     }
            //     else if(y==NULL && x!=NULL &&){
            //         fields->token = (tokenInfo *)malloc(sizeof(tokenInfo));
            //         fields->token->tid = iterator->symbol;
            //         fields->token->lexeme = iterator->firstChild->lex;
            //         fields->token->lineNo = iterator->firstChild->line;
            //         fields->type = RECORD_TYPE;
            //         fields->recordName = x->token->lexeme;
            //     }
            //     else if(y!=NULL && !y->isUnion)
            //     {
            //         printf("Use an alias for nested records at line no. %d\n",iterator->line);

            //     // printf("dont know what to do herererererere %s %s\n",fields->recordName,fields->token->lexeme);
            //     }
            //     else{
            //         printf("Record or union not defined at line no. %d\n",iterator->line);
            //     }
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

functionNode *createFNode(ast *root)
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

    return (strcmp(node1, node2) == 0) ? true : false;
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

    return (strcmp(((functionNode *)node1)->token->lexeme, ((functionNode *)node2)->token->lexeme) == 0) ? true : false;
}

// need to check this function
bool inode_check(void *node1, void *node2)
{
    identifierNode *temp1 = (identifierNode *)node1;
    identifierNode *temp2 = (identifierNode *)node2;
    //printf("%d %d %s %s\n",temp1->global,temp2->global, temp1->token->lexeme,temp2->token->lexeme);
    if (temp2->global && temp1->global)
    {
        // both are global and lexemes are same
        if (strcmp(temp2->token->lexeme, temp1->token->lexeme) == 0)
        {
            return true; // node already exists in symbol table
        }
    }
    else if ((!(temp1->global) && temp2->global) || (!(temp2->global) && temp1->global))
    {
        // one is global and the other is local
        if (strcmp(temp1->token->lexeme, temp2->token->lexeme) == 0)
        {
            return true;
        }
    }
    else if (strcmp(temp1->function->lexeme, temp2->function->lexeme) == 0)
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
    if (node == NULL)
        return NULL;
    int key;
    subTable *t;
    // return NULL;
    switch (type)
    {
    case ID:
    {
        key = hash(((identifierNode *)node)->token->lexeme);
        // printf("Lexeme for retrieve is %s\n",((identifierNode *)node)->token->lexeme);
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
        // printf("Lexeme for retrieve is %s\n",((functionNode *)node)->token->lexeme);
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
    void *check = t->table[key].node;
    if (!check)
    {
        // printf("inserting NULL %s\n",((identifierNode *)node)->token->lexeme);
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
        Entry *temp = (Entry *)malloc(sizeof(Entry));
        temp->key = key;
        temp->node = node;
        temp->next = NULL;
        entry->next = temp;
        // printf("inserting NOT NULL%s\n",((identifierNode *)node)->token->lexeme);
    }
}

identifierNode *createINode(ast *id, ast *func, Type type, bool is_global, int *offset)
{
    identifierNode *iden = (identifierNode *)malloc(sizeof(identifierNode));
    iden->function = (tokenInfo *)malloc(sizeof(tokenInfo));
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
    iden->isIP = false;
    iden->isOP = false;
    iden->recordList = NULL;
    iden->recordName = id->parent->lex;
    if (iden->type == RECORD_TYPE || iden->type == UNION_TYPE)
    {

        recordUnionNode *temp = (recordUnionNode *)malloc(sizeof(recordUnionNode));
        temp->token = (tokenInfo *)malloc(sizeof(tokenInfo));
        temp->token->lexeme = id->parent->lex;

        identifierNode *new1 = (identifierNode *)malloc(sizeof(identifierNode));
        new1->token = (tokenInfo *)malloc(sizeof(tokenInfo));
        new1->token->lexeme = id->parent->lex;

        identifierNode *checkAlias = (identifierNode *)retrieveFake(aliasTable, new1, true, true);

        if (checkAlias != NULL)
        {
            // printf("Preetika %s %s\n",temp->token->lexeme, checkAlias->token->lexeme);
            temp->token->lexeme = checkAlias->token->lexeme;
            iden->recordName = checkAlias->token->lexeme;
            // printf("Pritika %s %s\n",temp->token->lexeme, checkAlias->token->lexeme);
        }
        iden->recordList = (recordUnionNode *)retrieve(SymbolTable, temp, RECORD_OR_UNION);
        if (iden->recordList == NULL)
        {

            char string[100];
            sprintf(string, "Line %d: ERROR- Record or Union is not defined\n", iden->token->lineNo);
            addErrorToArray(string);
            // printf();
            return NULL;
        }
        if (iden->type == RECORD_TYPE)
        {
            iden->width = GodHelpMe(id->parent->lex, id->lex, false, func);
        }
        else
        {
            iden->width = GodHelpMeForUnion(id->parent->lex, id->lex, false, func);
        }
    }
    else
    {
        if (iden->type == INT_TYPE)
        {
            iden->width = INT_WIDTH;
        }
        else
        {
            iden->width = REAL_WIDTH;
        }
    }
    iden->global = is_global;
    (*offset) += iden->width;
    return iden;
}

// PASS 3 : record field lexemes are stored
void createRUtable(ast *root)
{
    // taking our third pass through the record table
    root = root->firstChild; // first node in FUNCTION_SEQ
    recordField *curr_field = NULL;
    recordField *fields = NULL;
    ast *curr_ast = NULL;
    while (root)
    {
        // root always points to one of the functions
        ast *child = root->firstChild;
        // child is the first child of the function pointed to by root
        while (child)
        {
            if (child->nodeType == RECORD_OR_UNION && child->firstChild->nodeType != ID)
            {
                curr_ast = child->firstChild;
                fields = createFieldList(curr_ast); // iterates and returns a linked list of field nodes
                recordUnionNode *new = createRUNode(child, fields);
                bool b1 = isVariant(fields);
                bool b2 = isValidVariant(fields);
                new->isVariant = b1 &&b2;
                new->recordName = child->lex;

                // check if the name exists in the first pass table
                // recordUnionNode* check = retrieve(SymbolTable, new, RECORD_OR_UNION);
                identifierNode *new1 = (identifierNode *)malloc(sizeof(identifierNode));
                new1->token = (tokenInfo *)malloc(sizeof(tokenInfo));
                new1->token->lexeme = curr_ast->lex;
                identifierNode *check = retrieveFake(firstPass, new1, true, false);
                if (!check)
                {
                    if (b2 || !b1)
                    {
                        insert(SymbolTable, new, RECORD_OR_UNION);
                    }
                }
            }
            child = child->nextSibling;
        }
        root = root->nextSibling;
    }
}

identifierNode *retrieveFake(subTable *st, identifierNode *id, bool token, bool alias)
{

    if (!alias)
    {
        if (token)
        {
            int key = hash(id->token->lexeme);
            if (st->table[key].node)
            {
                Entry *entry = &(st->table[key]);
                while (entry)
                {
                    if (first_check(((identifierNode *)(entry->node))->token->lexeme, id->token->lexeme))
                    {
                        return entry->node;
                    }
                    entry = entry->next;
                }
            }
        }
        else
        {
            int key = hash(id->function->lexeme);
            if (st->table[key].node)
            {
                Entry *entry = &(st->table[key]);
                while (entry)
                {
                    if (first_check(((identifierNode *)(entry->node))->token->lexeme, id->function->lexeme))
                    {
                        return entry->node;
                    }
                    entry = entry->next;
                }
            }
            return NULL;
        }
    }
    else
    {
        if (token)
        {
            int key = hash(id->token->lexeme);
            if (st->table[key].node)
            {
                Entry *entry = &(st->table[key]);
                while (entry)
                {
                    if (first_check(((identifierNode *)(entry->node))->function->lexeme, id->token->lexeme))
                    {
                        return entry->node;
                    }
                    entry = entry->next;
                }
            }
        }
        else
        {
            int key = hash(id->function->lexeme);
            if (st->table[key].node)
            {
                Entry *entry = &(st->table[key]);
                while (entry)
                {
                    if (first_check(((identifierNode *)(entry->node))->function->lexeme, id->function->lexeme))
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

void insertFake(subTable *st, identifierNode *id, bool token)
{
    int key;
    if (token)
    {
        key = hash(id->token->lexeme);
    }
    else
    {
        key = hash(id->function->lexeme);
    }

    void *check = st->table[key].node;
    if (check == NULL)
    {
        st->table[key].next = NULL;
        st->table[key].key = key;
        st->table[key].node = id;
    }
    else
    {
        Entry *entry = &(st->table[key]);
        while (entry->next != NULL)
        {
            entry = entry->next;
        }

        Entry *temp = (Entry *)malloc(sizeof(Entry));
        temp->key = key;
        temp->node = id;
        temp->next = NULL;
        entry->next = temp;
    }
}

// PASS 1 : collectiong record names by looking for record definitions
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
                // printf("Child node is %s and type is %d\n", child->lex, child->nodeType);
                curr_ast = child->firstChild;
                // printf("current node's lexeme: %s\n", curr_ast->lex);
                identifierNode *new = (identifierNode *)malloc(sizeof(identifierNode));
                new->token = (tokenInfo *)malloc(sizeof(tokenInfo));
                new->token->lexeme = child->lex;
                new->isUnion = child->is_union;
                identifierNode *check = retrieveFake(firstPass, new, true, false);
                if (check)
                {
                    char string[100];
                    sprintf(string, "Line %d: ERROR -Redeclaration of Record\n", curr_ast->line);
                    addErrorToArray(string);
                    // printf();
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

void printAliasList(identifierNode *existing)
{
    identifierNode *head = existing->aliasList;
    while (head != NULL)
    {
        printf(" %s ", head->function->lexeme);
        head = head->aliasList;
    }

    printf("\n");
}

void addAlias(identifierNode *existing, identifierNode *alias)
{
    // have to add alias to existing ka alias linked list
    // printf("\nAdding %s to alias list of %s\n", alias->function->lexeme, existing->token->lexeme);
    identifierNode *head = existing->aliasList;

    // getting the linked list
    if (head == NULL)
    {
        head = alias;
        existing->aliasList = head;
    }
    else
    {
        alias->aliasList = head;
        existing->aliasList = alias;
    }

    // head = existing;
    // head = existing->aliasList;
    // while(head!=NULL)
    //     {printf(" %s ", head->function->lexeme); head = head->aliasList;}
    // printAliasList(existing);
    // printf("\n");
}

void printReverseMapping(subTable *fun_table)
{
    int i;
    Entry *entry;
    identifierNode *fun_node;
    printf("#%-30s %s\n", "Lexeme", "Alias Name");
    for (i = 0; i < TABLE_SLOTS; i++)
    {
        entry = &(fun_table->table[i]);
        while (entry != NULL)
        {
            fun_node = (identifierNode *)(entry->node);
            if (fun_node != NULL)
            {
                printf("%-30s ", fun_node->token->lexeme);
                // printf("-------------------------------\n");

                printAliasList(fun_node);
            }
            entry = entry->next;
        }
    }
}

// PASS 2 : Mapping aliases to actual record/Union names and reverse mapping
void createAliasTable(ast *root)
{
    root = root->firstChild;
    ast *curr_ast = NULL;
    while (root)
    {
        ast *child = root->firstChild;
        while (child)
        {
            if (child->nodeType == DEFINETYPE)
            {
                // printf("In definetype, Child node is %s and type is %d\n", child->lex, child->nodeType);
                identifierNode *ru = (identifierNode *)malloc(sizeof(identifierNode));
                ru->token = (tokenInfo *)malloc(sizeof(tokenInfo));
                ru->token->lexeme = child->firstChild->nextSibling->lex; // actual name
                ru->function = (tokenInfo *)malloc(sizeof(tokenInfo));
                ru->function->lexeme = child->firstChild->nextSibling->nextSibling->lex; // alias name
                ru->isUnion = child->firstChild->is_union;

                identifierNode *existing = (identifierNode *)retrieveFake(firstPass, ru, true, false); // check if record exists
                if (existing == NULL)
                {
                    char string[100];
                    sprintf(string, "Line %d: ERROR - Record %s does not exist\n", child->firstChild->nextSibling->line, child->firstChild->nextSibling->lex);
                    addErrorToArray(string);
                    // printf();
                }
                else
                {
                    identifierNode *check = (identifierNode *)retrieveFake(aliasTable, ru, false, true); // check if alias exists
                    if (check)
                    {
                        char string[100];
                        sprintf(string, "Line %d: ERROR- Redeclaration of alias\n", child->firstChild->nextSibling->line);
                        addErrorToArray(string);
                        // printf();
                    }
                    else
                    {
                        identifierNode *check1 = (identifierNode *)retrieveFake(firstPass, ru, false, false); // check if alias is record name
                        if (check1)
                        {
                            char string[100];
                            sprintf(string, "Line %d: ERROR - Redeclaration of record name as alias\n", child->firstChild->nextSibling->line);
                            addErrorToArray(string);
                            // printf();
                        }
                        else
                        {
                            addAlias(existing, ru);
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
    if (root == NULL)
        return;
    root = root->firstChild;
    while (root != NULL)
    {
        int offset = 0;
        functionNode *func = createFNode(root);
        ast *child = root->firstChild;
        parameters *curr_ip = (parameters *)malloc(sizeof(parameters));
        parameters *curr_op = (parameters *)malloc(sizeof(parameters));
        curr_ip = NULL;
        curr_op = NULL;
        ast *pars = NULL;
        int width = 0;
        func->numOp = 0;
        while (child)
        {
            if (child->nodeType == INPUT_PARAMETERS)
            {
                pars = child->firstChild;
                while (pars != NULL)
                {
                    identifierNode *id;
                    switch (pars->symbol)
                    {
                    case TK_INT:
                    {
                        id = createINode(pars->firstChild, child->parent, INT_TYPE, false, &offset); // TODO
                        id->isIP = true;
                        break;
                    }
                    case TK_REAL:
                    {
                        id = createINode(pars->firstChild, child->parent, REAL_TYPE, false, &offset); // TODO
                        id->isIP = true;
                        break;
                    }
                    case TK_RUID:
                    {
                        if (pars->is_union)
                        {
                            id = createINode(pars->firstChild, child->parent, UNION_TYPE, false, &offset); // TODO
                            id->isIP = true;
                            // int a = GodHelpMe(pars->lex,pars->firstChild->lex,false,child->parent);
                            // offset += a;
                        }
                        else
                        {
                            id = createINode(pars->firstChild, child->parent, RECORD_TYPE, false, &offset); // TODO
                            id->isIP = true;
                            // int a = GodHelpMe(pars->lex,pars->firstChild->lex,false,child->parent);
                            // offset += a;
                        }
                        break;
                    }
                    }
                    identifierNode *check = (identifierNode *)retrieve(SymbolTable, id, ID);
                    if (check)
                    {

                        char string[100];
                        sprintf(string, "Line %d: ERROR - Redeclaration of %s in input parameter list\n", child->firstChild->line, pars->firstChild->lex);
                        addErrorToArray(string);
                        // printf();
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
                        id->isOP = true;
                        break;
                    }
                    case TK_REAL:
                    {
                        id = createINode(pars->firstChild, child->parent, REAL_TYPE, false, &offset); // TODO
                        id->isOP = true;
                        break;
                    }
                    case TK_RUID:
                    {
                        if (pars->is_union)
                        {
                            id = createINode(pars->firstChild, child->parent, UNION_TYPE, false, &offset); // TODO
                            id->isOP = true;
                        }
                        else
                        {
                            id = createINode(pars->firstChild, child->parent, RECORD_TYPE, false, &offset); // TODO
                            id->isOP = true;
                        }
                        break;
                    }
                    }
                    identifierNode *check = (identifierNode *)retrieve(SymbolTable, id, ID);
                    if (check)
                    {
                        char string[100];
                        sprintf(string, "Line %d: ERROR - Redeclaration of %s in output parameter list\n", child->firstChild->line, pars->firstChild->lex);
                        addErrorToArray(string);
                        // printf();
                    }
                    else
                    {
                        insert(SymbolTable, id, ID);
                    }
                    if (!curr_op)
                    {
                        func->opParams = createIPParams(pars->firstChild, pars->nodeType); // TODO
                        func->numOp++;
                        curr_op = func->opParams;
                    }
                    else
                    {
                        curr_op->next = createIPParams(pars->firstChild, pars->nodeType);
                        func->numOp++;
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
            char string[100];
            sprintf(string, "Line %d: ERROR - Redeclaration of function name %s\n", func->token->lineNo, func->token->lexeme);
            addErrorToArray(string);
            // printf();
        }
        else
        {
            insert(SymbolTable, func, FUNCTION_SEQ);
        }
        root = root->nextSibling;
    }
}

int GodHelpMeForUnion(char *unionName, char *dotName, bool global, ast *func)
{
    recordUnionNode *temp = (recordUnionNode *)malloc(sizeof(recordUnionNode));
    temp->token = (tokenInfo *)malloc(sizeof(tokenInfo));
    temp->token->lexeme = unionName;

    int width = 0;
    // checking for alias
    identifierNode *new1 = (identifierNode *)malloc(sizeof(identifierNode));
    new1->token = (tokenInfo *)malloc(sizeof(tokenInfo));
    new1->token->lexeme = unionName;
    identifierNode *checkAlias = (identifierNode *)retrieveFake(aliasTable, new1, true, true);
    if (checkAlias != NULL)
    {
        temp->token->lexeme = checkAlias->token->lexeme;
    }

    // iteration through fields
    recordUnionNode *ru = (recordUnionNode *)retrieve(SymbolTable, temp, RECORD_OR_UNION);
    recordField *head = ru->fieldList;
    while (head)
    {
        // printf("        UNION WIDTH %d\n",width);
        // concatenation of string for field ids
        int x = strlen(dotName);
        int y = strlen(head->token->lexeme);
        int z = x + y + 1;
        char *concatString = (char *)malloc(sizeof(char) * z);
        for (int i = 0; i < x; i++)
            concatString[i] = dotName[i];
        concatString[x] = '.';
        for (int j = 0; j < y; j++)
            concatString[j + x + 1] = (head->token->lexeme)[j];

        identifierNode *id = (identifierNode *)malloc(sizeof(identifierNode));
        id->token = (tokenInfo *)malloc(sizeof(tokenInfo));
        id->global = global;
        id->function = (tokenInfo *)malloc(sizeof(tokenInfo));
        id->function->lexeme = func->lex;
        id->isRecordField = true;
        id->recordList = ru;

        if (head->type == INT_TYPE || head->type == REAL_TYPE)
        {

            if (head->type == INT_TYPE)
            {
                id->type = INT_TYPE;
                width = max(width, INT_WIDTH);
            }
            else
            {
                id->type = REAL_TYPE;
                width = max(width, REAL_WIDTH);
            }
            id->width = head->width;
        }
        else
        {
            id->type = RECORD_TYPE;
            id->recordName = head->recordName;
            int x = GodHelpMe(head->recordName, concatString, false, func);
            id->width = x;
            width = max(width, x);
        }

        id->token->lexeme = concatString;

        // identifierNode *check = (identifierNode *)retrieve(SymbolTable, id, ID);
        // if (check != NULL)
        // {
        //     char string[100];
        //     sprintf(string, "Line %d: ERROR - Redeclaration of field ID %s\n", head->token->lineNo, head->token->lexeme);
        //     addErrorToArray(string);
        //     // printf();
        // }
        // else
        // {
            insert(SymbolTable, id, ID);
        //}
        head = head->next;
    }
    return width;
}

int GodHelpMe(char *recordName, char *dotName, bool global, ast *func)
{

    recordUnionNode *temp = (recordUnionNode *)malloc(sizeof(recordUnionNode));
    temp->token = (tokenInfo *)malloc(sizeof(tokenInfo));
    temp->token->lexeme = recordName;
    int width = 0;
    // checking for alias
    identifierNode *new1 = (identifierNode *)malloc(sizeof(identifierNode));
    new1->token = (tokenInfo *)malloc(sizeof(tokenInfo));
    new1->token->lexeme = recordName;
    identifierNode *checkAlias = (identifierNode *)retrieveFake(aliasTable, new1, true, true);
    if (checkAlias != NULL)
    {
        temp->token->lexeme = checkAlias->token->lexeme;
    }

    // iteration through fields
    recordUnionNode *ru = (recordUnionNode *)retrieve(SymbolTable, temp, RECORD_OR_UNION);
    recordField *head = ru->fieldList;
    while (head)
    {
        // concatenation of string for field ids
        int x = strlen(dotName);
        int y = strlen(head->token->lexeme);
        int z = x + y + 1;
        char *concatString = (char *)malloc(sizeof(char) * z);
        for (int i = 0; i < x; i++)
            concatString[i] = dotName[i];
        concatString[x] = '.';
        for (int j = 0; j < y; j++)
            concatString[j + x + 1] = (head->token->lexeme)[j];

        identifierNode *id = (identifierNode *)malloc(sizeof(identifierNode));
        id->token = (tokenInfo *)malloc(sizeof(tokenInfo));
        id->global = global;
        id->function = (tokenInfo *)malloc(sizeof(tokenInfo));
        id->function->lexeme = func->lex;
        id->isRecordField = true;
        id->recordList = ru;

        if (head->type == INT_TYPE || head->type == REAL_TYPE)
        {

            if (head->type == INT_TYPE)
            {
                id->type = INT_TYPE;
                width += INT_WIDTH;
            }
            else
            {
                id->type = REAL_TYPE;
                width += REAL_WIDTH;
            }
            id->width = head->width;
        }
        else if (head->type == RECORD_TYPE)
        {
            // printf("        LEXEME %s\n",head->token->lexeme);
            id->type = RECORD_TYPE;
            id->recordName = head->recordName;
            int x = GodHelpMe(head->recordName, concatString, false, func);
            id->width = x;
            width += x;
        }
        else
        {
            id->type = UNION_TYPE;
            id->recordName = head->recordName;
            int x = GodHelpMeForUnion(head->recordName, concatString, false, func);
            id->width = x;
            width += x;
        }

        id->token->lexeme = concatString;

        //identifierNode *check = (identifierNode *)retrieve(SymbolTable, id, ID);
        // if (check != NULL)
        // {
        //     char string[100];
        //     sprintf(string, "Line %d: ERROR - Redeclaration of field ID %s\n", head->token->lineNo, head->token->lexeme);
        //     addErrorToArray(string);
        //     // printf();
        // }
        // else
        // {
            insert(SymbolTable, id, ID);
        //}
        head = head->next;
    }
    return width;
}

char *concatString(char *s1, char *s2)
{
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    int size = l1 + l2 + 1;
    char *s = (char *)malloc(sizeof(char) * size);

    for (int i = 0; i < l1; i++)
    {
        s[i] = s1[i];
    }

    for (int i = 0; i < l2; i++)
    {
        s[l1 + i] = s2[i];
    }

    s[size - 1] = '\0';

    return s;
}

// return type expression
char *GodHelpMeOneMoreTime(char *recordName)
{
    recordUnionNode *temp = (recordUnionNode *)malloc(sizeof(recordUnionNode));
    temp->token = (tokenInfo *)malloc(sizeof(tokenInfo));
    temp->token->lexeme = recordName;
    recordUnionNode *ru = (recordUnionNode *)retrieve(SymbolTable, temp, RECORD_OR_UNION);
    recordField *head = ru->fieldList;
    char *a = (char *)malloc(sizeof(char) * 400);
    char *y;
    if (head)
    {
        if (head->type == INT_TYPE)
        {

            y = (char *)malloc(sizeof(char) * 5);
            y[0] = 'i';
            y[1] = 'n';
            y[2] = 't';
            y[3] = ' ';
        }
        else if (head->type == REAL_TYPE)
        {
            y = (char *)malloc(sizeof(char) * 5);
            y[0] = 'r';
            y[1] = 'e';
            y[2] = 'a';
            y[3] = 'l';
            y[4] = ' ';
        }
        else
        {
            char *z = GodHelpMeOneMoreTime(head->recordName);
            y = (char *)malloc(sizeof(char) * (3 + strlen(z)));
            y[0] = '(';
            y = concatString(y, z);
            int u = strlen(y);
            y[u] = ')';
            y[u + 1] = '\0';
        }
        head = head->next;
        a = concatString(a, y);
    }
    while (head != NULL)
    {
        if (head->type == INT_TYPE)
        {

            y = (char *)malloc(sizeof(char) * 6);
            y[0] = 'x';
            y[1] = ' ';
            y[2] = 'i';
            y[3] = 'n';
            y[4] = 't';
            y[5] = ' ';
        }
        else if (head->type == REAL_TYPE)
        {
            y = (char *)malloc(sizeof(char) * 7);
            y[0] = 'x';
            y[1] = ' ';
            y[2] = 'r';
            y[3] = 'e';
            y[4] = 'a';
            y[5] = 'l';
            y[6] = ' ';
        }
        else
        {
            char *z = GodHelpMeOneMoreTime(head->recordName);
            y = (char *)malloc(sizeof(char) * (3 + strlen(z)));
            y[0] = 'x';
            y[1] = '(';
            y = concatString(y, z);
            int u = strlen(y);
            y[u] = ')';
            y[u + 1] = '\0';
        }
        // printf("Y OUTSIDE: %s\n", y);
        head = head->next;
        a = concatString(a, y);
    }
    return a;
}

// PASS 4 : check for identifier declarations
void createITable(ast *root)
{
    int globalOffset = 0;
    root = root->firstChild; // root points to FUNCTION_SEQ nodes
    while (root)
    {
        ast *child = root->firstChild; // first child of the function
        functionNode *func = (functionNode *)malloc(sizeof(functionNode));
        func->token = (tokenInfo *)malloc(sizeof(tokenInfo));
        func->token->lexeme = root->lex;
        func->token->lineNo = root->line;
        func->token->tid = root->symbol;
        func = (functionNode *)retrieve(SymbolTable, func, FUNCTION_SEQ);
        int localOffset = func->width;
        while (child)
        {
            //printf("    %s\n",child->lex);
            if (child->nodeType == RECORD_OR_UNION && child->firstChild->nodeType == ID) // DECLARATION
            {
                if (child->is_union)
                {
                    char string[100];
                    sprintf(string, "Line %d: ERROR - Variable cannot be of union type\n", child->line);
                    addErrorToArray(string);
                    // printf();
                    child = child->nextSibling;
                    continue;
                }
                identifierNode *id = (identifierNode *)malloc(sizeof(identifierNode));
                if (child->firstChild->nextSibling)
                {
                    // It is a global identifier
                    if (!child->is_union)
                    {
                        id = createINode(child->firstChild, child->parent, RECORD_TYPE, true, &globalOffset);
                    }
                    else
                    {
                        id = createINode(child->firstChild, child->parent, UNION_TYPE, true, &globalOffset);
                    }
                }
                else
                {
                    // it is local id
                    if (!child->is_union)
                    {
                        id = createINode(child->firstChild, child->parent, RECORD_TYPE, false, &localOffset);
                    }
                    else
                    {
                        id = createINode(child->firstChild, child->parent, UNION_TYPE, false, &localOffset);
                    }
                }
                if (child->parent->nodeType != OUTPUT_PARAMETERS && child->parent->nodeType != INPUT_PARAMETERS)
                {
                    identifierNode *check = (identifierNode *)retrieve(SymbolTable, id, ID);
                    if (check)
                    {
                        //printf("%s %d\n",check->token->lexeme,check->global);
                        if (check->global)
                        {
                            char string[100];
                            sprintf(string, "Line %d: ERROR - Redeclaration of global variable %s\n", id->token->lineNo, id->token->lexeme);
                            addErrorToArray(string);
                            // printf();
                        }
                        else
                        {
                            if (id->global)
                            {
                                printf("        IN HERE\n");
                                int x = check->token->lineNo;
                                check->global = true;
                                check->type = id->type;
                                check->offset = globalOffset;
                                check->width = id->width;
                                globalOffset += check->width;
                                check->token = id->token;
                                char string[100];
                                sprintf(string, "Line %d: ERROR Redeclaration of global variable %s\n", x, check->token->lexeme);
                                addErrorToArray(string);
                                // printf();
                            }
                            else
                            {
                                char string[100];
                                sprintf(string, "Line %d: ERROR Redeclaration of variable %s\n", id->token->lineNo, id->token->lexeme);
                                addErrorToArray(string);
                                // printf();
                            }
                        }
                    }
                    else
                    {
                        if (id == NULL)
                        {
                            child = child->nextSibling;
                            continue;
                        }
                        insert(SymbolTable, id, ID);
                    }
                }
            }
            else if (child->nodeType == INTEGER && child->firstChild->nodeType == ID)
            {
                identifierNode *id = (identifierNode *)malloc(sizeof(identifierNode));

                if (child->firstChild->nextSibling)
                {
                    id = createINode(child->firstChild, child->parent, INT_TYPE, true, &globalOffset);
                }
                else
                {

                    id = createINode(child->firstChild, child->parent, INT_TYPE, false, &localOffset);
                }
                if (child->parent->nodeType != OUTPUT_PARAMETERS && child->parent->nodeType != INPUT_PARAMETERS)
                {

                    identifierNode *check = (identifierNode *)retrieve(SymbolTable, id, ID);

                    if (check != NULL)
                    {

                                                //printf("%s %d\n",check->token->lexeme,check->global);

                        if (check->global)
                        {
                            char string[100];
                            sprintf(string, "Line %d: ERROR- Redeclaration of global variable %s\n", id->token->lineNo, id->token->lexeme);
                            addErrorToArray(string);
                            // printf();
                        }
                        // else
                        // {
                        //     char string[100];
                        //     sprintf(string, "Line %d: ERROR - Redeclaration of variable %s\n", id->token->lineNo, id->token->lexeme);
                        //     addErrorToArray(string);
                        //     // printf();
                        // }
                        else
                        {
                            if (id->global)
                            {   
                                int x = check->token->lineNo;
                                check->global = true;
                                check->type = id->type;
                                check->offset = globalOffset;
                                check->width = id->width;
                                globalOffset += check->width;
                                check->token = id->token;
                                char string[100];
                                sprintf(string, "Line %d: ERROR Redeclaration of global variable %s\n", x, check->token->lexeme);
                                addErrorToArray(string);
                                // printf();
                            }
                            else
                            {
                                char string[100];
                                sprintf(string, "Line %d: ERROR Redeclaration of variable %s\n", id->token->lineNo, id->token->lexeme);
                                addErrorToArray(string);
                                // printf();
                            }
                        }
                    }
                    else
                    {
                        insert(SymbolTable, id, ID);
                    }
                }
            }
            else if (child->nodeType == REAL && child->firstChild->nodeType == ID)
            {
                identifierNode *id = (identifierNode *)malloc(sizeof(identifierNode));
                if (child->firstChild->nextSibling)
                {
                    id = createINode(child->firstChild, child->parent, REAL_TYPE, true, &globalOffset);
                }
                else
                {
                    id = createINode(child->firstChild, child->parent, REAL_TYPE, false, &localOffset);
                }
                if (child->parent->nodeType != OUTPUT_PARAMETERS && child->parent->nodeType != INPUT_PARAMETERS)
                {
                    identifierNode *check = (identifierNode *)retrieve(SymbolTable, id, ID);
                    if (check)
                    {
                                                //printf("%s %d\n",check->token->lexeme,check->global);

                        if (check->global)
                        {
                            char string[100];
                            sprintf(string, "Line %d: ERROR - Redeclaration of global variable %s\n", id->token->lineNo, id->token->lexeme);
                            addErrorToArray(string);
                            // printf();
                        }
                        // else
                        // {
                        //     char string[100];
                        //     sprintf(string, "Line %d: ERROR - Redeclaration of variable %s\n", id->token->lineNo, id->token->lexeme);
                        //     addErrorToArray(string);
                        //     // printf();
                        // }
                        else
                        {
                            if (id->global)
                            {
                                int x = check->token->lineNo;
                                check->global = true;
                                check->type = id->type;
                                check->offset = globalOffset;
                                check->width = id->width;
                                globalOffset += check->width;
                                check->token = id->token;
                                char string[100];
                                sprintf(string, "Line %d: ERROR Redeclaration of global variable %s\n", x, check->token->lexeme);
                                addErrorToArray(string);
                                // printf();
                            }
                            else
                            {
                                char string[100];
                                sprintf(string, "Line %d: ERROR Redeclaration of variable %s\n", id->token->lineNo, id->token->lexeme);
                                addErrorToArray(string);
                                // printf();
                            }
                        }
                    }
                    else
                    {
                        insert(SymbolTable, id, ID);
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
    // printFPTable(firstPass);
    createAliasTable(ast);
    // printAliasTable(aliasTable);
    createRUtable(ast);
    // printf("record table done\n");
    createFTable(ast);
    // printf("function table done\n");
    createITable(ast);
    // printf("identifier table done\n");
    // printf("Printing alias table\n");

    // printf("\nAlias table printed\n");
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

void printSymbolTable(symbol_Table *st)
{
    subTable *t = st->IdentifierTable;
    // int i;
    // Entry * entry;
    // identifierNode *node;
    // printf("%-20s %-15s %-10s %-15s %-10s %-10s %-10s %-10s\n", "Variable Name", "Scope", "Type", "Type Expression", "Width", "isGlobal", "Offset", "VariableUsage");
    // for (i = 0; i < TABLE_SLOTS; i++)
    // {
    //     entry = &(t->table[i]);
    //     while (entry != NULL)
    //     {
    //         node = (identifierNode *)(entry->node);

    //         if (node != NULL)
    //         {
    //             if(!(node->isRecordField)){
    //                 if(node->type == RECORD_TYPE || node->type == UNION_TYPE){
    //                     printf("%-30s %d %s %s %s %s   %d\n", node->token->lexeme, node->width, node->function->lexeme, node->recordName, GodHelpMeOneMoreTime(node->recordName), node->global?"true":"false",node->offset);
    //             }
    //             else if(node->type == INT_TYPE){
    //                 printf("%-30s %d %s INT %s   %d\n", node->token->lexeme, node->width, node->function->lexeme,node->global?"true":"false",node->offset);
    //             }
    //             else{
    //                 printf("%-30s %d %s REAL %s  %d\n", node->token->lexeme, node->width, node->function->lexeme, node->global?"true":"false",node->offset);
    //             }
    //             printf("-------------------------------\n");
    //             }
    //         }
    //         entry = entry->next;
    //     }
    // }
    int i;
    Entry *entry;
    identifierNode *fun_node;
    printf("#%-30s %-30s\n", "Lexeme", "Width");
    for (i = 0; i < TABLE_SLOTS; i++)
    {
        entry = &(t->table[i]);
        while (entry != NULL)
        {
            fun_node = (identifierNode *)(entry->node);
            if (fun_node != NULL)
            {
                //     printf("%-30s %d\n", fun_node->token->lexeme, fun_node->width);

                // printf("-------------------------------\n");

                functionNode *temp = (functionNode *)malloc(sizeof(functionNode));
                temp->token = (tokenInfo *)malloc(sizeof(tokenInfo));
                temp->token->lexeme = fun_node->function->lexeme;
                functionNode *temp1 = (functionNode *)retrieve(SymbolTable, temp, FUNCTION_SEQ);
                addID(temp1, fun_node);
                // printIDList(temp1);
            }
            entry = entry->next;
        }
    }
}

void printIDList(functionNode *exist)
{
    identifierNode *head = exist->idList;
    while (head)
    {
        // printf("%-10s %-10s %-10s %-10s\n",head->token->lexeme,head->function->lexeme);

        if (!(head->isRecordField))
        {
            char *varType;
            if (head->isIP)
            {
                varType = "Input Param";
            }
            else if (head->isOP)
            {
                varType = "Output Param";
            }
            else if (head->global)
            {
                varType = "Global";
            }
            else
            {
                varType = "Local";
            }

            if (head->type == RECORD_TYPE || head->type == UNION_TYPE)
            {
                printf("%-10s %-10d %-10s %-10s %-10s %-10s %-10d %-10s  ", head->token->lexeme, head->width, head->function->lexeme, head->recordName, GodHelpMeOneMoreTime(head->recordName), head->global ? "true" : "false", head->offset, varType);
            }
            else if (head->type == INT_TYPE)
            {
                printf("%-10s %-10d %-10s %-10s %-10s %-10s %-10d %-10s  ", head->token->lexeme, head->width, head->function->lexeme, "int", "---", head->global ? "true" : "false", head->offset, varType);
            }
            else
            {
                printf("%-10s %-10d %-10s %-10s %-10s %-10s %-10d %-10s  ", head->token->lexeme, head->width, head->function->lexeme, "real", "---", head->global ? "true" : "false", head->offset, varType);
            }
            printf("\n");
            printf("-------------------------------------------------------------------------------------------\n");
        }
        head = head->idList;
    }
}

void printGlobalList(functionNode *exist)
{
    identifierNode *head = exist->idList;
    while (head)
    {
        // printf("%-10s %-10s %-10s %-10s\n",head->token->lexeme,head->function->lexeme);

        if (!(head->isRecordField))
        {
            if (head->global)
            {
                if (head->type == RECORD_TYPE || head->type == UNION_TYPE)
                {
                    printf("%-30s %d %s %s   %d\n", head->token->lexeme, head->width, head->recordName, GodHelpMeOneMoreTime(head->recordName), head->offset);
                }
                else if (head->type == INT_TYPE)
                {
                    printf("%-30s %d INT   %d\n", head->token->lexeme, head->width, head->offset);
                }
                else
                {
                    printf("%-30s %d REAL  %d\n", head->token->lexeme, head->width, head->offset);
                }
                printf("-------------------------------\n");
            }
        }
        head = head->idList;
    }
}

void addID(functionNode *exist, identifierNode *id)
{
    identifierNode *head = exist->idList;

    if (!head)
    {
        head = id;
        exist->idList = head;
    }
    else
    {
        id->idList = head;
        exist->idList = id;
    }
}

void printIDTable(subTable *fun_table)
{
    int i;
    Entry *entry;
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
                //     printf("%-30s %d\n", fun_node->token->lexeme, fun_node->width);

                // printf("-------------------------------\n");
                functionNode *temp = (functionNode *)malloc(sizeof(functionNode));
                temp->token = (tokenInfo *)malloc(sizeof(tokenInfo));
                temp->token->lexeme = fun_node->function->lexeme;
                functionNode *temp1 = (functionNode *)retrieve(SymbolTable, temp, FUNCTION_SEQ);
                addID(temp1, fun_node);
                // printIDList(temp1);
            }
            entry = entry->next;
        }
    }
}

void printFPTable(subTable *fun_table)
{
    int i;
    Entry *entry;
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
    Entry *entry;
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

void print_Redeclarations()
{
    printf("\nREDECLARATION ERRORS:\n");
    printf("_____________________________________\n");

    for (int i = 0; i < 100; i++)
    {
        if (redeclaration_error_array[i][0] != '\0')
        {
            printf("%s\n", redeclaration_error_array[i]);
        }
    }

    // printf("_____________________________________\n");
    // printf("No. of Redeclarations = %d\n", redeclaration_error_index);
}

void addErrorToArray(char *errorMessage)
{
    strcpy(redeclaration_error_array[redeclaration_error_index], errorMessage);
    redeclaration_error_index++;
}

void populateIDTable(subTable *fun_table)
{
    int i;
    Entry *entry;
    identifierNode *fun_node;
    for (i = 0; i < TABLE_SLOTS; i++)
    {
        entry = &(fun_table->table[i]);
        while (entry != NULL)
        {
            fun_node = (identifierNode *)(entry->node);
            if (fun_node != NULL)
            {
                //     printf("%-30s %d\n", fun_node->token->lexeme, fun_node->width);

                // printf("-------------------------------\n");
                functionNode *temp = (functionNode *)malloc(sizeof(functionNode));
                temp->token = (tokenInfo *)malloc(sizeof(tokenInfo));
                temp->token->lexeme = fun_node->function->lexeme;
                functionNode *temp1 = (functionNode *)retrieve(SymbolTable, temp, FUNCTION_SEQ);
                addID(temp1, fun_node);
                // printIDList(temp1);
            }
            entry = entry->next;
        }
    }
}

void printFinalTable(subTable *fun_table)
{
    populateIDTable(SymbolTable->IdentifierTable);
    int i;
    Entry *entry;
    functionNode *fun_node;
    printf("%-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s \n", "LEXEME", "WIDTH", "SCOPE", "TYPE", "ISGLOBAL", "OFFSET", "VARIABLE_TYPE", "VARIABLE_USAGE");
    for (i = 0; i < TABLE_SLOTS; i++)
    {
        entry = &(fun_table->table[i]);
        while (entry != NULL)
        {
            fun_node = (functionNode *)(entry->node);
            if (fun_node != NULL)
            {
                // printf("%-30s %d\n", fun_node->token->lexeme, fun_node->width);
                // printf("-------------------------------\n");
                printIDList(fun_node);
            }
            entry = entry->next;
        }
    }
    printf("\n \n ALIAS TABLE\n");
    printReverseMapping(firstPass);
}

void printGlobalTable(subTable *fun_table)
{
    populateIDTable(SymbolTable->IdentifierTable);
    int i;
    Entry *entry;
    functionNode *fun_node;
    printf("LEXEME     WIDTH   TYPE  OFFSET\n");
    for (i = 0; i < TABLE_SLOTS; i++)
    {
        entry = &(fun_table->table[i]);
        while (entry != NULL)
        {
            fun_node = (functionNode *)(entry->node);
            if (fun_node != NULL)
            {
                // printf("%-30s %d\n", fun_node->token->lexeme, fun_node->width);
                // printf("-------------------------------\n");
                printGlobalList(fun_node);
            }
            entry = entry->next;
        }
    }
}