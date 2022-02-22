#include<stdio.h>
#include<string.h>

//helper functions

//strip white space from beginning of string
char *strip(char *str)
{
    while(*str==' ') str++;
    return str;
}

void removeComments(char* testcaseFile, char* cleanFile){
    printf("%s\n",testcaseFile);
    FILE *inp = fopen(testcaseFile,"r");
    FILE *out = fopen(cleanFile,"w");

    if(!inp){
        printf("Error opening testcaseFile\n");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t sizeRead;

    sizeRead = getline(&line, &len, inp);

    while(sizeRead != -1) {
        char *mod = strip(line);
        if(strlen(mod)>0){
            if(mod[0]=='%'){
                sizeRead = getline(&line, &len, inp);
                continue;
            }
        }
        fprintf(out,"%s",line);
        sizeRead = getline(&line, &len, inp);
    }
}

void main(){
    removeComments("testComments.txt","output.txt");
}