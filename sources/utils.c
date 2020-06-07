#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include <string.h>

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

enum dataType getDataType(char *parameter){
    enum dataType type;

    if(strstr(parameter, "int")) type = Int;
    if(strstr(parameter, "float")) type = Float;
    if(strstr(parameter, "double")) type = Double;
    if(strstr(parameter, "char")) type = Char;

    return type;
}

void raiseError(Errors error){
    if(error == UNABLE_TO_WRITE_DATA) printf("Unable to write data on file!\n");
    else if(error == INVALID_DATA_TYPE) printf("Invalid data type!\n");
    else if(error == NULL_METADATA) printf("NULL Metadata in function argument!\n");
    else if(error == EMPTY_QUERY) printf("Empty query in function argument!\n");
    else if(error == INVALID_QUERY) printf("Invalid query!\n");
    else if(error == INVALID_RRN_PAGE){ 
        printf("Invalid RRN page!\n");
        exit(0);
    }
    else if(error == EMPTY_PAGE) printf("Empty page on B-Tree!\n");
    else if(error == KEY_NOT_FOUND) printf("Key not found!\n");
    else if(error == KEY_ALREADY_EXISTS) printf("Key already exists!\n");
    else if(error == INVALID_FILE_POINTER) printf("Register or index file pointer error!\n");
}

void freeMetadata(Metadata *metadata){
    free(metadata->fields);
    free(metadata);
}

/* Only for test */
void printData(Metadata *metadata, Data **data){
    enum dataType type;
    int i;

    printf("%d\n", data[0]->Int);

    for(i=0;i<metadata->fieldCounter;i++){
        type = metadata->fields[i].type;
        if(type == Int) printf("%d\n", data[i+1]->Int);
        else if(type == Float) printf("%f\n", data[i+1]->Float);
        else if(type == Double) printf("%lf\n", data[i+1]->Double);
        else if(type == Char) printf("%s\n", data[i+1]->CharArray);
    }
    printf("\n");
}

char *createStringOnHeap(char *string){
    char *new = (char*)calloc(strlen(string),sizeof(char));
    strcpy(new,string);
    return new;
}

char *convertTypeToString(enum dataType type){
    char *stringType;
    switch (type){
    case Int:
        stringType = createStringOnHeap("Int");
        break;
    case Float: 
        stringType = createStringOnHeap("Float");
        break;
    case Double: 
        stringType = createStringOnHeap("Double");
        break;
    case Char: 
        stringType = createStringOnHeap("Char");
        break;
    default:
        stringType = NULL;
        break;
    }
    return stringType;
}

Errors printMetadata(Metadata *metadata){
    if(!metadata) return NULL_METADATA;
    int i;
    char *type = convertTypeToString(metadata->key.type);
    printf("\nKey name: %s\tKey Type:%s\n\n", metadata->key.description, type);

    for(i=0;i<metadata->fieldCounter;i++){
        type = convertTypeToString(metadata->fields[i].type);
        if(metadata->fields[i].type != Char)
            printf("Description: %s\tType: %s\n",metadata->fields[i].description ,type);
        else
            printf("Description: %s\tType: %s[%d]\n",
            metadata->fields[i].description ,type, metadata->fields[i].lenght);
    }
    printf("Register Size: %d\n\n", metadata->registerSize);
    return SUCCESS;
}