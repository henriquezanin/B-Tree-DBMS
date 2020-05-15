#include <stdlib.h>
#include <stdio.h>
#include <parser.h>
#include <utils.h>
#include <query.h>

int main(int argc, char *argv[]){

    char *line = readLine(stdin);
    Metadata *metadata = parseMetadata(line);
    Errors err;
    int i;

    printf("Key name: %s\tKey Type:%s\tSize: %d\n", metadata->key.description, metadata->key.type, metadata->key.lenght);

    for(i=0;i<metadata->fieldCounter;i++){
        printf("Description: %s\tType:%s\tSize: %d\n",metadata->fields[i].description ,metadata->fields[i].type, metadata->fields[i].lenght);
    }
    printf("Register Size: %d\n", metadata->registerSize);

    while(!feof(stdin)){
        char *line = readLine(stdin);
        err = evalQuery(metadata, line);
        raiseError(err);
    }

    return 0;
}