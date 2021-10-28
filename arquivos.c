//Componentes do grupo:
// - Artur Miozzo;
// - Arthur Casagrande
// - Paulo Roberto Gomes

#include "arquivos.h"

//Fun��es que manipulam os arquivos

METADADOS zeraMetadados(){

    METADADOS metadados;

    metadados.ini_indice = 0;
    metadados.ini_root = 0;
    metadados.tam_cluster = 0;
    metadados.tam_indice = 0;

    return metadados;
}

INDICE zeraIndice(){

    INDICE indice;

    int i;
    for(i = 0; i < sizeof(INDICE); i++){
        indice.indice[i] = 0;
    }
    return indice;
}

CLUSTER zeraCluster(){

    CLUSTER cluster;

    int i;
    for(i = 0; i < sizeof(CLUSTER); i++){
        cluster.dados[i] = 0;
    }

    return cluster;
}

OBJETO zeraObjeto(){

    OBJETO objeto;

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

void criaArquivoDados(int reset){
    //Cria o arquivo do sistema
    //Não sobrescreve arquivo existente se reset for 0

    FILE* light_fs;
    int i;

    light_fs = fopen(FILENAME, "rb");

    if(light_fs!=NULL && reset==0)
    {
        return;
    }

    fclose(light_fs);
    METADADOS meta;
    meta = zeraMetadados();
    meta.tam_indice = 256;
    meta.tam_cluster = 32768;
    meta.ini_indice = 8;
    meta.ini_root = 264;

    INDICE ind;
    ind = zeraIndice();

    CLUSTER clust;
    clust = zeraCluster();

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
    //Retorna o cluster do índice informado

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
    //Recebe um cluster como parâmetro e escreve ele dentro do arquivo do sistema

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
    //Coloca o cursor do arquivo no cluster informado

    rewind(light_fs);
    fseek(light_fs, sizeof(METADADOS), SEEK_SET);               //Salta os metadados
    fseek(light_fs, sizeof(INDICE), SEEK_CUR);                  //Salta o índice
    fseek(light_fs, ind_cluster * sizeof(CLUSTER), SEEK_CUR);   //Chega no cluster indicado
}

bool checaNomeRepetido(char nome[], uint8_t ind_cluster, FILE* light_fs){
    //Busca dentro da pasta se já existe algum item com o nome informado

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

    return false;   //Se chegar até aqui é porque não encontrou um nome repetido
}

int salvaObjetoNaPasta(OBJETO objeto, uint8_t ind_cluster){
    //Retorna 1 se tudo está OK
    //Retorna 0 se já existe um objeto de mesmo nome

    FILE* light_fs;
    char buffer;
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero máximo de objetos que podem existir em uma pasta

    light_fs = fopen(FILENAME, "rb+");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        //Checa se já não existe um objeto de mesmo nome
        if(checaNomeRepetido(objeto.nome, ind_cluster, light_fs)){
            printf("O item [%s] ja existe!\n", objeto.nome);
            return 0;
        }

        posicionaCursorNoCluster(ind_cluster, light_fs);

        //Busca a primeira área livre
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
            fwrite(&objeto, sizeof(OBJETO), 1, light_fs);           //Salva o objeto nessa posição livre
        }
        else{
            printf("Não há espaço disponível na pasta!\n");
        }


        fflush(light_fs);
    }
    fclose(light_fs);

    return 1;
}

void removeObjetoDaPasta(char nome_objeto[], uint8_t ind_cluster){
    //Apaga um item da pasta sobreescrevendo ele com um objeto vazio
    //Versão não segura. Não checa se é uma pasta vazia ou não.

    FILE* light_fs;
    OBJETO obj;
    char buffer[64];
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero máximo de objetos que podem existir em uma pasta

    obj = zeraObjeto();

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

            if(strcmp(buffer, nome_objeto) == 0){                        //Se encontrar um objeto com nome igual ao informado, apaga ele
                fseek(light_fs, -54 * sizeof(char), SEEK_CUR);
                fwrite(&obj, sizeof(OBJETO), 1, light_fs);
                break;
            }
            else{                                                   //Se não, continua a busca
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
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero máximo de objetos que podem existir em uma pasta
    int num_objetos = 0;

    light_fs = fopen(FILENAME, "rb+");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        posicionaCursorNoCluster(ind_cluster, light_fs);

        printf("N   - Nome                                                  - Extensao - Cluster ini - Tamanho\n");

        while(contador > 0){
            fread(&obj.nome, sizeof(obj.nome), 1, light_fs);
            fread(&obj.extensao, sizeof(obj.extensao), 1, light_fs);
            fread(&obj.cluster_inicial, sizeof(obj.cluster_inicial), 1, light_fs);
            fread(&obj.tamanho, sizeof(obj.tamanho), 1, light_fs);

            if(obj.nome[0] == 0){
                contador--;
                continue;
            }
            else{
                num_objetos++;

                printf("%-3d - ", num_objetos);
                printf("%-53s - ", obj.nome);
                printf("%-5s    - ", obj.extensao);
                printf("%-3d         - ", obj.cluster_inicial);
                printf("%-5d  ", obj.tamanho);
                printf("\n");
            }

            contador--;
        }

        fflush(light_fs);

        if(num_objetos == 0){
            printf("      <Vazio>\n");
        }

    }
    fclose(light_fs);
}

