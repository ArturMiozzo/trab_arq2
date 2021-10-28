//Componentes do grupo:
// - Artur Miozzo;
// - Arthur Casagrande
// - Paulo Roberto Gomes

#ifndef struct

#define FILENAME "LightFS.bin"

typedef struct{
    uint16_t tam_indice;
    uint16_t tam_cluster;
    uint16_t ini_indice;
    uint16_t ini_root;
}   METADADOS;

typedef struct{
    uint8_t indice[256];
}   INDICE;

typedef struct{
    uint8_t dados[32768];
}   CLUSTER;

typedef struct{
    char nome[54];
    char extensao[5];
    uint8_t cluster_inicial;
    unsigned int tamanho;
}   OBJETO;

/*
OBJETO pode ser usado tanto para arquivo quanto para pasta. Existem 4 casos possíveis de uso:

I - Arquivo que ocupa 1 cluster:
    O índice na tabela aponta para ele próprio para indicar que é o último cluster.
    O tamanho do arquivo será <= tamanho de um cluster.

II - Arquivo que ocupa vários clusters:
    O índice vai apontando para o cluster seguinte até chegar no indice que aponta para si mesmo.
    O tamanho do arquivo será > tamanho de um cluster

III - Pasta vazia:
    Pastas não tem extensão, o que permite diferenciar o que é pasta e o que é arquivo
    O índice aponta para si mesmo, que referencia um cluster que vai armazenar as informações, mas que no momento está vazio
    O tamanho da pasta é = 1, o que permite ler só o primeiro byte e checar se a pasta realmente está vazia

IV - Pasta cheia:
    Pastas não tem extensão, o que permite diferenciar o que é pasta e o que é arquivo
    O índice aponta para si mesmo, que referencia o cluster lotado
    O tamanho da pasta é = ao tamanho de um cluster
*/

#endif // struct
