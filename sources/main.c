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
        /*SHOW AVALIABLE PARAMETERS*/
    }
    else{
      userInterface();
    }
    return 0;
}