int buscaClusterLivre(){
    //Retorna o índice do primeiro cluster livre encontrado

    int indice;
    for(indice = 1; indice < sizeof(INDICE); indice++){
        if(leIndice(indice) == 0){
            break;
        }
    }

    if(indice == sizeof(INDICE)){   //Não encontrou cluster livre
        return -1;
    }
    else{
        return indice;              //Encontrou cluster livre
    }
}

int buscaNumeroClustersLivres(){
    //Retorna o número total de cluster disponíveis para uso

    int i;
    int total = 0;
    for(i = 1; i < sizeof(INDICE); i++){
        if(leIndice(i) == 0){
            total++;
        }
    }

    return total;
}

int salvaArquivo(OBJETO objeto, char dados[], uint8_t ind_cluster_pasta){
    //Salvamento completo. Salva as informações do arquivo dentro da pasta, aloca os dados dentro de um cluster livre e atualiza os índices.
    //Return 0 se acontecer algum problema
    //Return 1 se ocorrer tudo certo

    int indice_atual;
    int indice_seg;
    int i;
    int contador = 0;
    int num_clusters;
    CLUSTER clust;

    clust = zeraCluster();

    num_clusters = ((strlen(dados) + 1) / sizeof(CLUSTER)) + 1;

    if(buscaNumeroClustersLivres() < num_clusters){             //Checa se são necessários mais clusters do que se tem disponíveis
        printf("Nao existem clusters livres suficientes!");
        return 0;
    }

    //Busca um cluster vazio para guardar os dados
    indice_atual = buscaClusterLivre();

    //Termina de preencher as informações do objeto e salva na pasta
    objeto.cluster_inicial = indice_atual;
    objeto.tamanho = strlen(dados) + 1;
    if(salvaObjetoNaPasta(objeto, ind_cluster_pasta) == 0){    //Se o objeto já existe, a função é encerrada
        return 0;
    }


    //Começa a etapa de armazenar os dados
    if(objeto.tamanho <= sizeof(CLUSTER)){                 //Os dados cabem em um único cluster

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
                clust = zeraCluster();
            }
        }

        escreveCluster(clust, indice_atual);                    //Último cluster parcialmente preenchido
        escreveIndice(indice_atual, indice_atual);
    }

    return 1;
}

int salvaPasta(OBJETO objeto, uint8_t ind_cluster_pasta){
    //Salva uma pasta criada dentro da pasta atual
    //Return 0 se acontecer algum problema
    //Return 1 se ocorrer tudo certo

    int indice;

    //Busca um cluster vazio para guardar os dados
    indice = buscaClusterLivre();
    if(indice == -1){                     //Se não houver cluster disponível a função se encerra
        return 0;
    }

    //Termina de preencher as informações do objeto e salva na pasta atual
    objeto.cluster_inicial = indice;
    objeto.tamanho = 0;
    if(salvaObjetoNaPasta(objeto, ind_cluster_pasta) == 0){    //Se o objeto já existe, a função é encerrada
        return 0;
    }

    //Atualiza indice
    escreveIndice(indice, indice);

    return 1;
}

OBJETO criaInfoArquivo(char nome[], char extensao[]){
    //Inicializa um objeto como arquivo

    OBJETO objeto;
    objeto = zeraObjeto();

    strcpy(objeto.nome, nome);
    strcpy(objeto.extensao, extensao);
    objeto.cluster_inicial = 0;
    objeto.tamanho = 0;

    return objeto;
}

