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

typedef struct screenText screenContent;

typedef struct manInformations manualInformations;

void userInterface();
void commandLine();
void printCommandLineMenu();
void buildHelp();
void runManuals(char*);
void printMan(manualInformations*, struct winsize*);
void printEmptyLine(struct winsize*);
void printMainText(struct winsize*, screenContent*);
void runtimeInterface(struct winsize*);
void freeScreenContent(screenContent*);
void printMainOneText(struct winsize*, char*);
void printOnlyOneText(struct winsize*, char*);

#endif