/*
Henrique Gomes Zanin NUSP: 10441321
Gabriel Guimaraes Vilas Boas Marin NUSP: 11218521
*/
#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <parser.h>
#include <menu.h>
#include <limits.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

Data *getRegisterFieldFromFile(FILE *, enum dataType, unsigned int);

Data **getRegisterByRRN(Metadata *metadata, long rrn){
    if(!metadata->fpRegister)
        metadata->fpRegister = openOrCreateFile(metadata->registerFilename);
    assertFile(metadata->fpRegister,metadata->registerFilename);
    Data **data = (Data**)malloc((metadata->fieldCounter+1)*sizeof(Data*));
    short int i;
    fseek(metadata->fpRegister, (rrn-1)*metadata->registerSize, SEEK_SET);
    /*Read key from register file*/
    data[0] = getRegisterFieldFromFile(metadata->fpRegister, metadata->key.type, metadata->key.lenght);
    /*retrieves other fields*/
    for(i=0;i<metadata->fieldCounter;i++)
        data[i+1] = getRegisterFieldFromFile(metadata->fpRegister, metadata->fields[i].type, metadata->fields[i].lenght);

    return data;
}

Data *getRegisterFieldFromFile(FILE *fp, enum dataType type, unsigned int size){
    Data *data = (Data*)malloc(sizeof(Data));
    if(type == Int)
        fread(&data->Int, sizeof(int), size, fp);
    else if(type == Float)
        fread(&data->Float, sizeof(float), size, fp);
    else if(type == Double)
        fread(&data->Double, sizeof(double), size, fp);
    else if(type == Char){
        data->CharArray = (char*)calloc(size, sizeof(char));
        fread(data->CharArray, sizeof(char), size, fp);
    }
    return data;
}

PrimaryIndex *initializePrimaryIndex(Data **data){
    PrimaryIndex *index = (PrimaryIndex*)calloc(1, sizeof(PrimaryIndex));
    index->key = data[0]->Int;
    return index;
}

Errors writeOneFieldOnFile(FILE *fp,enum dataType type, Data *data, int size){
    if(type == Int){
        fwrite(&data->Int, sizeof(int), size, fp);
        return SUCCESS;
    }
    else if(type == Float){
        fwrite(&data->Float, sizeof(float), size, fp);
        return SUCCESS;
    }
    else if(type == Double){
        fwrite(&data->Double, sizeof(double), size, fp);
        return SUCCESS;
    }    
    else if(type == Char){
        fwrite(data->CharArray, sizeof(char), size, fp);
        return SUCCESS;
    }
    else{
        return INVALID_DATA_TYPE;
    }
}

long writeAllFieldsOnFile(Metadata *metadata, Data **data){
    assertFile(metadata->fpRegister,metadata->registerFilename);
    fseek(metadata->fpRegister, 0L, SEEK_END);
    int i;
    long RRN;
    enum dataType type;
    Errors err;
    /* Store key */
    type = metadata->key.type;
    err = writeOneFieldOnFile(metadata->fpRegister, type, data[0], 1);
    raiseError(err);
    for(i = 0; i < metadata->fieldCounter; i++){
        type = metadata->fields[i].type;
        err = writeOneFieldOnFile(metadata->fpRegister, type, data[i+1], metadata->fields[i].lenght);
        raiseError(err);
    }
    RRN = ftell(metadata->fpRegister)/metadata->registerSize;
    fflush(metadata->fpRegister);

    return RRN;
}

PrimaryIndex *storeData(Metadata *metadata, char *queryData){
    if(!metadata || !queryData) return NULL;
    Data **data = parseInsertQueryData(metadata, queryData);
    PrimaryIndex *index = initializePrimaryIndex(data);
    index->RRN = writeAllFieldsOnFile(metadata, data);

    return index;
}

void printByRRN(Metadata *metadata, long rrn){
    Data **data = getRegisterByRRN(metadata, rrn);
    short int i;

    screenContent *message = (screenContent*) calloc(1, sizeof(screenContent));
    struct winsize terminal;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal);

    message->numberOfStrings = 1+metadata->fieldCounter;
    message->strings = (char**) calloc(message->numberOfStrings, sizeof(char*));
    message->strings[0] = createStringOnHeap("Busca feita com sucesso, digite 3 para ver os comandos");
    
    if(metadata->key.type == Int) {
        message->strings[1] = (char*) calloc(strlen(metadata->key.description)+12, sizeof(char)); /* +12 pra caber qualquer inteiro */
        sprintf(message->strings[1], "%s: %d", metadata->key.description, data[0]->Int);
    }
        /* printf("%s: %d\n", metadata->key.description, data[0]->Int); */
    else if(metadata->key.type == Char) {
        message->strings[1] = (char*) calloc(strlen(metadata->key.description)+strlen(data[0]->CharArray), sizeof(char));
        sprintf(message->strings[1], "%s: %s", metadata->key.description, data[0]->CharArray);
    }
        /* printf("%s: %s\n", metadata->key.description, data[0]->CharArray); */

    for(i = 0; i < metadata->fieldCounter; i++){
        if(metadata->fields[i].type == Int) {
            message->strings[i+1] = (char*) calloc(strlen(metadata->fields[i].description)+12, sizeof(char));
            sprintf(message->strings[i+1], "%s: %d", metadata->fields[i].description, data[i+1]->Int);
        }
            /* printf("%s: %d\n", metadata->fields[i].description, data[i+1]->Int); */
        else if(metadata->fields[i].type == Float) {
            message->strings[i+1] = (char*) calloc(strlen(metadata->fields[i].description)+24, sizeof(char)); /* +24 pra caber qualquer Float/Double */
            sprintf(message->strings[i+1], "%s: %.2f", metadata->fields[i].description, data[i+1]->Float);
        }
            /* printf("%s: %.2f\n", metadata->fields[i].description, data[i+1]->Float); */
        else if(metadata->fields[i].type == Double) {
            message->strings[i+1] = (char*) calloc(strlen(metadata->fields[i].description)+24, sizeof(char));
            sprintf(message->strings[i+1], "%s: %.2f", metadata->fields[i].description, data[i+1]->Double);
        }
            /* printf("%s: %.2lf\n", metadata->fields[i].description, data[i+1]->Double); */
        else if(metadata->fields[i].type == Char) {
            message->strings[i+1] = (char*) calloc(strlen(metadata->fields[i].description)+strlen(data[i+1]->CharArray), sizeof(char));
            /* twoStringConcat(message->strings[i+1], metadata->fields[i].description); */
            strcpy(message->strings[i+1], metadata->fields[i].description);
            message->strings[i+1] = twoStringConcat(message->strings[i+1], ": ");
            message->strings[i+1] = twoStringConcat(message->strings[i+1], data[i+1]->CharArray);
        }
            /* printf("%s: %s\n", metadata->fields[i].description, data[i+1]->CharArray); */
    }
    /*  printf("\n"); */
    printMainText(&terminal, message);
    freeScreenContent(message);
}