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

    printf("Type \"help\" to see all commands\n");
    while (exitFlag == FALSE){
        printCommandLineMenu();
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
    manualInformations manInfo;


    if(!strcmp(command, "insert")) {
        
        manInfo.name =  "insert -- insert a person in the DBMS";
        manInfo.synopsis = "insert [integer,\"string\",integer,float]";
        manInfo.description = "For each operand that insert a person in this DBMS, you register it on a file with a index made with paged b-tree increasing it speed for big storage of data";
        manInfo.exampleHeader = "The following is how to insert an person by user input";
        manInfo.exampleText = "insert 1,\"gabriel\",20,300.00";
        manInfo.history = "An insert command appeared in Version 1";

        printMan(&manInfo, &terminal);

    } else if(!strcmp(command, "search")) {

        manInfo.name = "search -- search a person in the DBMS";
        manInfo.synopsis = "search [integer]";
        manInfo.description = "For each operand, this command searches a person that is registered on the DBMS printing all the data saved";
        manInfo.exampleHeader = "The following is how to search an person by user input";
        manInfo.exampleText = "search 1";
        manInfo.history = "An search command appeared in Version 1";

        printMan(&manInfo, &terminal);
        
    } else if(!strcmp(command, "rrn")) {

        manInfo.name = "rrn -- search register by his position registered";
        manInfo.synopsis = "rrn [integer]";
        manInfo.description = "For each operand seek to te position passed as a parameter in the register archive, really good for testing and visualizing things";
        manInfo.exampleHeader = "The following is how to seek an register by user input";
        manInfo.exampleText = "rrn 1";
        manInfo.history = "An rrn command appeared in Version 1";

        printMan(&manInfo, &terminal);

    } else if(!strcmp(command, "load")) {

        manInfo.name = "load -- load a file in the DBMS";
        manInfo.synopsis = "load [string]";
        manInfo.description = "For each operand, it loads an file with registered persons to query every thing you need";
        manInfo.exampleHeader = "The following is how to load a file by user input";
        manInfo.exampleText = "load 1.dat";
        manInfo.history = "An load command appeared in Version 1";

        printMan(&manInfo, &terminal);

    } else if(!strcmp(command, "table")) {

        manInfo.name = "table -- shows table information";
        manInfo.synopsis = "table";
        manInfo.description = "For each operand prints a few informations about the table that have been loaded and are being used to inser, search and etc";
        manInfo.exampleHeader = "The following is how to use table by user input";
        manInfo.exampleText = "table";
        manInfo.history = "An table command appeared in Version 1";

        printMan(&manInfo, &terminal);

    } else if(!strcmp(command, "exit")) {

        manInfo.name = "exit -- exit the program";
        manInfo.synopsis = "exit";
        manInfo.description = "For this single operand, it exit the program writing all the registered persons and needed information";
        manInfo.exampleHeader = "The following is how to exit the program by user input";
        manInfo.exampleText = "exit";
        manInfo.history = "An exit command appeared in Version 1";

        printMan(&manInfo, &terminal);

    } else if(!strcmp(command, "help")) {

        manInfo.name = "help -- print help menu";
        manInfo.synopsis = "help";
        manInfo.description = "For each operand, this command prints all available commands and shows how to acess their manual";
        manInfo.exampleHeader = "The following is how to use help by user input";
        manInfo.exampleText = "help";
        manInfo.history = "An help command appeared in Version 1";

        printMan(&manInfo, &terminal);

    } else{
        printf("INVALID COMMAND\n");
    }
}

void printMan(manualInformations *infos, struct winsize *terminal) {
    int descSize = strlen(infos->description);
    int i = 0, j = 0;

    printf("\t\t\t Very fast DBMS\n");
        printf("NAME\n");
        printf("\t%s\n\n", infos->name);
        printf("SYNOPSIS\n");
        printf("\t%s\n\n", infos->synopsis);
        printf("DESCRIPTION\n");
         while(i < descSize) {
            printf("\t");
            if (infos->description[i] == ' ') i++;
            for (j = 0; (j < terminal->ws_col-8 && i < descSize); j++, i++) {
                printf("%c", infos->description[i]);
            }
            printf("\n");
        }
        printf("\n");
        printf("EXAMPLES\n");
        printf("\t%s\n\n", infos->exampleHeader);
        printf("\t%s\n\n", infos->exampleText);
        printf("HISTORY\n");
        printf("\t%s\n\n", infos->history);
}