#include "comandos.h"
#include "interface.h"
#include <stdio.h>
#include <stdlib.h>

//Funções de interface
void boot(){
    char arg[100];
    char strcaminho[MAX_FOLDER][MAX_FOLDER];
    int  ncaminho[MAX_FOLDER];
    int sizecaminho = 1;
    char *comando;
    char *args;
    char *arg1;
    int curFolder = 0;

    strcpy(strcaminho[0],"C:");
    ncaminho[0] = 0;
    while ((strcmp(comando, "SHUTDOWN") != 0) && (strcmp(args, "-S") != 0)) {
        escreveCaminho(strcaminho, sizecaminho);
        scanf("%[^\n]",arg);

        comando = strtok(arg, " ");
        args = strtok(NULL, " ");
        arg1 = strtok(NULL, "");

        if (args == NULL){
            args = "";
        }

        comando = strupr(comando);

        if (strcmp(comando, "MKFILE") == 0){
            if (!mkfile(args, curFolder)){
                printf("ERRO AO CRIAR ARQUIVO\n");
            }
        } else if (strcmp(comando, "MKDIR") == 0){
            if (!mkdir(args, curFolder)){
                printf("ERRO AO CRIAR PASTA\n");
            }
        }else if (strcmp(comando, "DIR") == 0){
            if (!DIR(curFolder)){
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
        }else if (strcmp(comando, "EDIT") == 0){
            if (!edit(args, arg1, curFolder)){
                printf("ERRO AO EDITAR ARQUIVO\n");
            }
        }else if (strcmp(comando, "CD") == 0){
            if (strcmp(args,"..") == 0){
                if (sizecaminho > 1){
                    strcpy(strcaminho[sizecaminho-1],"");
                    ncaminho[sizecaminho-1] = -1;
                    sizecaminho--;
                    curFolder = ncaminho[sizecaminho-1];
                }
            }else{
                int aux = CD(args, curFolder);
                if(aux>=0)
                {
                    sizecaminho++;
                    strcpy(strcaminho[sizecaminho-1],args);
                    ncaminho[sizecaminho-1] = aux;
                    curFolder = aux;
                }
            }
        }else if ((strcmp(comando, "SHUTDOWN") != 0) && (strcmp(args, "-S") != 0)){
            printf("COMANDO INVALIDO\n");
        }
        args = strupr(args);
        scanf("%*[^\n]"); scanf("%*c");
    }
}

void escreveCaminho(char caminho[MAX_FOLDER][MAX_FOLDER], int size){
    printf("%s",caminho[0]);

    for (int i = 1;i<size;i++){
       printf("\\%s", caminho[i]);
    }
    if(size == 1){
        printf("\\");
    }

    printf(">");
}
