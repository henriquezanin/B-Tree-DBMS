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

typedef struct manInformations manualInformations;

void userInterface();
void commandLine();
void printCommandLineMenu();
void buildHelp();
void runManuals(char*);
void printMan(manualInformations*, struct winsize*);
void printEmptyLine(struct winsize*);
void printText(struct winsize*, char*);

#endif