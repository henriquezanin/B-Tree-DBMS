/*
Henrique Gomes Zanin NUSP: 10441321
Gabriel Guimaraes Vilas Boas Marin NUSP: 11218521
*/
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
    char *key = extractKeyFromInsertQuery(queryData);
    if(selectQuery(metadata,key,FALSE) == SUCCESS)
        return KEY_ALREADY_EXISTS;
    PrimaryIndex *index = storeData(metadata, queryData);
    if(!index) error = NULL_METADATA;
     
    btPage *root = getOrCreateRoot(metadata->fpIndex);
    raiseError(bTreeInsert(index, root, metadata->fpIndex));

    return error;
}

Errors selectQuery(Metadata *metadata, char *queryData, BOOL printable){ /* Se for 0 eh busca se for 1 eh consulta */
    if(!metadata) return NULL_METADATA;
    long keyRrn;
    btPage *root = getRoot(metadata->fpIndex);
    if(!root) return KEY_NOT_FOUND;
    keyRrn = bTreeSelect(root, atoi(queryData),metadata->fpIndex);
    if(keyRrn < 0){
        return KEY_NOT_FOUND;
    }
    if (printable == TRUE) printByRRN(metadata, keyRrn);
    return SUCCESS;
}

Errors loadTable(char *filename, Metadata **metadata){
    if(*metadata){
        if((*metadata)->fpRegister)
            fclose((*metadata)->fpRegister);
        if((*metadata)->fpIndex)
            fclose((*metadata)->fpIndex);
        freeMetadata(*metadata);
    }
    *metadata = parseMetadata(filename);
    (*metadata)->fpRegister = openOrCreateFile((*metadata)->registerFilename);
    (*metadata)->fpIndex = openOrCreateFile(generateIndexNameFromRegisterName((*metadata)->registerFilename));
    return SUCCESS;
}

/*
    Until table doesn't change the file still opened for adding data. 
    This reduces open/close operations and improves writing performance.
*/
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
        err = selectQuery(*metadata, queryData, TRUE);
        break;
    case rrn:
        printByRRN(*metadata, atol(queryData));
        break;
    case load:
        /*Load table file*/
        loadTable(queryData, metadata);
        /* printMetadata(*metadata); */
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
        runManuals(queryData);
        break;
    default:
    err = INVALID_QUERY;
        break;
    }
    return err;
}