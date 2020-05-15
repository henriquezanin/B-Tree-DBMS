#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <parser.h>


PrimaryIndex *initializePrimaryIndex(Data **data){
    PrimaryIndex *index = (PrimaryIndex*)calloc(1, sizeof(PrimaryIndex));
    index->key = data[0]->Int;
    return index;
}

Errors writeOneFieldOnFile(FILE *fp,enum dataType type, Data *data, int size){
    if(type == Int){
        fwrite(&data->Int, sizeof(int), size, fp);
        printf("Write: %d\n", data->Int);
        return SUCCESS;
    }
    else if(type == Float){
        fwrite(&data->Float, sizeof(float), size, fp);
        printf("Write: %f\n", data->Float);
        return SUCCESS;
    }
    else if(type == Double){
        fwrite(&data->Double, sizeof(double), size, fp);
        printf("Write: %lf\n", data->Double);
        return SUCCESS;
    }    
    else if(type == Char){
        fwrite(data->CharArray, sizeof(char), size, fp);
        printf("Write: %s\n", data->CharArray);
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

    //Store key
    type = getDataType(metadata->key.type);
    err = writeOneFieldOnFile(metadata->fpRegister, type, data[0], 1);
    raiseError(err);

    for(i = 0; i < metadata->fieldCounter; i++){
        type = getDataType(metadata->fields[i].type);
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