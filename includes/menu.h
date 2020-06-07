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
void runManuals(char*);
void printMan(manualInformations*, struct winsize*);
void printEmptyLine(struct winsize*);
void printMainText(struct winsize*, screenContent*);
void runtimeInterface(struct winsize*);
void insertFromUI(struct winsize*, char*);
void freeScreenContent(screenContent*);
void printMainOneText(struct winsize*, char*);
void printOnlyOneText(struct winsize*, char*);
void printRegister(userInput*);
userInput* readUserInput();
char* formatStringToBtreePattern(userInput*);
void freeAll();
void freeRegister(userInput*);


#endif