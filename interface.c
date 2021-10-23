#include "comandos.h"
#include "interface.h"

//Funções de interface
void boot(){
    char comando[7] = "";
    char arg[100];
    char *arg1;
    char *arg2;
    int curFolder = 0;

    while ((strcmp(comando, "shutdown") != 0) && (strcmp(arg1, "-s") != 0)) {
        printf("C:\\>");
        scanf("%s %[^\n]",comando,arg);

        arg1 = strtok(arg, " ");
        arg2 = strtok(NULL, " ");

        if (strcmp(comando, "MKFILE") == 0){
            if (!mkfile(arg1, curFolder)){
                printf("ERRO AO CRIAR ARQUIVO\n");
            }
        } else if (strcmp(comando, "MKDIR") == 0){
            if (!mkdir(arg1, curFolder)){
                printf("ERRO AO CRIAR PASTA\n");
            }
        }else if ((strcmp(comando, "shutdown") != 0) && (strcmp(arg1, "-s") != 0)){
            printf("COMANDO INVALIDO\n");
        }
    }
}
