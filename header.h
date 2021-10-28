#ifndef header

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

METADADOS zeraMetadados();

INDICE zeraIndice();

CLUSTER zeraCluster();

OBJETO zeraObjeto();

void criaArquivoDados();

CLUSTER leCluster(uint8_t num_cluster);

void escreveCluster(CLUSTER cluster, uint8_t ind_cluster);

void posicionaCursorNoIndice(uint8_t indice, FILE* light_fs);

uint8_t leIndice(uint8_t indice);

void escreveIndice(uint8_t valor, uint8_t indice);

void posicionaCursorNoCluster(uint8_t ind_cluster, FILE* light_fs);

bool checaNomeRepetido(char nome[], uint8_t ind_cluster, FILE* light_fs);

int salvaObjetoNaPasta(OBJETO objeto, uint8_t ind_cluster);

void removeObjetoDaPasta(char nome_objeto[], uint8_t ind_cluster);

void imprimePasta(uint8_t ind_cluster);

int buscaClusterLivre();

int buscaNumeroClustersLivres();

int salvaArquivo(OBJETO objeto, char dados[], uint8_t ind_cluster_pasta);

int salvaPasta(OBJETO objeto, uint8_t ind_cluster_pasta);

OBJETO criaInfoArquivo(char nome[], char extensao[]);

OBJETO criaInfoPasta(char nome[]);

OBJETO retornaObjetoDaPasta(char nome[], uint8_t ind_cluster_pasta);

void apagaArquivo(OBJETO arquivo, uint8_t ind_cluster_pasta);

int modificaArquivo(OBJETO arquivo, char novos_dados[], uint8_t ind_cluster_pasta);

int calculaTamanhoDaPasta(uint8_t ind_cluster_pasta);

int modificaInfoTamanho(char nome[], int tam_atualizado, uint8_t ind_cluster_pasta);

void atualizaTamanhoDasPastas(OBJETO caminho[], uint8_t tam_caminho);

int apagaPastaVazia(char nome[], uint8_t ind_cluster_pasta);

void lacoApagaArquivo(uint8_t ind_cluster_pasta);

void lacoEntraPastaCheia(uint8_t ind_cluster_pasta);

void lacoApagaPastaVazia(uint8_t ind_cluster_pasta);

void recursaoApagaPastaCheia(uint8_t ind_cluster_pasta);

int apagaPastaCheia(char nome[], uint8_t ind_cluster_pasta);

int modificaInfoCluster(char nome[], int cluster_atualizado, uint8_t ind_cluster_pasta);

char* retornaDadosDoArquivo(OBJETO arquivo, uint8_t ind_cluster_pasta);

OBJETO buscaOcupaCluster(uint8_t ind_cluster_buscado, uint8_t ind_cluster_atual, uint8_t* ind_cluster_salvo);

OBJETO retornaObjetoOcupandoCluster(uint8_t ind_cluster_buscado, uint8_t* ind_cluster_salvo);

int reorganizaClusters();

int recursaoCalculaTamanhoReservado(uint8_t ind_cluster_pasta);

int calculaTamanhoReservadoDaPasta(uint8_t ind_cluster_pasta);

//---------------------------------------------------------------------------------

#endif // header
