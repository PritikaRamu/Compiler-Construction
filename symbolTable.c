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
    
}