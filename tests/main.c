/*
Henrique Gomes Zanin NUSP: 10441321
Gabriel Guimaraes Vilas Boas Marin NUSP: 11218521
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <utils.h>

TextDocument *createTests(TextDocument *, int);

int main(int argc, char *argv[]){

    if(argc < 2){
        puts("Usage: ./main int\nexample: ./main 10");
        return 1;
    }
    FILE *examples = openFile("names.csv");
    assertFile(examples, "names.csv");
    TextDocument *doc = readAll(examples);
    fclose(examples);
    TextDocument *tests = createTests(doc,atoi(argv[1]));
    printTextDocument(tests); 

    return 0;
}

TextDocument *createTests(TextDocument *examples, int size){
    int i;
    char buffer[12];
    TextDocument *tests = (TextDocument*)calloc(1,sizeof(TextDocument));
    tests->nLines++;
    tests->text = (char**)malloc(sizeof(char*));
    tests->text[0] = createStringOnHeap("load alunos.table");
    for(i = 0;i < size; i++){
        srand(i*13);
        tests->text = (char**)realloc(tests->text,(tests->nLines+1)*sizeof(char*));
        tests->text[tests->nLines] = (char*)calloc(100,sizeof(char));
        strcpy(tests->text[tests->nLines], "insert ");
        sprintf(buffer,"%d",(i+1));
        strcat(tests->text[tests->nLines], buffer);
        strcat(tests->text[tests->nLines], ",");
        strcat(tests->text[tests->nLines], "\"");
        strcat(tests->text[tests->nLines], examples->text[rand()%examples->nLines]);
        strcat(tests->text[tests->nLines], "\"");
        strcat(tests->text[tests->nLines], ",");
        srand(time(NULL)+i);
        strcat(tests->text[tests->nLines], "\"");
        strcat(tests->text[tests->nLines], examples->text[rand()%examples->nLines]);
        strcat(tests->text[tests->nLines], "\"");
        strcat(tests->text[tests->nLines], ",");
        strcat(tests->text[tests->nLines], "\"");
        strcat(tests->text[tests->nLines], "Sistemas de Informacao");
        strcat(tests->text[tests->nLines], "\"");
        strcat(tests->text[tests->nLines], ",");
        sprintf(buffer,"%.2f",((rand()%100)+1)/10.0);
        strcat(tests->text[tests->nLines], buffer);
        tests->nLines++;
    }
    tests->text = (char**)realloc(tests->text,(tests->nLines)*sizeof(char*));
    tests->text[tests->nLines++] = createStringOnHeap("exit");
    return tests;
}