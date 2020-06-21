/*
Henrique Gomes Zanin NUSP: 10441321
Gabriel Guimaraes Vilas Boas Marin NUSP: 11218521
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <parser.h>
#include <utils.h>

TextDocument *readFileUntilEOF(FILE *fp){
    TextDocument *document = (TextDocument*)calloc(1,sizeof(TextDocument));
    int size = 0;
    while(!feof(fp)){
        document->text = (char**)realloc(document->text,(size+1)*sizeof(char*));
        document->text[size++] = readLine(fp);
    }
    document->numberOfLines = size;
    return document;
}

/*Get parameter from query*/
char *extractParameter(char *string, char parameterSep){
    char *parameter;
    int stringLenght = strlen(string);
    int subStringSize = 0;
    while(string[subStringSize] != parameterSep && subStringSize < stringLenght && ++subStringSize);
    parameter = (char*)calloc(subStringSize+1, sizeof(char));
    strncpy(parameter, string, subStringSize);
    return parameter;
}

char *removeSpaces(char *string){
    int i = 0,newSize = 0;
    char *aux = (char*)calloc(strlen(string),sizeof(char));
    for(i=0;i<strlen(string);i++){
        if(string[i] != SPACE && string[i] != STRING_DELIMITER)
            aux[newSize++] = string[i];
        else if(string[i] == STRING_DELIMITER){
            aux[newSize++] = string[i++];
            while(string[i] != STRING_DELIMITER) aux[newSize++] = string[i++];
            aux[newSize++] = string[i];
        }
    }

    string = (char*)realloc(string,(strlen(aux)+1)*sizeof(char));
    strcpy(string, aux);
    free(aux);
    return string;
}
/*Get parameter value from query and remove spaces*/
char *extractParameterValue(char *string, char parameterSep){
    char *value, *startOfSubstring;
    int stringLenght = strlen(string);
    int subStringSize = 0;
    
    while(string[subStringSize] != parameterSep && subStringSize < stringLenght && ++subStringSize);
    startOfSubstring = string + (subStringSize+1);
    value = (char*)calloc(strlen(startOfSubstring)+1, sizeof(char));
    strcpy(value, startOfSubstring);
    value = removeSpaces(value);
    return value;
}

BOOL isFileName(char *parameter){
    return (BOOL)(!strcmp(parameter, "filename"));
}
BOOL isKey(char *parameter){
    if(strstr(parameter, "key")) return TRUE;
    else return FALSE;
}
BOOL isField(char *parameter){
    if(strstr(parameter, "field")) return TRUE;
    else return FALSE;
}

void loadFileNameIntoMetadata(Metadata *metadata, char *fullParameter){
    char *parameterValue = extractParameterValue(fullParameter, METADATA_PARAMETER_SEP);
    strcpy(metadata->registerFilename, parameterValue);
    free(parameterValue);
}

void raiseErrorAndExit(char *error){
    printf("%s, exiting program!\n", error);
    exit(0);
}

/*Return an enum with data type*/
enum ParameterType getParameterType(char *parameter){
    enum ParameterType type;
    if(strstr(parameter,"name")) type = Name;
    else if (strstr(parameter,"type")) type = Type;
    else{
        char error[80];
        strcpy(error, "Parameter ");
        strcat(error, parameter);
        strcat(error, " have incorrect format");
        raiseErrorAndExit(error);
    }
    return type;
}
/*Load primary key fields into metadata TAD*/
void loadKeyIntoMetadata(Metadata *metadata, char *fullParameter){
    char *parameter = extractParameter(fullParameter, METADATA_PARAMETER_SEP);
    char *parameterValue = extractParameterValue(fullParameter, METADATA_PARAMETER_SEP);
    enum ParameterType type = getParameterType(parameter);
    if(type == Name)
        strcpy(metadata->key.description,parameterValue);
    else
        metadata->key.type = getDataType(parameterValue);
    metadata->key.lenght = 1;
}

/*Get char size defined in metadata file knowed as table file*/
unsigned int getCharTypeSize(char *parameterValue){
    unsigned int size = 1, i = 0;
    char *p, digit[8];
    while(parameterValue[i++] != '[' && i < strlen(parameterValue));
    if(i >= strlen(parameterValue)) return 1;
    p = &parameterValue[i];
    i = 0;
    while(isdigit(p[i++]));
    strncpy(digit,p,--i);
    digit[i] = '\0';
    size = atoi(digit);
    return size;
}

void setDataTypeInMetadata(metadataField *field, char *parameterValue){
    field->type = getDataType(parameterValue);
    if(field->type != Char)
        field->lenght = 1;
    else
        field->lenght = getCharTypeSize(parameterValue);
}

void loadFieldIntoMetadata(Metadata *metadata, char *fullParameter){
    char *parameter = extractParameter(fullParameter, METADATA_PARAMETER_SEP);
    char *parameterValue = extractParameterValue(fullParameter, METADATA_PARAMETER_SEP);
    enum ParameterType ParameterType = getParameterType(parameter);
    if(ParameterType == Name){
        metadata->fields = (metadataField*)realloc(metadata->fields, (metadata->fieldCounter+1)*sizeof(metadataField));
        strcpy(metadata->fields[metadata->fieldCounter].description, parameterValue);
        metadata->fieldCounter++;
    }
    else{
        setDataTypeInMetadata(&(metadata->fields[metadata->fieldCounter-1]), parameterValue);
    }
}

