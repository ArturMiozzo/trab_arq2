#include "header.h"

//Fun��es que manipulam os arquivos

METADADOS zeraMetadados(METADADOS metadados){

    metadados.ini_indice = 0;
    metadados.ini_root = 0;
    metadados.tam_cluster = 0;
    metadados.tam_indice = 0;

    return metadados;
}

INDICE zeraIndice(INDICE indice){

    int i;
    for(i = 0; i < sizeof(INDICE); i++){
        indice.indice[i] = 0;
    }
    return indice;
}

CLUSTER zeraCluster(CLUSTER cluster){

    int i;
    for(i = 0; i < sizeof(CLUSTER); i++){
        cluster.dados[i] = 0;
    }

    return cluster;
}

OBJETO zeraObjeto(OBJETO objeto){

    int i;
    for(i = 0; i < 54; i++){
        objeto.nome[i] = 0;
    }
    for(i = 0; i < 5; i++){
        objeto.extensao[i] = 0;
    }
    objeto.cluster_inicial = 0;
    objeto.tamanho = 0;

    return objeto;
}

void criaArquivoDados(){
    //Cria o arquivo do sistema

    FILE* light_fs;
    int i;

    METADADOS meta;
    meta = zeraMetadados(meta);
    meta.tam_indice = 256;
    meta.tam_cluster = 32768;
    meta.ini_indice = 8;
    meta.ini_root = 264;

    INDICE ind;
    ind = zeraIndice(ind);

    CLUSTER clust;
    clust = zeraCluster(clust);

    light_fs = fopen(FILENAME, "wb");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        rewind(light_fs);

        fwrite(&meta, sizeof(METADADOS), 1, light_fs);
        fwrite(&ind, sizeof(INDICE), 1, light_fs);

        for(i = 0; i < sizeof(INDICE); i++){
            fwrite(&clust, sizeof(CLUSTER), 1, light_fs);
        }
        fflush(light_fs);
    }
    fclose(light_fs);
}

CLUSTER leCluster(uint8_t ind_cluster){
    //Retorna o cluster do �ndice informado

    FILE* light_fs;
    CLUSTER clust;

    light_fs = fopen(FILENAME, "rb");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        rewind(light_fs);
        fseek(light_fs, sizeof(METADADOS), SEEK_SET);
        fseek(light_fs, sizeof(INDICE), SEEK_CUR);
        fseek(light_fs, ind_cluster * sizeof(CLUSTER), SEEK_CUR);

        fread(&clust, sizeof(CLUSTER), 1, light_fs);

        fflush(light_fs);
    }
    fclose(light_fs);

    return clust;
}

void escreveCluster(CLUSTER cluster, uint8_t ind_cluster){
    //Recebe um cluster como par�metro e escreve ele dentro do arquivo do sistema

    FILE* light_fs;

    light_fs = fopen(FILENAME, "rb+");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        rewind(light_fs);
        fseek(light_fs, sizeof(METADADOS), SEEK_SET);
        fseek(light_fs, sizeof(INDICE), SEEK_CUR);
        fseek(light_fs, ind_cluster * sizeof(CLUSTER), SEEK_CUR);

        fwrite(&cluster, sizeof(CLUSTER), 1, light_fs);

        fflush(light_fs);
    }
    fclose(light_fs);
}

void posicionaCursorNoIndice(uint8_t indice, FILE* light_fs){
    //Coloca o curso do arquivo no indice informado

    rewind(light_fs);
    fseek(light_fs, sizeof(METADADOS), SEEK_SET);
    fseek(light_fs, indice * sizeof(uint8_t), SEEK_CUR);
}

uint8_t leIndice(uint8_t indice){
    //Retorna o valor salvo no indice informado

    FILE* light_fs;
    uint8_t ind;

    light_fs = fopen(FILENAME, "rb");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        posicionaCursorNoIndice(indice, light_fs);

        fread(&ind, sizeof(ind), 1, light_fs);

        fflush(light_fs);
    }
    fclose(light_fs);

    return ind;
}

void escreveIndice(uint8_t valor, uint8_t indice){
    //Escreve o valor no indice informado

    FILE* light_fs;

    light_fs = fopen(FILENAME, "rb+");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        posicionaCursorNoIndice(indice, light_fs);

        fwrite(&valor, sizeof(valor), 1, light_fs);

        fflush(light_fs);
    }
    fclose(light_fs);
}

