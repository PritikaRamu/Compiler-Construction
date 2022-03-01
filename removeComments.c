#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>

//strip white space from beginning of string
char *strip(char *str)
{
    while(*str==' ' || *str=='\t') str++;
    return str;
}

void removeComments(char* testcaseFile, char* cleanFile){
    //printf("%s\n",testcaseFile);
    FILE *inp = fopen(testcaseFile,"r");
    FILE *out = fopen(cleanFile,"w");

    if(!inp){
        printf("Error opening testcaseFile\n");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    size_t sizeRead;

    // sizeRead = getline(&line, &len, inp);

    // while(sizeRead != -1) {
    //     char *mod = strip(line);
    //     if(strlen(mod)>0){
    //         if(mod[0]=='%'){
    //             sizeRead = getline(&line, &len, inp);
    //             continue;
    //         }
    //     }
    //     fprintf(out,"%s",line);
    //     sizeRead = getline(&line, &len, inp);
    // }

    while(1){
        sizeRead = getline(&line, &len, inp);
        if(sizeRead==-1)
            break;
        char *mod = strip(line);
        if(strlen(mod)>0){
            if(mod[0]!='%'){
                fprintf(out,"%s",line);
            }
        }
    }
}

int main()
{
    removeComments("t3.txt","cleanFile.txt");
    return 0;
}