OBJETO criaInfoPasta(char nome[]){
    //Inicializa um objeto como pasta

    int i;
    OBJETO objeto;
    objeto = zeraObjeto();

    strcpy(objeto.nome, nome);
    for(i = 0; i < sizeof(objeto.extensao); i++){
        objeto.extensao[i] = 0;
    }
    objeto.cluster_inicial = 0;
    objeto.tamanho = 0;

    return objeto;
}

OBJETO retornaObjetoDaPasta(char nome[], uint8_t ind_cluster_pasta){
    //Retorna um objeto de uma pasta buscando pelo seu nome

    FILE* light_fs;
    OBJETO obj;
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero máximo de objetos que podem existir em uma pasta

    light_fs = fopen(FILENAME, "rb+");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        posicionaCursorNoCluster(ind_cluster_pasta, light_fs);

        while(contador > 0){

            fread(&obj.nome, sizeof(obj.nome), 1, light_fs);
            fread(&obj.extensao, sizeof(obj.extensao), 1, light_fs);
            fread(&obj.cluster_inicial, sizeof(obj.cluster_inicial), 1, light_fs);
            fread(&obj.tamanho, sizeof(obj.tamanho), 1, light_fs);

            if(strcmp(obj.nome, nome) == 0){
                break;
            }

            contador--;
        }

        fflush(light_fs);
    }
    fclose(light_fs);

    if(contador == 0){                                  //Se não encontrar o objeto informado retorna um objeto vazio
        printf("Item nao encontrado na pasta!\n");
        obj = zeraObjeto();
        return obj;
    }
    else{                                               //Se encontrar retorna o objeto
        return obj;
    }
}

void apagaArquivo(OBJETO arquivo, uint8_t ind_cluster_pasta){
    //Apaga um arquivo, tanto suas informações da pasta quanto os dados alocados

    uint8_t indice_atual;
    uint8_t indice_seg;
    CLUSTER clust;

    indice_atual = arquivo.cluster_inicial;
    indice_seg = leIndice(indice_atual);
    clust = zeraCluster();

    //Limpa os clusters e índices
    while(indice_atual != indice_seg){
        escreveCluster(clust, indice_atual);
        escreveIndice(0, indice_atual);
        indice_atual = indice_seg;
        indice_seg = leIndice(indice_atual);
    }
    escreveCluster(clust, indice_atual);
    escreveIndice(0, indice_atual);

    removeObjetoDaPasta(arquivo.nome, ind_cluster_pasta);
}

void apagaPasta(OBJETO pasta, uint8_t ind_cluster_pasta){
    //Versão não segura. Apaga as informações, limpa o índice e o cluster

    uint8_t indice = pasta.cluster_inicial;
    CLUSTER clust = zeraCluster();

    escreveCluster(clust, indice);
    escreveIndice(0, indice);
    removeObjetoDaPasta(pasta.nome, ind_cluster_pasta);
}

int modificaArquivo(OBJETO arquivo, char novos_dados[], uint8_t ind_cluster_pasta){
    //Apaga a versão original e salva uma nova versão com os novos dados, atualizando tanto as informações na pasta quando os dados nos clusters

    int espaco_necessario = strlen(novos_dados) + 1;
    int espaco_livre = (buscaNumeroClustersLivres() * sizeof(CLUSTER)) + arquivo.tamanho;

    if(espaco_necessario > espaco_livre){

        printf("Nao ha espaço disponivel para alocar os novos dados!\n");
        return 0;
    }
    else{

        apagaArquivo(arquivo, ind_cluster_pasta);
        salvaArquivo(arquivo, novos_dados, ind_cluster_pasta);

        return 1;
    }
}

int calculaTamanhoDaPasta(uint8_t ind_cluster_pasta){
    //Calcula o tamanho de tudo que está armazenado dentro da pasta atual somando seus tamanhos

    FILE* light_fs;
    OBJETO obj;
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero máximo de objetos que podem existir em uma pasta
    int tamanho_total = 0;
    int objetos = 0;

    light_fs = fopen(FILENAME, "rb+");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        posicionaCursorNoCluster(ind_cluster_pasta, light_fs);

        while(contador > 0){
            fread(&obj.nome, sizeof(obj.nome), 1, light_fs);
            fread(&obj.extensao, sizeof(obj.extensao), 1, light_fs);
            fread(&obj.cluster_inicial, sizeof(obj.cluster_inicial), 1, light_fs);
            fread(&obj.tamanho, sizeof(obj.tamanho), 1, light_fs);

            if(obj.nome[0] != 0){
                tamanho_total = tamanho_total + obj.tamanho;
                objetos++;
            }

            contador--;
        }

        fflush(light_fs);
    }
    fclose(light_fs);

    return tamanho_total + (objetos * sizeof(OBJETO));
}

