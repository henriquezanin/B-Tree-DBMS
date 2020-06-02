#include <string.h>
#include <menu.h>

int main(int argc, char *argv[]){
    if(argc > 1){
        if(!strcmp(argv[1], "--cmd")){
            commandLine();
        }
    }
    return 0;
}