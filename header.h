#ifndef header

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "struct.h"

METADADOS zeraMetadados(METADADOS metadados);

INDICE zeraIndice(INDICE indice);

CLUSTER zeraCluster(CLUSTER cluster);

OBJETO zeraObjeto(OBJETO objeto);

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

int salvaArquivo(OBJETO* objeto, char dados[], uint8_t ind_cluster_pasta);

int salvaPasta(OBJETO* ptr_objeto, uint8_t ind_cluster_pasta);

#endif // header