int tamanhoReservadoEmDisco(){
    //Retorna o tamanho em bytes de todos os clusters ocupados

    int i;
    int num_clusters_ocupados = 0;

    for(i = 0; i < sizeof(INDICE); i++){
        if(leIndice(i) != 0){
            num_clusters_ocupados++;
        }
    }

    return (num_clusters_ocupados * sizeof(CLUSTER)) + sizeof(CLUSTER);
}

int modificaInfoTamanho(char nome[], int tam_atualizado, uint8_t ind_cluster_pasta){

    FILE* light_fs;
    OBJETO obj = zeraObjeto();
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero máximo de objetos que podem existir em uma pasta

    light_fs = fopen(FILENAME, "rb+");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        posicionaCursorNoCluster(ind_cluster_pasta, light_fs);

        while(contador > 0){

            fread(&obj.nome, sizeof(obj.nome), 1, light_fs);
            fread(&obj.extensao, sizeof(obj.extensao), 1, light_fs);
            fread(&obj.cluster_inicial, sizeof(obj.cluster_inicial), 1, light_fs);
            fread(&obj.tamanho, sizeof(obj.tamanho), 1, light_fs);

            if(strcmp(obj.nome, nome) == 0){
                fseek(light_fs, -1 * sizeof(obj.tamanho), SEEK_CUR);
                break;
            }

            contador--;
        }

        fflush(light_fs);
    }


    if(contador == 0){
        printf("Item nao encontrado na pasta!\n");
        fclose(light_fs);
        return 0;
    }
    else{

        fwrite(&tam_atualizado, sizeof(obj.tamanho), 1, light_fs);
        fclose(light_fs);
        return 1;
    }
}

void atualizaTamanhoDasPastas(OBJETO caminho[], uint8_t tam_caminho){
    //Atualiza o tamanho de todas as pastas que estão no caminho da criação/remoção/modificação de um arquivo
    //caminho é o vetor que guarda as informações do caminho de pastas acessadas desde a raíz, que fica em caminho[0], até a pasta atual
    //É necessário informar também o tamanho do vetor de caminho

    FILE* light_fs;
    OBJETO obj = zeraObjeto();
    int i;
    int num_pastas = 0;
    int tamanho = 0;
    int indice_pasta_atual = 0;
    int indice_pasta_anterior = 0;

    light_fs = fopen(FILENAME, "rb+");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        for(i = 0; i < tam_caminho; i++){

            if(strcmp(caminho[i].nome, obj.nome) != 0){
                num_pastas++;
            }
        }

        for(i = num_pastas - 1; i > 0; i--){

            indice_pasta_atual = caminho[i].cluster_inicial;
            indice_pasta_anterior = caminho[i-1].cluster_inicial;

            posicionaCursorNoCluster(indice_pasta_atual, light_fs);
            tamanho = calculaTamanhoDaPasta(indice_pasta_atual);

            posicionaCursorNoCluster(indice_pasta_anterior, light_fs);
            modificaInfoTamanho(caminho[i].nome, tamanho, indice_pasta_anterior);
        }

        fflush(light_fs);
    }

    fclose(light_fs);
}

int apagaPastaVazia(char nome[], uint8_t ind_cluster_pasta){
    //Apaga as informações da pasta atual, zera o índice e sobreescreve o cluster alocado com um cluster vazio

    FILE* light_fs;
    OBJETO obj = zeraObjeto();
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero máximo de objetos que podem existir em uma pasta

    light_fs = fopen(FILENAME, "rb+");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        posicionaCursorNoCluster(ind_cluster_pasta, light_fs);

        while(contador > 0){

            fread(&obj.nome, sizeof(obj.nome), 1, light_fs);
            fread(&obj.extensao, sizeof(obj.extensao), 1, light_fs);
            fread(&obj.cluster_inicial, sizeof(obj.cluster_inicial), 1, light_fs);
            fread(&obj.tamanho, sizeof(obj.tamanho), 1, light_fs);

            if(strcmp(obj.nome, nome) == 0){
                break;
            }

            contador--;
        }

        fflush(light_fs);
    }
    fclose(light_fs);

    if(contador == 0){
        printf("Item nao encontrado na pasta!\n");
        return 0;
    }
    else if(obj.tamanho != 0){
        printf("A pasta nao pode ser apagada porque tem itens dentro!\n");
        return 0;
    }
    else{

        apagaPasta(obj, ind_cluster_pasta);

        return 1;
    }
}

