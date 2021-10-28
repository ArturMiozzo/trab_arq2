#include "comandos.h"

int DIR(OBJETO caminho[255], int cur)
{
    imprimePasta(caminho[cur].cluster_inicial);
    return 1;
}

int mkfile(char *name, OBJETO caminho[255], int cur){
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
    if (salvaArquivo(obj, dados, caminho[cur].cluster_inicial)){
        atualizaTamanhoDasPastas(caminho, cur+1);
        return 1;
    }
    return 0;
}

int mkdir(char *name, OBJETO caminho[255], int cur){
    OBJETO obj;
    obj = zeraObjeto(obj);
    char originalName[100];

    strcpy(originalName, name);
    strcpy(obj.nome,strtok(name, "."));

    if (strcmp(name,originalName) != 0){
        return 0;
    }

    strcpy(obj.extensao, "");

    if (salvaPasta(obj, caminho[cur].cluster_inicial)){
        atualizaTamanhoDasPastas(caminho, cur+1);
        return 1;
    }
    return 0;
}

int MOVE(char *file, char *path, OBJETO caminho[255], int cur)
{
    char dirpath[100];
    char fileName[100];
    int folderIn;
    OBJETO objMove, tmpObj;

    int folderOut = retornaClusterCaminho(path, caminho[cur].cluster_inicial);
    if(folderOut<0) return 0;

    char *ptr_aux = strrchr(file,'/');

    int index = ptr_aux-file;

    if(index>0)
    {
        strncpy(dirpath, file, index);
        strcpy(fileName, ptr_aux);
        folderIn = retornaClusterCaminho(dirpath, caminho[cur].cluster_inicial);
        if(folderIn<0) return 0;
    }
    else
    {
        strcpy(fileName, file);
        folderIn = caminho[cur].cluster_inicial;
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

int RENAME(char *oldname, char *newname, OBJETO caminho[255], int cur)
{
    OBJETO objAux;
    char strPath[100];
    char strName[100];
    char *strAux = malloc(sizeof(*strAux));
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
        nFolder = retornaClusterCaminho(strPath, caminho[cur].cluster_inicial);
    }else{
        strcpy(strPath, "");
        nFolder = caminho[cur].cluster_inicial;
    }

    //faz as validações dos nomes
    if (strstr(strName, ".txt") == NULL){
        //se não tem .txt, pelas validações na hora de criar um obj, é obrigatoriamente uma pasta
        //sendo uma pasta, retorna 0 se há uma extensão no novo nome
        if (strrchr(newname,'.')-newname > -1){
            return 0;
        }
    }else{
        //possuindo .txt, pelas validações na hora de criar um obj, este está no final e o objeto é um arquivo
        //assim, se o novo nome não termina com .txt, retorna 0
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

int edit(char *name, char* newdata, OBJETO caminho[255], int cur){
    char *strAux = malloc(sizeof(*strAux));
    char *originalName = name;
    char strName[100];
    char strPath[100];
    int nFolder;
    OBJETO obj;

    if((newdata == NULL) || (newdata[0] != '"') || (newdata[strlen(newdata)-1] != '"') || (strlen(newdata) < 2)){
        return 0;
    }

    strcpy(strPath,name);
    strcpy(strAux, strtok(originalName,"/"));

    while (strAux != NULL){
        strcpy(strName, strAux);
        strAux = strtok(NULL,"/");
    }
    strPath[strlen(strPath)-strlen(strName)-1] = 0;

    if (strcmp(strName,strPath)==0){
        strcpy(strPath,"");
    }

    if (newdata == NULL){
        return 0;
    }

    if (strcmp(strPath,"")==0){
        nFolder = caminho[cur].cluster_inicial;
        obj = retornaObjetoDaPasta(strName, caminho[cur].cluster_inicial);
    }else{
        nFolder = retornaClusterCaminho(strPath,caminho[cur].cluster_inicial);
        obj = retornaObjetoDaPasta(strName, nFolder);
    }

    if (strcmp(obj.nome, "") == 0){
        return 0;
    }

    if (modificaArquivo(obj, newdata, nFolder)){
        atualizaTamanhoDasPastas(caminho, cur+1);
        return 1;
    }
    return 0;
}

int CD(char *args, OBJETO caminho[255], int cur)
{
    int strsize = strlen(args);
    if ((strsize > 3) && (args[strsize-4] == '.')){
        return -1;
    }
    int aux = retornaClusterCaminho(args, caminho[cur].cluster_inicial);
    if(aux>=0)
    {
        return aux;
    }
    return -1;
}

int remove_(char *path, int force, OBJETO caminho[255], int cur){
    OBJETO objAux;
    char strPath[100];
    char strName[100];
    char *strAux = malloc(sizeof(*strAux));
    int nFolder;

    strcpy(strPath,path);
    strcpy(strAux, strtok(path,"/"));
    strcpy(strName, strAux);

    //separa caminho do objeto final (pasta ou arquivo a ser excluido)
    if (strcmp(strAux, strPath)!=0){
        while (strAux != NULL){
            strcpy(strName, strAux);
            strAux = strtok(NULL,"/");
        }
        strPath[strlen(strPath)-strlen(strName)-1] = 0;
        nFolder = retornaClusterCaminho(strPath, caminho[cur].cluster_inicial);
    }else{
        strcpy(strPath, "");
        nFolder = caminho[cur].cluster_inicial;
    }

    // Verifica se o objeto existe
    objAux = retornaObjetoDaPasta(strName, nFolder);
    if (strcmp(objAux.nome,"") == 0){
        return 0;
    }

    //se é um arquivo, remove ele da pasta
    if (strstr(strName, ".txt") != NULL){
        apagaArquivo(objAux,nFolder);
        return 1;
    }else{
        //se for uma pasta, apaga a depender se é vazia e se o comando é o rm ou o rf
        if (force){
            return apagaPastaCheia(strName,nFolder);
        }else{
            return apagaPastaVazia(strName,nFolder);
        }
    }
}
