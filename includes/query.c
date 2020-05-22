#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <parser.h>
#include <query.h>
#include <pager.h>


/* Main function to insert register. B-tree implementation needs to be here. */
Errors insertQuery(Metadata *metadata, char *queryData){
    Errors error;
    /*
    Until table doesn't change,the file will be open for adding data. 
    This reduces open/close operations and improves writing performance.
    */
    metadata->fpRegister = openOrCreateFile(metadata->registerFilename);
    PrimaryIndex *index = storeData(metadata, queryData);
    if(!index) error = NULL_METADATA;

    /*printf("Key:%d\tRRN: %ld\n", index->key, index->RRN);*/

    return error;
}


Errors evalQuery(Metadata *metadata, char *fullQuery){
    Errors err;
    if(!metadata) return err = NULL_METADATA;
    if(!fullQuery) return err = EMPTY_QUERY;

    char *queryType = extractParameter(fullQuery, QUERY_SEPARATOR);
    char *queryData = extractParameterValue(fullQuery,QUERY_SEPARATOR);

    if(!strcmp(queryType, "insert")){
        err = insertQuery(metadata, queryData);
        if(!err) return err;
        /*puts("INSERT");*/
        return SUCCESS;
    }
    else if(!strcmp(queryType, "search")){
        /*puts("SEARCH");*/
        return SUCCESS;
    }
    else if(!strcmp(queryType, "rrn")){
        /*puts("rrn");*/
        printByRRN(metadata, atol(queryData));
        return SUCCESS;
    }
    else if(!strcmp(queryType, "exit")){
        puts("EXIT");
        fclose(metadata->fpRegister);
        return SUCCESS;
    }
    else{
        puts("outra");
        return INVALID_QUERY;
    }
}