/*
Henrique Gomes Zanin NUSP: 10441321
Gabriel Guimaraes Vilas Boas Marin NUSP: 11218521
*/
#ifndef __BTREEMENU__
#define __BTREEMENU__

#include <sys/ioctl.h>

struct manInformations {
    char *name;
    char *synopsis;
    char *description;
    char *exampleHeader;
    char *exampleText;
    char *history;
};

struct screenText {
    char **strings;
    int numberOfStrings;
};

struct userInput {
    char *name;
    char *nusp;
    char *lastName;
    char *course;
    char *grade;
};

typedef struct userInput userInput;

typedef struct screenText screenContent;

typedef struct manInformations manualInformations;

void userInterface();
void commandLine();
void batchLoad(FILE *);
void printCommandLineMenu();
void buildHelp();
char* insertFromUI(struct winsize*, screenContent*);
void runManuals(char*);
void printMan(manualInformations*, struct winsize*);
void printEmptyLine(struct winsize*);
void printMainText(struct winsize*, screenContent*);
void runtimeInterface(struct winsize*);
void freeScreenContent(screenContent*);
void printMainOneText(struct winsize*, char*);
void printOnlyOneText(struct winsize*, char*);
void printRegister(userInput*);
userInput* readUserInput();
char* formatStringToBtreePattern(userInput*);
void freeRegister(userInput*);
char* twoStringConcat(char*, char*);


#endif