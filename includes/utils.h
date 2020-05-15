#ifndef __UTILS__
#define __UTILS__

#define BOOL unsigned char
#define TRUE 1
#define FALSE 0
//End of line
#define EOL "\n\r"

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
    char type[20];
    unsigned char lenght;

}metadataField;

typedef struct metadata{
    FILE *fpRegister;
    char registerFilename[50];
    metadataField key;
    metadataField *fields;
    int fieldCounter;
    int registerSize;
}Metadata;

typedef enum errors{
    SUCCESS,
    NULL_METADATA,
    EMPTY_QUERY,
    UNABLE_TO_WRITE_DATA,
    INVALID_DATA_TYPE,
    INVALID_QUERY
}Errors;

enum dataType{Int,Float,Double,Char};
enum operationType{insert, search};

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

#endif