/*
Henrique Gomes Zanin NUSP: 10441321
Gabriel Guimaraes Vilas Boas Marin NUSP: 11218521
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <utils.h>

BOOL checkEndOfLine(char character){
    if(strchr(EOL, character) || character == EOF) return TRUE;
    else return FALSE;
}

char *readLine(FILE *fp){
    char *textLine = NULL;
    unsigned int size = 0;
    do{
        textLine = (char*)realloc(textLine, (size+1)*sizeof(char));
        textLine[size] = fgetc(fp);
    }while(!checkEndOfLine(textLine[size++]));
    textLine[size-1] = '\0';
    return textLine;
}

void assertFile(FILE *fp, char *fileName){
    if(!fp){
        printf("Fail to open file %s, exiting program!\n", fileName);
        exit(0);
    }
}

FILE *openOrCreateFile(char *fileName){
    FILE *fp;
    fp = fopen(fileName, "r+");
    if(!fp) fp = fopen(fileName, "w+");
    assertFile(fp, fileName);
  return fp;
}

FILE *openFile(char *fileName){
    FILE *fp = fopen(fileName, "r");
    assertFile(fp, fileName);
    return fp;
}

TextDocument *readAll(FILE *fp){
    TextDocument *doc = (TextDocument*)calloc(1,sizeof(TextDocument));
    char *aux;
    while (!feof(fp)){
        aux = readLine(fp);
        if(strlen(aux)){
            doc->nLines++;
            doc->text = (char**)realloc(doc->text,doc->nLines*sizeof(char*));
            doc->text[doc->nLines-1] = aux;
        }
        else{
            free(aux);
        }
    }
    return doc;
}

void printTextDocument(TextDocument *doc){
    int i;
    for(i = 0; i < doc->nLines; i++){
        printf("%s\n", doc->text[i]);
    }
}

char *createStringOnHeap(char *string){
    char *new = (char*)calloc(strlen(string),sizeof(char));
    strcpy(new,string);
    return new;
}