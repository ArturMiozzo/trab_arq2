#include "header.h"

int main()
{
    //Deve ser executado s� na primeira vez, do contr�rio ela ir� apagar o arquivo atual e criar um novo.
    //Para testar funcionalidades � mais �til desativar depois de usado, pois d� pra ver melhor os impactos
    criaArquivoDados();


    OBJETO obj;
    obj = zeraObjeto(obj);
    obj = criaInfoArquivo("Arquivo teste 1!", ".txt");
    char dados[] = "Teste de arquivo que cabe em um unico cluster! :D";

    OBJETO obj2;
    obj2 = zeraObjeto(obj2);
    obj2 = criaInfoArquivo("Arquivo teste 2!", ".txt");
    char dados_2[50001];

    int i;
    for(i = 0; i < 50000; i++){
        dados_2[i] = '2';
    }
    dados_2[50000] = '\0';

    OBJETO obj3;
    obj3 = zeraObjeto();
    obj3 = criaInfoArquivo("Arquivo teste 3!", ".txt");
    char dados_3[] = "Mais um teste!";

    OBJETO pst;
    pst = zeraObjeto(pst);
    pst = criaInfoPasta("Pasta exemplo");

    OBJETO pst2;
    pst2 = zeraObjeto();
    pst2 = criaInfoPasta("Pasta exemplo 2");

    //-----------------------------------------------------------------------
    //Exemplo de uso

    OBJETO caminho[255];
    OBJETO root;

    for(i = 0; i < 255; i++){
        caminho[i] = zeraObjeto();
    }

    root = criaInfoPasta("Root");
    caminho[0] = root;




    salvaArquivo(obj, dados, caminho[0].cluster_inicial);
    salvaArquivo(obj2, dados_2, caminho[0].cluster_inicial);

    salvaPasta(pst, caminho[0].cluster_inicial);                                //Cria uma pasta dentro da pasta atual
    caminho[1] = retornaObjetoDaPasta(pst.nome, caminho[0].cluster_inicial);    //Salva no caminho a pasta "Pasta exemplo". O equivalidente a fazer um CD "Pasta exemplo"

    salvaArquivo(obj3, dados_3, caminho[1].cluster_inicial);                    //Salva um arquivo dentro da pasta
    atualizaTamanhoDasPastas(caminho, sizeof(caminho) / sizeof(caminho[0]));    //Atualiza o tamanho da pasta

    salvaPasta(pst2, caminho[1].cluster_inicial);                               //Cria uma pasta dentro da pasta atual
    caminho[2] = retornaObjetoDaPasta(pst2.nome, caminho[1].cluster_inicial);   //Salva no caminho a pasta "Pasta exemplo 2". O equivalidente a fazer um CD "Pasta exemplo 2"

    salvaArquivo(obj, dados, caminho[2].cluster_inicial);                       //Salva um arquivo dentro da pasta
    atualizaTamanhoDasPastas(caminho, sizeof(caminho) / sizeof(caminho[0]));    //Atualiza o tamanho da pasta

    apagaPastaVazia(pst2.nome, caminho[1].cluster_inicial);


    //Imprime o conte�do das pastas
    imprimePasta(caminho[0].cluster_inicial);
    printf("\n");
    imprimePasta(caminho[1].cluster_inicial);
    printf("\n");
    imprimePasta(caminho[2].cluster_inicial);
    printf("\n");

    return 0;
}


//Fun��o que retorna o tamanho em disco ocupado por tudo dentro da pasta atual (total de clusters ocupados * tamanho de um cluster, inclui o cluster da pasta atual).


//Fun��o que salva arquivos e pastas dentro de uma pasta deve atualizr o tamanho da pasta que est� armazenando esss informa��es. Pasta de
//tamanho 0 est� vazia. Se houverem itens, tamanho � igual ao n�mero de bytes desses itens
