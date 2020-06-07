/*
Henrique Gomes Zanin NUSP: 10441321
Gabriel Guimaraes Vilas Boas Marin NUSP: 11218521
*/
#include <string.h>
#include <stdio.h>
#include <utils.h>
#include <menu.h>

int main(int argc, char *argv[]){
    if(argc > 1){
        if(!strcmp(argv[1], "--cmd")){
            commandLine();
        }
        else if(!strcmp(argv[1],"--batch")){
           FILE *fp = openFile(argv[2]);
           batchLoad(fp);
           fclose(fp);
        }
        else{
            puts("Try:");
            puts("--cmd -> enter into command line menu.");
            puts("--batch filename.in -> load file with commands.");
        }
    }
    else{
      userInterface();
    }
    return 0;
}
