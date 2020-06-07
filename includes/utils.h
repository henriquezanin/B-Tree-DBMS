/*
Henrique Gomes Zanin NUSP: 10441321
Gabriel Guimaraes Vilas Boas Marin NUSP: 11218521
*/
#ifndef __UTILS__
#include <stdio.h>
#define __UTILS__

#define BOOL unsigned char
#define TRUE 1
#define FALSE 0
/* End of line */
#define EOL "\n\r"

enum dataType{Int,Float,Double,Char};

typedef union Data{
    int Int;
    float Float;
    double Double;
    char *CharArray;
}Data;

typedef struct index{
    int key;
    long RRN;
}PrimaryIndex;

typedef struct metadataField{
    char description[30];
    enum dataType type;
    unsigned char lenght;
}metadataField;

typedef struct metadata{
    FILE *fpRegister;
    FILE *fpIndex;
    char registerFilename[50];
    metadataField key;
    metadataField *fields;
    int fieldCounter;
    int registerSize;
}Metadata;

typedef enum errors{
    SUCCESS,
    EXIT,
    NULL_METADATA,
    EMPTY_QUERY,
    EMPTY_PAGE,
    UNABLE_TO_WRITE_DATA,
    INVALID_DATA_TYPE,
    INVALID_QUERY,
    INVALID_RRN_PAGE,
    INVALID_FILE_POINTER,
    KEY_NOT_FOUND,
    KEY_ALREADY_EXISTS
}Errors;


typedef struct textDocument{
    int numberOfLines;
    char **text;
}TextDocument;

char *readLine(FILE *);
void assertFile(FILE *, char*);
FILE *openFile(char *);
FILE *openOrCreateFile(char *);
enum dataType getDataType(char *);
void printData(Metadata *, Data **);
void raiseError(Errors);
void freeMetadata(Metadata *);
Errors printMetadata(Metadata *);
char *createStringOnHeap(char *);

#endif