void lacoApagaArquivo(uint8_t ind_cluster_pasta){
    //Apaga todos os arquivos dentro da pasta atual

    FILE* light_fs;
    OBJETO obj = zeraObjeto();
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero máximo de objetos que podem existir em uma pasta
    int objetos_lidos = 0;


    //Laço que apaga os arquivos
    while(contador > 0){

        light_fs = fopen(FILENAME, "rb+");
        if(light_fs == NULL)
        {
            printf("Erro na abertura do arquivo!\n");
        }
        else
        {
            posicionaCursorNoCluster(ind_cluster_pasta, light_fs);
            fseek(light_fs, objetos_lidos * sizeof(OBJETO), SEEK_CUR);

            fread(&obj.nome, sizeof(obj.nome), 1, light_fs);
            fread(&obj.extensao, sizeof(obj.extensao), 1, light_fs);
            fread(&obj.cluster_inicial, sizeof(obj.cluster_inicial), 1, light_fs);
            fread(&obj.tamanho, sizeof(obj.tamanho), 1, light_fs);

            objetos_lidos++;

            if(obj.nome[0] != 0 && obj.extensao[0] != 0){

                fclose(light_fs);
                apagaArquivo(obj, ind_cluster_pasta);       //A função abre e fecha o arquivo, então antes de chamar ela a chamada atual do arquivo deve ser fechada
                contador--;
                continue;
            }
        }

        fclose(light_fs);
        contador--;
    }

}

void lacoEntraPastaCheia(uint8_t ind_cluster_pasta){
    //Busca as pastas ocupadas dentro da pasta atual e faz a chamada recursiva

    FILE* light_fs;
    OBJETO obj = zeraObjeto();
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero máximo de objetos que podem existir em uma pasta
    int objetos_lidos = 0;


    //Laço que busca as pastas cheias para limpar
    while(contador > 0){

        light_fs = fopen(FILENAME, "rb+");
        if(light_fs == NULL)
        {
            printf("Erro na abertura do arquivo!\n");
        }
        else
        {
            posicionaCursorNoCluster(ind_cluster_pasta, light_fs);
            fseek(light_fs, objetos_lidos * sizeof(OBJETO), SEEK_CUR);

            fread(&obj.nome, sizeof(obj.nome), 1, light_fs);
            fread(&obj.extensao, sizeof(obj.extensao), 1, light_fs);
            fread(&obj.cluster_inicial, sizeof(obj.cluster_inicial), 1, light_fs);
            fread(&obj.tamanho, sizeof(obj.tamanho), 1, light_fs);

            objetos_lidos++;

            if(obj.nome[0] != 0 && obj.extensao[0] == 0 && obj.tamanho != 0){

                fclose(light_fs);
                recursaoApagaPastaCheia(obj.cluster_inicial);   //A função abre e fecha o arquivo, então antes de chamar ela a chamada atual do arquivo deve ser fechada
                contador--;
                continue;

            }
        }

        fclose(light_fs);
        contador--;
    }

}

void lacoApagaPastaVazia(uint8_t ind_cluster_pasta){
    //Apaga todas as pastas vazias dentro da pasta atual

    FILE* light_fs;
    OBJETO obj = zeraObjeto();
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero máximo de objetos que podem existir em uma pasta
    int objetos_lidos = 0;


    //Laço que busca as pastas cheias para limpar
    while(contador > 0){

        light_fs = fopen(FILENAME, "rb+");
        if(light_fs == NULL)
        {
            printf("Erro na abertura do arquivo!\n");
        }
        else
        {
            posicionaCursorNoCluster(ind_cluster_pasta, light_fs);
            fseek(light_fs, objetos_lidos * sizeof(OBJETO), SEEK_CUR);

            fread(&obj.nome, sizeof(obj.nome), 1, light_fs);
            fread(&obj.extensao, sizeof(obj.extensao), 1, light_fs);
            fread(&obj.cluster_inicial, sizeof(obj.cluster_inicial), 1, light_fs);
            fread(&obj.tamanho, sizeof(obj.tamanho), 1, light_fs);

            objetos_lidos++;



            if(obj.nome[0] != 0 && obj.extensao[0] == 0){
                fclose(light_fs);
                apagaPasta(obj, ind_cluster_pasta);   //A função abre e fecha o arquivo, então antes de chamar ela a chamada atual do arquivo deve ser fechada
                contador--;
                continue;


            }
        }

        fclose(light_fs);
        contador--;
    }

}