void posicionaCursorNoCluster(uint8_t ind_cluster, FILE* light_fs){
    //Coloca o curso do arquivo no cluster informado

    rewind(light_fs);
    fseek(light_fs, sizeof(METADADOS), SEEK_SET);               //Salta os metadados
    fseek(light_fs, sizeof(INDICE), SEEK_CUR);                  //Salta o �ndice
    fseek(light_fs, ind_cluster * sizeof(CLUSTER), SEEK_CUR);   //Chega no cluster indicado
}

bool checaNomeRepetido(char nome[], uint8_t ind_cluster, FILE* light_fs){
    //Busca dentro da pasta se j� existe algum item com o nome informado

    char buffer[54];
    int contador = sizeof(CLUSTER) / sizeof(OBJETO);

    posicionaCursorNoCluster(ind_cluster, light_fs);

    while(contador > 0){
        fread(&buffer, 54 * sizeof(char), 1, light_fs);

        if(strcmp(buffer, nome) != 0){              //Compara o nome lido com o nome do objeto a ser escrito
            fseek(light_fs, -54*sizeof(char), SEEK_CUR);
            fseek(light_fs, sizeof(OBJETO), SEEK_CUR);
        }
        else{
            return true;
        }

        contador--;
    }

    return false;   //Se chegar at� aqui � porque n�o encontrou um nome repetido
}

int salvaObjetoNaPasta(OBJETO objeto, uint8_t ind_cluster){
    //Retorna 1 se tudo est� OK
    //Retorna 0 se j� existe um objeto de mesmo nome

    FILE* light_fs;
    char buffer;
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero m�ximo de objetos que podem existir em uma pasta

    light_fs = fopen(FILENAME, "rb+");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        //Checa se j� n�o existe um objeto de mesmo nome
        if(checaNomeRepetido(objeto.nome, ind_cluster, light_fs)){
            printf("O item [%s] ja existe!\n", objeto.nome);
            return 0;
        }

        posicionaCursorNoCluster(ind_cluster, light_fs);

        //Busca a primeira �rea livre
        while(contador > 0){
            fread(&buffer, sizeof(char), 1, light_fs);

            if(buffer != 0){
                fseek(light_fs, -1*sizeof(char), SEEK_CUR);
                fseek(light_fs, sizeof(OBJETO), SEEK_CUR);
            }
            else{
                break;
            }

            contador--;
        }
        fseek(light_fs, -1*sizeof(char), SEEK_CUR);

        if(contador > 0){
            fwrite(&objeto, sizeof(OBJETO), 1, light_fs);           //Salva o objeto nessa posi��o livre
        }
        else{
            printf("N�o h� espa�o dispon�vel na pasta!\n");
        }


        fflush(light_fs);
    }
    fclose(light_fs);

    return 1;
}

void removeObjetoDaPasta(char nome_objeto[], uint8_t ind_cluster){
    //Apaga um item da pasta sobreescrevendo ele com um objeto vazio
    //Vers�o n�o segura. N�o checa se � uma pasta vazia ou n�o.

    FILE* light_fs;
    OBJETO obj;
    char buffer[64];
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero m�ximo de objetos que podem existir em uma pasta

    obj = zeraObjeto(obj);

    light_fs = fopen(FILENAME, "rb+");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        posicionaCursorNoCluster(ind_cluster, light_fs);

        while(contador > 0){
            fread(&buffer, 54 * sizeof(char), 1, light_fs);
            printf("Buffer: %s - Nome: %s\n", buffer, nome_objeto);

            if(strcmp(buffer, nome_objeto) == 0){                        //Se encontrar um objeto com nome igual ao informado, apaga ele
                fseek(light_fs, -54 * sizeof(char), SEEK_CUR);
                fwrite(&obj, sizeof(OBJETO), 1, light_fs);
                break;
            }
            else{                                                   //Se n�o, continua a busca
                fseek(light_fs, -54 * sizeof(char), SEEK_CUR);
                fseek(light_fs, sizeof(OBJETO), SEEK_CUR);
            }
            contador--;

        }

        if(contador == 0){
            printf("A pasta nao contem nenhum item [%s]!\n", nome_objeto);
        }

        fflush(light_fs);
    }
    fclose(light_fs);
}


