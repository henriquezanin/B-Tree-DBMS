#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <parser.h>

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
    if(metadata->key.type == Int)
        printf("%s: %d\n", metadata->key.description, data[0]->Int);
    else if(metadata->key.type == Char)
        printf("%s: %s\n", metadata->key.description, data[0]->CharArray);

    for(i = 0; i < metadata->fieldCounter; i++){
        if(metadata->fields[i].type == Int)
            printf("%s: %d\n", metadata->fields[i].description, data[i+1]->Int);
        else if(metadata->fields[i].type == Float)
            printf("%s: %.2f\n", metadata->fields[i].description, data[i+1]->Float);
        else if(metadata->fields[i].type == Double)
            printf("%s: %.2lf\n", metadata->fields[i].description, data[i+1]->Double);
        else if(metadata->fields[i].type == Char)
            printf("%s: %s\n", metadata->fields[i].description, data[i+1]->CharArray);
    }
    printf("\n");
}