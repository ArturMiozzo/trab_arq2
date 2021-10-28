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
OBJETO pode ser usado tanto para arquivo quanto para pasta. Existem 4 casos poss�veis de uso:

I - Arquivo que ocupa 1 cluster:
    O �ndice na tabela aponta para ele pr�prio para indicar que � o �ltimo cluster.
    O tamanho do arquivo ser� <= tamanho de um cluster.

II - Arquivo que ocupa v�rios clusters:
    O �ndice vai apontando para o cluster seguinte at� chegar no indice que aponta para si mesmo.
    O tamanho do arquivo ser� > tamanho de um cluster

III - Pasta vazia:
    Pastas n�o tem extens�o, o que permite diferenciar o que � pasta e o que � arquivo
    O �ndice aponta para si mesmo, que referencia um cluster que vai armazenar as informa��es, mas que no momento est� vazio
    O tamanho da pasta � = 1, o que permite ler s� o primeiro byte e checar se a pasta realmente est� vazia

IV - Pasta cheia:
    Pastas n�o tem extens�o, o que permite diferenciar o que � pasta e o que � arquivo
    O �ndice aponta para si mesmo, que referencia o cluster lotado
    O tamanho da pasta � = ao tamanho de um cluster
*/

#endif // struct