BOOL isNotComment(char *line){
    if(strlen(line) >= strlen(COMMENT) ){
        if(!strncmp(line, COMMENT, strlen(COMMENT))){
            return FALSE;
        }
    }
    return TRUE;
}

Metadata *parseMetadataFile(TextDocument *document){
    int i;
    char *parameter;
    Metadata *metadata = (Metadata*)calloc(1, sizeof(Metadata));

    for(i=0;i<document->numberOfLines;i++){
        if(strlen(document->text[i]) && isNotComment(document->text[i])){
            parameter = extractParameter(document->text[i], METADATA_PARAMETER_SEP);
            if(isFileName(parameter)){
                loadFileNameIntoMetadata(metadata,document->text[i]);
            }
            else if(isKey(parameter)){
                loadKeyIntoMetadata(metadata, document->text[i]);
            }
            else if(isField(parameter)){
                loadFieldIntoMetadata(metadata,document->text[i]);
            }
            else{
                printf("Invalid parameter in Metadata file: %s\tIgnoring field.\n", document->text[i]);
            }
            free(parameter);
        }
    }

    return metadata;
}

void calcRegisterSize(Metadata *metadata){
    int i;

    if(metadata->key.type == Int)
        metadata->registerSize += sizeof(int);
    
    for(i=0;i<metadata->fieldCounter;i++){
        if(metadata->fields[i].type == Int)
            metadata->registerSize += sizeof(int);
        else if(metadata->fields[i].type == Float)
            metadata->registerSize += sizeof(float);
        if(metadata->fields[i].type == Double)
            metadata->registerSize += sizeof(double);
        if(metadata->fields[i].type == Char)
            metadata->registerSize += metadata->fields[i].lenght;
    }

}

Metadata *parseMetadata(char *filename){
    FILE *fp = openFile(filename);
    TextDocument *metadataFileLines = readFileUntilEOF(fp);
    Metadata *metadata = parseMetadataFile(metadataFileLines);
    calcRegisterSize(metadata);
    fclose(fp);
    return metadata;
}

char **extractFields(char *query,char queryDelimiter,char stringDelimiter){
    if(!query) return NULL;
    int numberOfFields = 1;
    char **fields = (char**)malloc(sizeof(char*));
    char *aux = NULL;
    char *start = query, *end;

    while((end = strchr(start,queryDelimiter))){
        /* Ignore query delimiter in quoted string with stringDelimiter */
        if(start[0] == stringDelimiter){
            end = strchr(&start[1],stringDelimiter);
            end += 1;
        }
        aux = (char*)calloc((end-start)+1,sizeof(char));
        strncpy(aux,start,end-start);
        fields = (char**)realloc(fields, (numberOfFields+1)*sizeof(char*));
        fields[numberOfFields-1] = aux;
        numberOfFields++;
        start = end+1;
    }
    /* Takes last field that's haven't a delimiter at end */
    if(strlen(start)){
        aux = (char*)calloc(strlen(start)+1,sizeof(char));
        strncpy(aux,start,strlen(start));
        fields = (char**)realloc(fields, (numberOfFields+1)*sizeof(char*));
        fields[numberOfFields-1] = aux;
        numberOfFields++;
    }

    return fields;
}

char *removeQuotesInString(char *string){
    if(string[0] != STRING_DELIMITER) return string;
    char *newString = (char*) calloc((strlen(string)-1),sizeof(char));
    string[strlen(string)-1] = '\0';
    strncpy(newString,&string[1],strlen(string)-1);
    free(string);
    return newString;
}

Data *convertStringToData(char *field, enum dataType type, int size){
    Data *data = (Data*)malloc(sizeof(Data));
    char *aux;
    switch (type){
    case Int:
        data->Int = atoi(field);
        break;
    case Float:
        data->Float = atof(field);
        break;
    case Double:
        data->Double = atof(field);
        break;
    case Char:
        aux = (char*)calloc(size,sizeof(char));
        strcpy(aux, field);
        data->CharArray = aux;
        break;
    
    default:
        break;
    }
    return data;
}

Data **parseInsertQueryData(Metadata *metadata, char *query){
    int i;
    enum dataType type;
    Data **data = (Data**)malloc((metadata->fieldCounter+1)*sizeof(Data*));
    char **fields = extractFields(query, FIELD_DELIMITER, STRING_DELIMITER);

    type = metadata->key.type;
    data[0] = convertStringToData(fields[0],type, metadata->key.lenght);

    for(i=0;i<metadata->fieldCounter;i++){
        fields[i+1] = removeQuotesInString(fields[i+1]);
        type = metadata->fields[i].type;
        data[i+1] = convertStringToData(fields[i+1], type, metadata->fields[i].lenght);
    }

    for(i=0;i<=metadata->fieldCounter;i++){
        free(fields[i]);
    }
    free(fields);

    return data;
}

char *extractKeyFromInsertQuery(char *queryData){
    int size = 0;
    char *digit;
    while(isdigit(queryData[size]))
        size++;
    digit = (char*)calloc(size+1, sizeof(char));
    strncpy(digit,queryData,size);
    return digit;
}

char *generateIndexNameFromRegisterName(char *regFilename){
    char *dotOcurrence = strchr(regFilename, '.');
    int filenameSize = dotOcurrence - regFilename;
    char *indexFilename = (char *)calloc(filenameSize+5, sizeof(char));
    strncat(indexFilename,regFilename,filenameSize);
    strcat(indexFilename,".idx");
    return indexFilename;
}