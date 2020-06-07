#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include <query.h>
#include <menu.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>

/* Printa indicador do command line */
void printCommandLineMenu(){
    printf("db > ");
}

/* Roda o programa com a interface de usuário */
void userInterface() {
    struct winsize terminal;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal);

    screenContent mainText;
    
    mainText.numberOfStrings = 9;
    mainText.strings = (char**) calloc(9, sizeof(char*));

    mainText.strings[0] = "BEM VINDO AO FAST ICMCDB";
    mainText.strings[1] = "Feito por Henrique Zanin (10441321) e Gabriel Marin (11218521)";
    mainText.strings[2] = "Comandos :";
    mainText.strings[3] = "1: Inserir";
    mainText.strings[4] = "2: Buscar";
    mainText.strings[5] = "3: Carregar tabela";
    mainText.strings[6] = "4: Entrar com arquivo de comandos";
    mainText.strings[7] = "5: Ver comandos disponiveis";
    mainText.strings[8] = "0: Sair do programa";
    
    printMainText(&terminal, &mainText);
    runtimeInterface(&terminal);
    
}

/* Roda o programa no modo de bd */
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
        free(query);
        switch (err){
        case EXIT:
            if(metadata){
                if(metadata->fpRegister)
                    fclose(metadata->fpRegister);
                if(metadata->fpIndex)
                    fclose(metadata->fpIndex);
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

void batchLoad(FILE *fp){
    Errors err;
    BOOL exitFlag = FALSE;
    char *query;
    Metadata *metadata = NULL;
    while (exitFlag == FALSE){
        query = readLine(fp);
        err = evalQuery(&metadata, query);
        free(query);
        switch (err){
        case EXIT:
            if(metadata){
                if(metadata->fpRegister)
                    fclose(metadata->fpRegister);
                if(metadata->fpIndex)
                    fclose(metadata->fpIndex);
                metadata->fpRegister = NULL;
                freeMetadata(metadata);
            }
            exitFlag = TRUE;
            break;
        default:
            raiseError(err);
            break;
        }
    }
}

/* Constroi o help na tela do usuario */
void buildHelp() {
    printf("***\n");
    printf("Available commands:\n");
    printf("insert, search, rrn, load, table, exit\n");
    printf("Type \"man *command*\" to see their manual\n");
    printf("***\n");
}

/* Commando man com seus programas registrados */
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

/* Printa a estrutura do manual com as informacoes passadas pelo parametro */
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

/* Printa uma linha vazia, só com as paredes na esquerda e direita do terminal */
void printEmptyLine(struct winsize *terminal) {
    int j;

    printf("|");
    for (j = 0; j < terminal->ws_col-2; j++) {
        printf(" ");
    }
    printf("|\n");
}

/* Printa os textos designados na tela, circulado por um quadrado responsivo com o terminal */
void printMainText(struct winsize *terminal, screenContent *content) {
    int k, stringSize, neededLines, i, pos, linesPrinted = 0;

    for (i = 0; i < terminal->ws_col; i++) printf("-");
    printf("\n");

    int halfSizeOfTerminal = terminal->ws_row/2;

    /* Printa uma parte do terminal antes dos textos */
    for (i = 0; i < halfSizeOfTerminal-4; i++) printEmptyLine(terminal);

    /* Printa os textos com os lados do terminal */
    for (k = 0; k < content->numberOfStrings; k++) {
        stringSize = strlen(content->strings[k]);
        neededLines = 1 + (stringSize/(terminal->ws_col - 4));
        pos = 0;

        for (i = 0; i < neededLines; i++) {
            printf("| ");
            for (i = 0; i < terminal->ws_col-4; i++, pos++) {
                if (pos < stringSize) printf("%c", content->strings[k][pos]);
                else printf(" ");
            }
            printf(" |\n");
            linesPrinted++;
        }
    }
    
    /* Printa o resto do terminal que sobrou */
    for (i = 0; i < (terminal->ws_row/2)-(linesPrinted-2); i++) printEmptyLine(terminal);
    for (i = 0; i < terminal->ws_col; i++) printf("-");

}

/* Runtime da interface de usuário */
void runtimeInterface(struct winsize *terminal) {
    int command = 1;
    screenContent message;
    message.strings = NULL;

    int forTestOnly;
    char test[4];

    while (command != 0) {
        scanf("%d", &command);
        switch (command) {
            case 1:
                printOnlyOneText(terminal, "Digite as informacoes necessarias");

                printf("\nDigite a chave desejada: ");
                scanf("%d", &forTestOnly);

                printf("\nDigite o nome: ");
                scanf("%d", &forTestOnly);

                printf("\nDigite a idade: ");
                scanf("%d", &forTestOnly);

                printf("\nDigite a nota: ");
                scanf("%d", &forTestOnly);
                printf("\n");
                

                message.numberOfStrings = 5;

                message.strings = (char**) calloc(5, sizeof(char*));
                message.strings[0] = "Registro inserido, digite outro comando";
                sprintf(test, "%d", forTestOnly);
                message.strings[1] = test;
                message.strings[2] = test;
                message.strings[3] = test;
                message.strings[4] = test;

                printMainText(terminal, &message);
                freeScreenContent(&message);
                break;
            case 2:
                printf("\nDigite a chave que deseja buscar: ");
                scanf("%d", &forTestOnly);
                printf("\n");

                printOnlyOneText(terminal, "Buscando...");

                message.numberOfStrings = 1;

                message.strings = (char**) calloc(1, sizeof(char*));
                message.strings[0] = "Busca completada, digite outro comando";

                printMainText(terminal, &message);
                freeScreenContent(&message);
                break;
            case 3:
                printOnlyOneText(terminal, "Carregando arquivo...");

                printf("\nDigite o nome do arquivo que deseja carregar: ");
                scanf("%d", &forTestOnly);
                printf("\n");

                printMainOneText(terminal, "Arquivo carregado");
                break;
            case 4:
                printMainOneText(terminal, " ");
                break;
            case 5:
                printOnlyOneText(terminal, "Digite o nome do arquivo com os comandos");
                scanf("%d", &forTestOnly);

                printMainOneText(terminal, "Todos os arquivos foram inseridos");
                break;
            case 0:
                printOnlyOneText(terminal, "Finalizando o programa...");



                printOnlyOneText(terminal, "Programa finalizado");
                break;
            default:
                printMainOneText(terminal, "Comando invalido");
                break;
        }
    }
    
}

void freeScreenContent(screenContent *objectToFree) {
    if (objectToFree->strings) {
        free(objectToFree->strings);
    }
}

void printMainOneText(struct winsize *terminal ,char *string) {
    screenContent message;

    message.numberOfStrings = 7;

    message.strings = (char**) calloc(7, sizeof(char*));
    message.strings[0] = string;
    message.strings[1] = "1: Inserir";
    message.strings[2] = "2: Buscar";
    message.strings[3] = "3: Carregar arquivo";
    message.strings[4] = "4: Entrar com arquivo de insercao";
    message.strings[5] = "5: Ver comandos disponiveis";
    message.strings[6] = "0: Sair do programa";

    printMainText(terminal, &message);
}

void printOnlyOneText(struct winsize *terminal ,char *string) {
    screenContent message;

    message.numberOfStrings = 1;

    message.strings = (char**) calloc(1, sizeof(char*));
    message.strings[0] = string;

    printMainText(terminal, &message);
}

