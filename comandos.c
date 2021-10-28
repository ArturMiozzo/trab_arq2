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

int MOVE(char *file, char *path, int folder)
{
    char dirpath[100];
    char fileName[100];
    int folderIn;
    OBJETO objMove, tmpObj;

    int folderOut = retornaClusterCaminho(path, folder);
    if(folderOut<0) return 0;

    char *ptr_aux = strrchr(file,'/');

    int index = ptr_aux-file;

    if(index>0)
    {
        strncpy(dirpath, file, index);
        strcpy(fileName, ptr_aux);
        folderIn = retornaClusterCaminho(dirpath);
        if(folderIn<0) return 0;
    }
    else
    {
        strcpy(fileName, file);
        folderIn = folder;
    }

    tmpObj = retornaObjetoDaPasta(fileName, folderOut);
    if(tmpObj.tamanho>0) return 0;

    objMove = retornaObjetoDaPasta(fileName, folderIn);
    if(objMove.tamanho==0) return 0;

    tmpObj = zeraObjeto();

    tmpObj.cluster_inicial = objMove.cluster_inicial;
    tmpObj.tamanho = objMove.tamanho;

    strcpy(tmpObj.extensao, objMove.extensao);
    strcpy(tmpObj.nome, objMove.nome);

    removeObjetoDaPasta(fileName, folderIn);

    salvaObjetoNaPasta(tmpObj, folderOut);

    return 1;
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
    int strsize = strlen(args);
    if ((strsize > 3) && (args[strsize-4] == '.')){
        return -1;
    }
    int aux = retornaClusterCaminho(args, folder);
    if(aux>=0)
    {
        return aux;
    }
    return -1;
}
