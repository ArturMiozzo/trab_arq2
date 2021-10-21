#include "header.h"

int main()
{
    //Deve ser executado só na primeira vez, do contrário ela irá apagar o arquivo atual e criar um novo.
    //Para testar funcionalidades é mais útil desativar depois de usado, pois dá pra ver melhor os impactos
    //criaArquivoDados();


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

    OBJETO pst;
    pst = zeraObjeto(pst);
    pst = criaInfoPasta("Pasta exemplo");

    //-----------------------------------------------------------------------
    //Exemplo de uso


    /*
    salvaArquivo(obj, dados, 0);
    salvaArquivo(obj2, dados_2, 0);
    salvaPasta(pst, 0);
    salvaArquivo(obj, dados, retornaObjetoDaPasta(pst.nome, 0).cluster_inicial);*/


    /*
    obj = retornaObjetoDaPasta(obj.nome, 0);    //Pega a versão atualizada do objeto, com informações atualizadas de cluster e tamanho
    modificaArquivo(obj, dados_2, 0);*/


    return 0;
}

//Função que atualiza o tamanho da pasta



//O indice de uma pasta vazia guarda seu próprio valor. Para verificar se a pasta é vazia é necessário varrer o cluster ou checar o tamanho dela em suas informações


//Função que salva arquivos e pastas dentro de uma pasta deve atualizr o tamanho da pasta que está armazenando esss informações. Pasta de
//tamanho 0 está vazia. Se houverem itens, tamanho é igual ao número de bytes desses itens