void recursaoApagaPastaCheia(uint8_t ind_cluster_pasta){
    //Deleta recursivamente tudo que existe dentro da pasta informada

    lacoApagaArquivo(ind_cluster_pasta);
    lacoEntraPastaCheia(ind_cluster_pasta);
    lacoApagaPastaVazia(ind_cluster_pasta);
}

int apagaPastaCheia(char nome[], uint8_t ind_cluster_pasta){
    //Busca a pasta informada, deleta tudo que existe dentro dela e então deleta ela já vazia

    FILE* light_fs;
    OBJETO obj = zeraObjeto();
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero máximo de objetos que podem existir em uma pasta

    light_fs = fopen(FILENAME, "rb+");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        //Busca a pasta informada
        posicionaCursorNoCluster(ind_cluster_pasta, light_fs);

        while(contador > 0){

            fread(&obj.nome, sizeof(obj.nome), 1, light_fs);
            fread(&obj.extensao, sizeof(obj.extensao), 1, light_fs);
            fread(&obj.cluster_inicial, sizeof(obj.cluster_inicial), 1, light_fs);
            fread(&obj.tamanho, sizeof(obj.tamanho), 1, light_fs);

            if(strcmp(obj.nome, nome) == 0){

                break;
            }

            contador--;
        }

        fflush(light_fs);
    }

    fclose(light_fs);

    if(contador == 0){
        printf("Item nao encontrado na pasta!\n");

        return 0;
    }
    else{

        //Apaga tudo que tem dentro dela recursivamente
        recursaoApagaPastaCheia(obj.cluster_inicial);

        //Apaga a pasta já vazia
        apagaPasta(obj, ind_cluster_pasta);

        return 1;
    }
}

int modificaInfoCluster(char nome[], int cluster_atualizado, uint8_t ind_cluster_pasta){

    FILE* light_fs;
    OBJETO obj = zeraObjeto();
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero máximo de objetos que podem existir em uma pasta

    light_fs = fopen(FILENAME, "rb+");
    if(light_fs == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
    }
    else
    {
        posicionaCursorNoCluster(ind_cluster_pasta, light_fs);

        while(contador > 0){

            fread(&obj.nome, sizeof(obj.nome), 1, light_fs);
            fread(&obj.extensao, sizeof(obj.extensao), 1, light_fs);
            fread(&obj.cluster_inicial, sizeof(obj.cluster_inicial), 1, light_fs);
            fread(&obj.tamanho, sizeof(obj.tamanho), 1, light_fs);

            if(strcmp(obj.nome, nome) == 0){
                fseek(light_fs, -1 * (sizeof(obj.tamanho) + sizeof(obj.cluster_inicial)), SEEK_CUR);
                break;
            }

            contador--;
        }

        fflush(light_fs);
    }


    if(contador == 0){
        printf("Item nao encontrado na pasta!\n");
        fclose(light_fs);
        return 0;
    }
    else{

        fwrite(&cluster_atualizado, sizeof(obj.cluster_inicial), 1, light_fs);
        fclose(light_fs);
        return 1;
    }
}

int reorganizaClusters(){
    //Percorre os índices. Quando acha um indice livre vai em busca de um ocupado na sequência. Se isso acontecer, salva esse arquivo em um buffer, apaga
    //ele do sistema e então salva novamente no sistema.

    int i, j;
    OBJETO obj = zeraObjeto();
    CLUSTER clust = zeraCluster();
    uint8_t cluster_salvo = 0;
    int objetos_movidos = 0;

    for(i = 1; i < sizeof(INDICE); i++){

        if(leIndice(i) == 0){   //Encontrou um cluster vazio

            for(j = i; j < sizeof(INDICE); j++){

                if(leIndice(j) != 0){   //Encontrou um cluster ocupado

                    obj = retornaObjetoOcupandoCluster(j, &cluster_salvo);

                    if(obj.extensao[0] == 0){   //É uma pasta

                        //Pasta vazia. Corrige os índices e atualiza o apontamento de cluster inicial
                        if(obj.tamanho == 0){
                            escreveIndice(0, j);
                            escreveIndice(i, i);
                            modificaInfoCluster(obj.nome, i, cluster_salvo);
                            objetos_movidos++;
                        }
                        //Pasta ocupada. Faz backup das informações de dentro dela, corrige os índices, atualiza o apontamento de cluster inicial e restaura o backup
                        else{
                            clust = leCluster(obj.cluster_inicial);
                            escreveIndice(0, j);
                            escreveIndice(i, i);
                            modificaInfoCluster(obj.nome, i, cluster_salvo);
                            escreveCluster(clust, i);
                            objetos_movidos++;
                        }

                    }
                    else{                       //É um arquivo
                        //Faz backup dos dados, apaga o arquivo e salva novamente
                        char *dados = (char*) malloc(obj.tamanho * sizeof(char));
                        dados = retornaDadosDoArquivo(obj, cluster_salvo);

                        apagaArquivo(obj, cluster_salvo);
                        salvaArquivo(obj, dados, cluster_salvo);
                        objetos_movidos++;
                    }

                    break;
                }
            }

        }

        cluster_salvo = 0;
    }

    return objetos_movidos;
}

