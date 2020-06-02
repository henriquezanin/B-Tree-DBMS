#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include <query.h>
#include <menu.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

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

void runManuals(char *command) {
    struct winsize terminal;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal);
    int i = 0, j = 0;

    if(!strcmp(command, "insert")) {

        char *description = "For each operand that insert a person in this DBMS, you register it on a file with a index made with paged b-tree increasing it speed for big storage of data";
        printf("\t\t\t Very fast DBMS\n");
        printf("NAME\n");
        printf("\tinsert -- insert a person in the DBMS\n\n");
        printf("SYNOPSIS\n");
        printf("\tinsert [integer,\"string\",integer,float]\n\n");
        printf("DESCRIPTION\n");
        while(i < strlen(description)) {
            printf("\t");
            if (description[i] == ' ') i++;
            for (j = 0; (j < terminal.ws_col-8 && i < strlen(description)); j++, i++) {
                printf("%c", description[i]);
            }
            printf("\n");
        }
        printf("\n");
        printf("EXAMPLES\n");
        printf("\tThe following is how to insert an person by user input\n\n");
        printf("\tinsert 1,\"gabriel\",20,300.00\n\n");
        printf("HISTORY\n");
        printf("An insert command appeared in Version 1\n\n");

    } else if(!strcmp(command, "search")) {

        char *description = "For each operand, this command searches a person that is registered on the DBMS printing all the data saved";
        printf("\t\t\t Very fast DBMS\n");
        printf("NAME\n");
        printf("\tsearch -- search a person in the DBMS\n\n");
        printf("SYNOPSIS\n");
        printf("\tsearch [integer]\n\n");
        printf("DESCRIPTION\n");
        while(i < strlen(description)) {
            printf("\t");
            if (description[i] == ' ') i++;
            for (j = 0; (j < terminal.ws_col-8 && i < strlen(description)); j++, i++) {
                printf("%c", description[i]);
            }
            printf("\n");
        }
        printf("\n");
        printf("EXAMPLES\n");
        printf("\tThe following is how to search an person by user input\n\n");
        printf("\tsearch 1\n\n");
        printf("HISTORY\n");
        printf("An search command appeared in Version 1\n\n");
        
    } else if(!strcmp(command, "rrn")) {

        char *description = "descricao aqui";
        printf("\t\t\t Very fast DBMS\n");
        printf("NAME\n");
        printf("\trrn -- testinho sobre isso\n\n");
        printf("SYNOPSIS\n");
        printf("\trrn [variaveis aqui]\n\n");
        printf("DESCRIPTION\n");
        while(i < strlen(description)) {
            printf("\t");
            if (description[i] == ' ') i++;
            for (j = 0; (j < terminal.ws_col-8 && i < strlen(description)); j++, i++) {
                printf("%c", description[i]);
            }
            printf("\n");
        }
        printf("\n");
        printf("EXAMPLES\n");
        printf("\tThe following is how to insert an person by user input\n\n");
        printf("\trrn taltaltal\n\n");
        printf("HISTORY\n");
        printf("An rrn command appeared in Version 1\n\n");

    } else if(!strcmp(command, "load")) {

        char *description = "For each operand, it loads an file with registered persons to query every thing you need";
        printf("\t\t\t Very fast DBMS\n");
        printf("NAME\n");
        printf("\tload -- load a file in the DBMS\n\n");
        printf("SYNOPSIS\n");
        printf("\tload [string]\n\n");
        printf("DESCRIPTION\n");
        while(i < strlen(description)) {
            printf("\t");
            if (description[i] == ' ') i++;
            for (j = 0; (j < terminal.ws_col-8 && i < strlen(description)); j++, i++) {
                printf("%c", description[i]);
            }
            printf("\n");
        }
        printf("\n");
        printf("EXAMPLES\n");
        printf("\tThe following is how to load a file by user input\n\n");
        printf("\tload 1.dat\n\n");
        printf("HISTORY\n");
        printf("An load command appeared in Version 1\n\n");

    } else if(!strcmp(command, "table")) {

        char *description = "For each operand that insert a person in this DBMS, you register it on a file with a index made with paged b-tree increasing it speed for big storage of data";
        printf("\t\t\t Very fast DBMS\n");
        printf("NAME\n");
        printf("\tinsert -- insert a person in the DBMS\n\n");
        printf("SYNOPSIS\n");
        printf("\tinsert [integer,\"string\",integer,float]\n\n");
        printf("DESCRIPTION\n");
        while(i < strlen(description)) {
            printf("\t");
            if (description[i] == ' ') i++;
            for (j = 0; (j < terminal.ws_col-8 && i < strlen(description)); j++, i++) {
                printf("%c", description[i]);
            }
            printf("\n");
        }
        printf("\n");
        printf("EXAMPLES\n");
        printf("\tThe following is how to insert an person by user input\n\n");
        printf("\tinsert 1,\"gabriel\",20,300.00\n\n");
        printf("HISTORY\n");
        printf("An insert command appeared in Version 1\n\n");

    } else if(!strcmp(command, "exit")) {

        char *description = "For this single operand, it exit the program writing all the registered persons and needed information";
        printf("\t\t\t Very fast DBMS\n");
        printf("NAME\n");
        printf("\texit -- exit the program\n\n");
        printf("SYNOPSIS\n");
        printf("\texit\n\n");
        printf("DESCRIPTION\n");
        while(i < strlen(description)) {
            printf("\t");
            if (description[i] == ' ') i++;
            for (j = 0; (j < terminal.ws_col-8 && i < strlen(description)); j++, i++) {
                printf("%c", description[i]);
            }
            printf("\n");
        }
        printf("\n");
        printf("EXAMPLES\n");
        printf("\tThe following is how to exit the program by user input\n\n");
        printf("\texit\n\n");
        printf("HISTORY\n");
        printf("An exit command appeared in Version 1\n\n");

    } else if(!strcmp(command, "help")) {

        char *description = "For each operand, this command prints all available commands and shows how to acess their manual";
        printf("\t\t\t Very fast DBMS\n");
        printf("NAME\n");
        printf("\thelp -- print help menu\n\n");
        printf("SYNOPSIS\n");
        printf("\thelp\n\n");
        printf("DESCRIPTION\n");
        while(i < strlen(description)) {
            printf("\t");
            if (description[i] == ' ') i++;
            for (j = 0; (j < terminal.ws_col-8 && i < strlen(description)); j++, i++) {
                printf("%c", description[i]);
            }
            printf("\n");
        }
        printf("\n");
        printf("EXAMPLES\n");
        printf("\tThe following is how to use help by user input\n\n");
        printf("\thelp\n\n");
        printf("HISTORY\n");
        printf("An help command appeared in Version 1\n\n");

    } else{
        printf("INVALID COMMAND\n");
    }
}
