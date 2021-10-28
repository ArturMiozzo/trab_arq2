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

int RENAME(char *oldname, char *newname, int folder)
{
    OBJETO objAux;
    char strPath[100];
    char strName[100];
    char *strAux;
    int sizeStr;
    int nFolder;

    strcpy(strPath,oldname);
    strcpy(strAux, strtok(oldname,"/"));
    strcpy(strName, strAux);

    if (strcmp(strAux, strPath)!=0){
        while (strAux != NULL){
            strcpy(strName, strAux);
            strAux = strtok(NULL,"/");
        }
        strPath[strlen(strPath)-strlen(strName)-1] = 0;
        nFolder = retornaClusterCaminho(strPath, folder);
    }else{
        strcpy(strPath, "");
        nFolder = folder;
    }

    //faz as valida��es dos nomes
    if (strstr(strName, ".txt") == NULL){
        //se n�o tem .txt, pelas valida��es na hora de criar um obj, � obrigatoriamente uma pasta
        //sendo uma pasta, retorna 0 se h� uma extens�o no novo nome
        if (strrchr(newname,'.')-newname > -1){
            return 0;
        }
    }else{
        //possuindo .txt, pelas valida��es na hora de criar um obj, este est� no final e o objeto � um arquivo
        //assim, se o novo nome n�o termina com .txt, retorna 0
        sizeStr = strlen(newname);
        if ((sizeStr < 4) || (newname[sizeStr-4] != '.') || (newname[sizeStr-3] != 't') || (newname[sizeStr-2] != 'x') ||
            (newname[sizeStr-1] != 't')){
            return 0;
        }
    }

    objAux = retornaObjetoDaPasta(strName, nFolder);
    if (strcmp(objAux.nome,"") == 0){
        return 0;
    }

    removeObjetoDaPasta(strName, nFolder);

    strcpy(objAux.nome, newname);
    salvaObjetoNaPasta(objAux, nFolder);

    return 1;
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