char* retornaDadosDoArquivo(OBJETO arquivo, uint8_t ind_cluster_pasta){
    //Lê e retorna os dados de um arquivo

    uint8_t indice_atual;
    uint8_t indice_seg;
    char *dados = (char*) malloc(arquivo.tamanho * sizeof(char));
    CLUSTER clust = zeraCluster();
    int i;

    for(i = 0; i < arquivo.tamanho; i++){
        dados[i] = 0;
    }

    indice_atual = arquivo.cluster_inicial;
    indice_seg = leIndice(indice_atual);

    //Limpa os clusters e índices
    while(indice_atual != indice_seg){

        clust = leCluster(indice_atual);
        strcat(dados, clust.dados);

        indice_atual = indice_seg;
        indice_seg = leIndice(indice_atual);
    }
    clust = leCluster(indice_atual);
    strcat(dados, clust.dados);

    return dados;
}

OBJETO buscaOcupaCluster(uint8_t ind_cluster_buscado, uint8_t ind_cluster_atual, uint8_t* ind_cluster_salvo){
    //Busca qual objeto ocupa o cluster informado. Retorna via ponteiro em qual pasta as informações desse onjeto estão guardadas

    FILE* light_fs;
    OBJETO obj = zeraObjeto();
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero máximo de objetos que podem existir em uma pasta
    int objetos_lidos = 0;
    int ind_at = 0;
    int ind_seg = 0;


    while(contador > 0){

        light_fs = fopen(FILENAME, "rb+");
        if(light_fs == NULL)
        {
            printf("Erro na abertura do arquivo!\n");
        }
        else
        {
            posicionaCursorNoCluster(ind_cluster_atual, light_fs);
            fseek(light_fs, objetos_lidos * sizeof(OBJETO), SEEK_CUR);

            fread(&obj.nome, sizeof(obj.nome), 1, light_fs);
            fread(&obj.extensao, sizeof(obj.extensao), 1, light_fs);
            fread(&obj.cluster_inicial, sizeof(obj.cluster_inicial), 1, light_fs);
            fread(&obj.tamanho, sizeof(obj.tamanho), 1, light_fs);

            objetos_lidos++;
            fclose(light_fs);

            if(obj.cluster_inicial == ind_cluster_buscado){ //O cluster buscado é o cluster inicial de algum objeto

                if(*ind_cluster_salvo <= 0){
                    *ind_cluster_salvo = ind_cluster_atual;
                }
                break;
            }
            else if(obj.tamanho > sizeof(CLUSTER) && obj.extensao[0] != 0){  //Checa se o objeto é um arquivo que ocupa vários clusters

                ind_at = obj.cluster_inicial;
                ind_seg = leIndice(ind_at);

                while(ind_at != ind_seg){
                    if(ind_at == ind_cluster_buscado){
                        if(*ind_cluster_salvo <= 0){
                            *ind_cluster_salvo = ind_cluster_atual;
                        }
                        break;
                    }

                    ind_at = ind_seg;
                    ind_seg = leIndice(ind_at);
                }
                if(ind_at == ind_cluster_buscado){
                    if(*ind_cluster_salvo <= 0){
                        *ind_cluster_salvo = ind_cluster_atual;
                    }
                    break;
                }
            }
            else if(obj.tamanho > 0 && obj.extensao[0] == 0){   //Entra na pasta ocupada e segue a busca

                obj = buscaOcupaCluster(ind_cluster_buscado, obj.cluster_inicial, ind_cluster_salvo);
                if(obj.nome[0] != 0){
                    if(*ind_cluster_salvo <= 0){
                        *ind_cluster_salvo = ind_cluster_atual;
                    }
                    break;
                }
            }

        }

        contador--;
    }

    return obj;
}

