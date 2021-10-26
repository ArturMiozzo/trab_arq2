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

int edit(char *name, char* newdata,int folder){
    char* truedata;
    OBJETO obj;

    if((newdata == NULL) || (newdata[0] != '"') || (newdata[strlen(newdata)-1] != '"') || (strlen(newdata) < 2)){
        return 0;
    }

    truedata = strtok(newdata,"\"");
    if (truedata == NULL){
        truedata = "";
    }

    obj = retornaObjetoDaPasta(name, folder);

    if (strcmp(obj.nome, "") == 0){
        return 0;
    }

    modificaArquivo(obj, truedata, folder);
    obj = retornaObjetoDaPasta(name, folder);
    return 1;
}

int CD(char *args, int folder)
{
    int aux = retornaClusterCaminho(args, folder);
    if(aux>=0)
    {
        return aux;
    }
    return -1;
}
