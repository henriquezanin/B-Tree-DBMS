#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include <query.h>
#include <menu.h>

void printCommandLineMenu(){
    printf("db > ");
}

void commandLine(){
    Errors err;
    BOOL exitFlag = FALSE;
    char *query;
    Metadata *metadata = NULL;

    /*printf("Type \"help\" to see all commands\n");*/
    while (exitFlag == FALSE){
        /*printCommandLineMenu();*/
        query = readLine(stdin);
        err = evalQuery(&metadata, query);
        switch (err){
        case EXIT:
            if(metadata){
                if(metadata->fpRegister)
                    fclose(metadata->fpRegister);
                metadata->fpRegister = NULL;
                freeMetadata(metadata);
            }
            exitFlag = TRUE;
            break;
        case INVALID_QUERY:
            /*SHOW HELP MENU*/
            break;
        default:
            raiseError(err);
            break;
        }
    }
}

void buildHelp() {
    printf("***\n");
    printf("Available commands:\n");
    printf("insert, search, rrn, table, exit\n");
    printf("Type \"man *command*\" to see their manual\n");
    printf("***\n");
}

void runManuals() {
    printf("*** Aqui vai aparecer o manual do que o usuario quer ***\n");
}
