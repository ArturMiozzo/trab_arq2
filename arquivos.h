//Componentes do grupo:
// - Artur Miozzo;
// - Arthur Casagrande
// - Paulo Roberto Gomes

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "struct.h"

//Fun��es que manipulam os arquivos
METADADOS zeraMetadados();

INDICE zeraIndice();

CLUSTER zeraCluster();

OBJETO zeraObjeto();

void criaArquivoDados(int reset);

CLUSTER leCluster(uint8_t ind_cluster);

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

int calculaTamanhoDaPasta(uint8_t ind_cluster_pasta, FILE* light_fs);

int modificaInfoTamanho(char nome[], int tam_atualizado, uint8_t ind_cluster_pasta, FILE* light_fs);

void atualizaTamanhoDasPastas(OBJETO caminho[], uint8_t tam_caminho);

int apagaPastaVazia(char nome[], uint8_t ind_cluster_pasta);

int apagaPastaCheia(char nome[], uint8_t ind_cluster_pasta);

void recursaoApagaPastaCheia(uint8_t ind_cluster_pasta);

OBJETO retornaObjetoOcupandoCluster(uint8_t ind_cluster_buscado, uint8_t* ind_cluster_salvo);

char* retornaDadosDoArquivo(OBJETO arquivo, uint8_t ind_cluster_pasta);

int retornaClusterCaminho(char *caminho, uint8_t ind_cluster_pasta);

int calculaTamanhoReservadoDaPasta(uint8_t ind_cluster_pasta);

int reorganizaClusters();