void imprimePasta(uint8_t ind_cluster){

    FILE* light_fs;
    OBJETO obj;
    int contador = sizeof(CLUSTER) / sizeof(OBJETO);

    light_fs = fopen(FILENAME, "rb+");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        posicionaCursorNoCluster(ind_cluster, light_fs);

        printf("N   - Nome                             - Extensao - Cluster ini - Tamanho\n", obj.nome, obj.extensao, obj.cluster_inicial, obj.tamanho);

        while(contador > 0){
            fread(&obj.nome, sizeof(obj.nome), 1, light_fs);
            fread(&obj.extensao, sizeof(obj.extensao), 1, light_fs);
            fread(&obj.cluster_inicial, sizeof(obj.cluster_inicial), 1, light_fs);
            fread(&obj.tamanho, sizeof(obj.tamanho), 1, light_fs);

            if(obj.nome[0] == 0){
                if(contador == sizeof(CLUSTER) / sizeof(OBJETO)){
                    printf("      <Vazio>\n");
                }
                break;
            }
            else{
                printf("%-3d - ", 512 - contador);
                printf("%-32s - ", obj.nome);
                printf("%-5s    - ", obj.extensao);
                printf("%-3d         - ", obj.cluster_inicial);
                printf("%-5d  ", obj.tamanho);
                printf("\n");
            }

            contador--;
        }

        fflush(light_fs);
    }
    fclose(light_fs);
}

int buscaClusterLivre(){

    int indice;
    for(indice = 1; indice < sizeof(INDICE); indice++){
        if(leIndice(indice) == 0){
            break;
        }
    }

    if(indice == sizeof(INDICE)){   //N�o encontrou cluster livre
        return -1;
    }
    else{
        return indice;              //Encontrou cluster livre
    }
}

int buscaNumeroClustersLivres(){

    int i;
    int total = 0;
    for(i = 1; i < sizeof(INDICE); i++){
        if(leIndice(i) == 0){
            total++;
        }
    }

    return total;
}

int salvaArquivo(OBJETO* ptr_objeto, char dados[], uint8_t ind_cluster_pasta){
    //Return 0 se acontecer algum problema
    //Return 1 se ocorrer tudo certo

    int indice_atual;
    int indice_seg;
    int i;
    int contador = 0;
    int num_clusters;
    CLUSTER clust;

    clust = zeraCluster(clust);

    num_clusters = ((strlen(dados) + 1) / sizeof(CLUSTER)) + 1;

    if(buscaNumeroClustersLivres() < num_clusters){             //Checa se s�o necess�rios mais clusters do que se tem dispon�veis
        printf("Nao existem clusters livres suficientes!");
        return 0;
    }

    //Busca um cluster vazio para guardar os dados
    indice_atual = buscaClusterLivre();

    //Termina de preencher as informa��es do objeto e salva na pasta
    ptr_objeto->cluster_inicial = indice_atual;
    ptr_objeto->tamanho = strlen(dados) + 1;
    if(salvaObjetoNaPasta(*ptr_objeto, ind_cluster_pasta) == 0){    //Se o objeto j� existe, a fun��o � encerrada
        return 0;
    }


    //Come�a a etapa de armazenar os dados
    if(ptr_objeto->tamanho <= sizeof(CLUSTER)){                 //Os dados cabem em um �nico cluster

        strcpy(clust.dados, dados);
        escreveCluster(clust, indice_atual);
        escreveIndice(indice_atual, indice_atual);
    }
    else{                                                       //Os dados precisam de mais de um cluster

        for(i = 0; i < strlen(dados) + 1; i++){
            if(contador < sizeof(CLUSTER)){
                clust.dados[contador] = dados[i];
                contador++;
            }
            else{                                               //Cluster completamente preenchido

                contador = 0;
                escreveCluster(clust, indice_atual);
                escreveIndice(indice_atual, indice_atual);
                indice_seg = buscaClusterLivre();
                escreveIndice(indice_seg, indice_atual);
                indice_atual = indice_seg;
                clust = zeraCluster(clust);
            }
        }

        escreveCluster(clust, indice_atual);                    //�ltimo cluster parcialmente preenchido
        escreveIndice(indice_atual, indice_atual);
    }

    return 1;
}


int salvaPasta(OBJETO* ptr_objeto, uint8_t ind_cluster_pasta){

    int indice;

    //Busca um cluster vazio para guardar os dados
    indice = buscaClusterLivre();
    if(indice == -1){                     //Se n�o houver cluster dispon�vel a fun��o se encerra
        return 0;
    }

    //Termina de preencher as informa��es do objeto e salva na pasta atual
    ptr_objeto->cluster_inicial = indice;
    ptr_objeto->tamanho = 0;
    if(salvaObjetoNaPasta(*ptr_objeto, ind_cluster_pasta) == 0){    //Se o objeto j� existe, a fun��o � encerrada
        return 0;
    }

    //Atualiza indice
    escreveIndice(indice, indice);

    return 1;
}

