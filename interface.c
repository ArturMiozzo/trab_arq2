#include "comandos.h"
#include "interface.h"
#include <stdio.h>
#include <stdlib.h>

//Funções de interface
void escreveCaminho(OBJETO caminho[255], int size){
    printf("%s",caminho[0].nome);

    for (int i = 1;i<size;i++){
       printf("/%s", caminho[i].nome);
    }
    if(size == 1){
        printf("/");
    }

    printf(">");
}

void boot(){
    char arg[100];
    OBJETO caminho[255];
    OBJETO root;
    int sizecaminho = 1;
    char *comando;
    char *args;
    char *arg1;

    for(int i = 0; i < 255; i++){
        caminho[i] = zeraObjeto();
    }

    root = criaInfoPasta("Root");
    caminho[0] = root;

    comando = "";
    while ((strcmp(comando, "SHUTDOWN") != 0) && (strcmp(args, "-S") != 0)) {
        escreveCaminho(caminho, sizecaminho);
        scanf("%[^\n]",arg);

        comando = strtok(arg, " ");
        args = strtok(NULL, " ");
        arg1 = strtok(NULL, "");

        if (args == NULL){
            args = "";
        }

        comando = strupr(comando);

        if (strcmp(comando, "MKFILE") == 0){
            if (!mkfile(args, caminho, sizecaminho-1)){
                printf("ERRO AO CRIAR ARQUIVO\n");
            }
        } else if (strcmp(comando, "MKDIR") == 0){
            if (!mkdir(args, caminho, sizecaminho-1)){
                printf("ERRO AO CRIAR PASTA\n");
            }
        }else if (strcmp(comando, "DIR") == 0){
            if (!DIR(caminho, sizecaminho-1)){
                printf("ERRO AO LER PASTA\n");
            }
        }else if (strcmp(comando, "MOVE") == 0){
            if (!MOVE(args, arg1, caminho, sizecaminho-1)){
                printf("ERRO MOVER ARQUIVO DA PASTA\n");
            }
        }else if (strcmp(comando, "RENAME") == 0){
            if (!RENAME(args, arg1, caminho, sizecaminho-1)){
                printf("ERRO AO RENOMEAR ARQUIVO/DIRETORIO\n");
            }
        }else if (strcmp(comando, "EDIT") == 0){
            if (!edit(args, arg1, caminho, sizecaminho-1)){
                printf("ERRO AO EDITAR ARQUIVO\n");
            }
        }else if (strcmp(comando, "RM") == 0){
            if (!remove_(args, 0, caminho, sizecaminho-1)){
                printf("ERRO AO REMOVER ARQUIVO/DIRETORIO\n");
            }
        }else if (strcmp(comando, "RF") == 0){
            if (!remove_(args, 1, caminho, sizecaminho-1)){
                printf("ERRO AO REMOVER ARQUIVO/DIRETORIO E SEUS FILHOS\n");
            }
        }else if (strcmp(comando, "DISK") == 0){
            if (!disk(args, caminho, sizecaminho-1)){
                printf("ERRO AO CALCULAR ESPACO\n");
            }
        }else if (strcmp(comando, "DEFRAG") == 0){
            if (!defrag(args, caminho, sizecaminho-1)){
                printf("DEFRAG NAO REALIZADO\n");
            }
        }else if (strcmp(comando, "CD") == 0){
            arg1 = strtok(args,"/");
            while (arg1 != NULL ){
                if (strcmp(arg1,"..") == 0){
                    if (sizecaminho > 1){
                        caminho[sizecaminho-1] = zeraObjeto();
                        sizecaminho--;
                    }
                }else{
                    int aux = CD(arg1, caminho, sizecaminho-1);
                    if(aux>=0)
                    {
                        if (strcmp(arg1,"Root")==0){
                            for (int i = sizecaminho-1;i>0;i--){
                                caminho[i] = zeraObjeto();
                            }
                            sizecaminho = 1;
                        }else{
                            sizecaminho++;
                            caminho[sizecaminho-1] = retornaObjetoDaPasta(arg1, caminho[sizecaminho-2].cluster_inicial);
                        }
                    }
                    else{
                       printf("ERRO AO ACESSAR PASTA\n");
                    }
                }
                arg1 = strtok(NULL,"/");
            }
        }else if ((strcmp(comando, "SHUTDOWN") != 0) && (strcmp(args, "-S") != 0)){
            printf("COMANDO INVALIDO\n");
        }
        args = strupr(args);
        scanf("%*[^\n]"); scanf("%*c");
    }
}

