/*
Henrique Gomes Zanin NUSP: 10441321
Gabriel Guimaraes Vilas Boas Marin NUSP: 11218521
*/
#ifndef __UTILSTEST__
#define __UTILSTEST__
#define EOL "\n\r"
#define BOOL unsigned char
#define TRUE 1
#define FALSE 0

#include <stdio.h>

typedef struct textdocument{
    char **text;
    int nLines;
}TextDocument;

char *readLine(FILE *);
void assertFile(FILE *, char*);
FILE *openOrCreateFile(char *);
FILE *openFile(char *);
TextDocument *readAll(FILE *);
void printTextDocument(TextDocument *);
char *createStringOnHeap(char *string);

#endif