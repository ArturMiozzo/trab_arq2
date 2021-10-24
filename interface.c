#include "comandos.h"
#include "interface.h"
#include <stdio.h>

//Funções de interface
void boot(){
    char comando[7] = "";
    char arg[100];
    char *args;
    char *arg1;
    int curFolder = 0;

    while ((strcmp(comando, "shutdown") != 0) && (strcmp(args, "-s") != 0)) {

        printf("C:\\>");
        scanf("%s %[^\n]",comando,arg);

        args = strtok(arg, " ");
        arg1 = strtok(NULL, " ");

        if (strcmp(comando, "MKFILE") == 0){
            if (!mkfile(args, curFolder)){
                printf("ERRO AO CRIAR ARQUIVO\n");
            }
        } else if (strcmp(comando, "MKDIR") == 0){
            if (!mkdir(args, curFolder)){
                printf("ERRO AO CRIAR PASTA\n");
            }
        }else if (strcmp(comando, "DIR") == 0){
            if (!DIR(args, curFolder)){
                printf("ERRO AO LER PASTA\n");
            }
        }else if (strcmp(comando, "MOVE") == 0){
            if (!MOVE(args, curFolder)){
                printf("ERRO MOVER ARQUIVO DA PASTA\n");
            }
        }else if (strcmp(comando, "RENAME") == 0){
            if (!RENAME(args, curFolder)){
                printf("ERRO MOVER ARQUIVO DA PASTA\n");
            }
        }else if ((strcmp(comando, "shutdown") != 0) && (strcmp(args, "-s") != 0)){
            printf("COMANDO INVALIDO\n");
        }
    }
}