OBJETO retornaObjetoOcupandoCluster(uint8_t ind_cluster_buscado, uint8_t* ind_cluster_salvo){

    return buscaOcupaCluster(ind_cluster_buscado, 0, ind_cluster_salvo);
}

int recursaoCalculaTamanhoReservado(uint8_t ind_cluster_pasta){

    FILE* light_fs;
    OBJETO obj = zeraObjeto();
    int contador = sizeof(CLUSTER) / sizeof(OBJETO); //Numero máximo de objetos que podem existir em uma pasta
    int objetos_lidos = 0;
    int tamanho = 0;


    while(contador > 0){

        light_fs = fopen(FILENAME, "rb+");
        if(light_fs == NULL)
        {
            printf("Erro na abertura do arquivo!\n");
        }
        else
        {
            posicionaCursorNoCluster(ind_cluster_pasta, light_fs);
            fseek(light_fs, objetos_lidos * sizeof(OBJETO), SEEK_CUR);

            fread(&obj.nome, sizeof(obj.nome), 1, light_fs);
            fread(&obj.extensao, sizeof(obj.extensao), 1, light_fs);
            fread(&obj.cluster_inicial, sizeof(obj.cluster_inicial), 1, light_fs);
            fread(&obj.tamanho, sizeof(obj.tamanho), 1, light_fs);

            objetos_lidos++;
            fclose(light_fs);

            if(obj.tamanho <= sizeof(CLUSTER) && obj.extensao[0] != 0){ //É um arquivo que ocupa 1 só cluster

                tamanho = tamanho + sizeof(CLUSTER);
            }
            else if(obj.tamanho > sizeof(CLUSTER) && obj.extensao[0] != 0){  //É um arquivo que ocupa vários clusters

                tamanho = tamanho + (((obj.tamanho / sizeof(CLUSTER)) + 1) * sizeof(CLUSTER));
            }
            else if(obj.nome[0] != 0 && obj.tamanho == 0 && obj.extensao[0] == 0){   //É uma pasta vazia

                tamanho = tamanho + sizeof(CLUSTER);
            }
            else if(obj.nome[0] != 0 && obj.tamanho != 0 && obj.extensao[0] == 0){   //É uma pasta ocupada

                tamanho = tamanho + sizeof(CLUSTER) + recursaoCalculaTamanhoReservado(obj.cluster_inicial);
            }

        }

        contador--;
    }

    return tamanho;
}

int calculaTamanhoReservadoDaPasta(uint8_t ind_cluster_pasta){

    return recursaoCalculaTamanhoReservado(ind_cluster_pasta) + sizeof(CLUSTER);
}


//---------------------------------------------------------------------------------

int retornaClusterCaminho(char *caminho, uint8_t ind_cluster_pasta){

    OBJETO pasta;
    char root[100];
    char ptr[100];

    char *ptr_aux = strchr(caminho,'/');

    int index = ptr_aux-caminho;

    if(index>0)
    {
        strcpy(ptr, caminho);

        strncpy(root, caminho, index);
        root[index] = '\0';

        pasta = retornaObjetoDaPasta(root, ind_cluster_pasta); //le se a pasta do inicio do caminho eh subpasta da pasta

        if(pasta.tamanho==0 && strcmp(root, "Root")==0) pasta.cluster_inicial=0;

        ptr_aux = strchr(ptr,'/');
        index = ptr_aux-ptr;

        while(index>0)
        {
            strcpy(ptr, ptr_aux);
            memmove(ptr, ptr+1, strlen(ptr));
            ptr_aux = strchr(ptr,'/');
            index = ptr_aux-ptr;

            if(index>0)
            {
                strncpy(root, ptr, index);
                root[index] = '\0';
            }
            else strcpy(root, ptr);

            pasta = retornaObjetoDaPasta(root, pasta.cluster_inicial); //le se a pasta do inicio do caminho eh subpasta da pasta
        }
        if(pasta.cluster_inicial==0) return -1;
    }
    else
    {
        pasta = retornaObjetoDaPasta(caminho, ind_cluster_pasta); //le se a pasta do inicio do caminho eh subpasta da pasta
        if(pasta.nome[0]==0)
        {
            if(strcmp(caminho, "Root") == 0)
                return 0;
            else return -1;
        }
    }

	return pasta.cluster_inicial;
}
