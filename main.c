#include "header.h"

int main()
{
    //Deve ser executado só na primeira vez, do contrário ela irá apagar o arquivo atual e criar um novo.
    //Para testar funcionalidades é mais útil desativar depois de usado, pois dá pra ver melhor os impactos
    criaArquivoDados();


    OBJETO obj;
    obj = zeraObjeto(obj);
    strcpy(obj.nome, "Arquivo teste 1!");
    strcpy(obj.extensao, ".txt");
    char dados[] = "Teste de arquivo que cabe em um unico cluster! :D";

    OBJETO obj2;
    obj2 = zeraObjeto(obj2);
    strcpy(obj2.nome, "Arquivo teste 2!");
    strcpy(obj2.extensao, ".txt");
    char dados_2[32768];

    int i;
    for(i = 0; i < 32768; i++){
        dados_2[i] = '2';
    }

    OBJETO pst;
    pst = zeraObjeto(pst);
    strcpy(pst.nome, "Exemplo de pasta");
    strcpy(pst.extensao, "");

    //-----------------------------------------------------------------------
    //Exemplo de uso

    salvaArquivo(&obj, dados, 0);
    salvaArquivo(&obj2, dados_2, 0);
    salvaPasta(&pst, 0);
    salvaArquivo(&obj, dados, pst.cluster_inicial);

    imprimePasta(0); //Root
    imprimePasta(pst.cluster_inicial); //Conteúdo da pasta "exemplo de pasta" criada anteriormente.

    return 0;
}

//O indice de uma pasta vazia guarda seu próprio valor. Para verificar se a pasta é vazia é necessário varrer o cluster

//imprimePasta pode ter uma versão alternativa que não busque um espaço vazio, mas sim leia os objetos baseado no tamanho em bytes informado

//MKFILE: salva o nome e a extensão em um objeto; cria um vetor vazio e usa um sizeof dele para preencher o tamanho

//Função que salva arquivos e pastas dentro de uma pasta deve atualizr o tamanho da pasta que está armazenando esss informações. Pasta de
//tamanho 0 está vazia. Se houverem itens, tamanho é igual ao número de bytes desses itens
