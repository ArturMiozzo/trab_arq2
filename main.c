#include "header.h"

int main()
{
    //Deve ser executado s� na primeira vez, do contr�rio ela ir� apagar o arquivo atual e criar um novo.
    //Para testar funcionalidades � mais �til desativar depois de usado, pois d� pra ver melhor os impactos
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
    imprimePasta(pst.cluster_inicial); //Conte�do da pasta "exemplo de pasta" criada anteriormente.

    return 0;
}

//O indice de uma pasta vazia guarda seu pr�prio valor. Para verificar se a pasta � vazia � necess�rio varrer o cluster

//imprimePasta pode ter uma vers�o alternativa que n�o busque um espa�o vazio, mas sim leia os objetos baseado no tamanho em bytes informado

//MKFILE: salva o nome e a extens�o em um objeto; cria um vetor vazio e usa um sizeof dele para preencher o tamanho

//Fun��o que salva arquivos e pastas dentro de uma pasta deve atualizr o tamanho da pasta que est� armazenando esss informa��es. Pasta de
//tamanho 0 est� vazia. Se houverem itens, tamanho � igual ao n�mero de bytes desses itens
