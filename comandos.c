# include "comandos.h"
# include "arquivos.h"

int DIR(int folder)
{
    imprimePasta(folder);
    return 1;
}

int mkfile(char *name, int folder){
    OBJETO obj;
    obj = zeraObjeto(obj);
    char* auxString;
    char originalName[100];
    int index;

    strcpy(originalName, name);
    index = strrchr(name,'.')-name;

    if (index < 0){
        return 0;
    }

    strncpy(obj.nome, name, index);
    auxString = obj.nome;
    strcat(auxString, ".txt");

    if (strcmp(auxString,originalName) != 0){
        return 0;
    }

    strcpy(obj.extensao, ".txt");
    char dados[] = "";
    salvaArquivo(obj, dados, folder);
    return 1;
}

int mkdir(char *name, int folder){
    OBJETO obj;
    obj = zeraObjeto(obj);
    char originalName[100];

    strcpy(originalName, name);
    strcpy(obj.nome,strtok(name, "."));

    if (strcmp(name,originalName) != 0){
        return 0;
    }

    strcpy(obj.extensao, "");
    char dados[] = "";
    salvaArquivo(obj, dados, folder);
    return 1;
}

int MOVE(char *args, int folder)
{

}

int RENAME(char *args, int folder)
{

}
