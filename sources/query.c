#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <parser.h>
#include <query.h>
#include <pager.h>
#include <btree.h>
#include <menu.h>

enum operationType getOperationFromString(char *command){
    enum operationType operation;
    if(!strcmp(command, "insert"))
        operation = insert;
    else if(!strcmp(command, "search"))
        operation = search;
    else if(!strcmp(command, "rrn"))
        operation = rrn;
    else if(!strcmp(command, "load"))
        operation = load;
    else if(!strcmp(command, "table"))
        operation = table;
    else if(!strcmp(command, "exit"))
        operation = exitCommand;
    else if(!strcmp(command, "help"))
        operation = showHelp;
    else if(!strcmp(command, "man"))
        operation = man;
    else{
        raiseError(INVALID_QUERY);
        operation = showHelp;
    }
    return operation;
}

/* Main function to insert register. B-tree implementation needs to be here. */
Errors insertQuery(Metadata *metadata, char *queryData){
    if(!metadata) return NULL_METADATA;
    Errors error;
    /*
    Until table doesn't change the file still opened for adding data. 
    This reduces open/close operations and improves writing performance.
    */
    if(!metadata->fpRegister)
        metadata->fpRegister = openOrCreateFile(metadata->registerFilename);
    PrimaryIndex *index = storeData(metadata, queryData);
    if(!index) error = NULL_METADATA;

    /*TESTE DA B-TREE*/
    FILE *fp = openOrCreateFile("index.dat");
    btPage *root = getRoot(fp);
    /*printPageNode(root);
    printf("\n");*/
    raiseError(bTreeInsert(index, root, fp));

    return error;
}

int selectQuery(Metadata *metadata, char *queryData){
    if(!metadata) return NULL_METADATA;
    long keyRrn;
    FILE *fp = openOrCreateFile("index.dat");
    btPage *root = getRoot(fp);
    keyRrn = bTreeSelect(root, atoi(queryData),fp);
    if(keyRrn < 0){
        printf("Key not found!\n");
        return -1;
    }
    printByRRN(metadata, keyRrn);
    return 0;
}

Errors loadTable(char *filename, Metadata **metadata){
    if(*metadata){
        fclose((*metadata)->fpRegister);
        freeMetadata(*metadata);
    }
    *metadata = parseMetadata(filename);
    return SUCCESS;
}

Errors evalQuery(Metadata **metadata, char *fullQuery){
    Errors err;
    if(!fullQuery) return err = EMPTY_QUERY;

    char *queryType = extractParameter(fullQuery, QUERY_SEPARATOR);
    char *queryData = extractParameterValue(fullQuery,QUERY_SEPARATOR);
    enum operationType operation = getOperationFromString(queryType);

    switch (operation){
    case insert:
        err = insertQuery(*metadata, queryData);
        break;
    case search:
        err = selectQuery(*metadata, queryData);
        break;
    case rrn:
        printByRRN(*metadata, atol(queryData));
        break;
    case load:
        /*Load table file*/
        loadTable(queryData, metadata);
        printMetadata(*metadata);
        break;
    case table:
        printMetadata(*metadata);
        break;
    case exitCommand:
        err = EXIT;
        break;
    case showHelp:
        buildHelp();
        break;
    case man:
        runManuals();
        break;
    default:
    err = INVALID_QUERY;
        break;
    }
    return err;
}