#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <menu.h>
#include <parser.h>
#include <utils.h>
#include <query.h>

int main(int argc, char *argv[]){

    if(argc > 1){
        if(!strcmp(argv[1], "--cmd")){
            commandLine();
        }
    }
    
    /*Errors err;
    int i;*/

    /*printf("Key name: %s\tKey Type:%d\tSize: %d\n", metadata->key.description, metadata->key.type, metadata->key.lenght);

    for(i=0;i<metadata->fieldCounter;i++){
        printf("Description: %s\tType:%d\tSize: %d\n",metadata->fields[i].description ,metadata->fields[i].type, metadata->fields[i].lenght);
    }
    printf("Register Size: %d\n\n", metadata->registerSize);*/

    /*while(!feof(stdin)){
        char *line = readLine(stdin);
        err = evalQuery(metadata, line);
        raiseError(err);
    }*/

    return 0;
}