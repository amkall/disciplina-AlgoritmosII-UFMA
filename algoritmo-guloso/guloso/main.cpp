
#include<iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#pragma pack(push, 1)
typedef struct _packing_{
    int flag, peso_max, tamanho;
    int  *vet;
    char *aux, *sentinela;
}packing;
#pragma pack(pop)

double iter = 0;
/////////////////////////////////////////////////////////inicializa��o/////////////////////////////////////////////////////////////////////
char* inicializar(int tamanho, char a){
    char *novo;
    novo = new char[tamanho];
    for(int i= 0;i < tamanho; i++){
        novo[i] = a;
    }
    return novo;
}

packing Createpacking(int tamanho, int peso){
    packing *pacote = new packing[0];

    pacote->flag = tamanho;
    pacote->tamanho = tamanho;
    pacote->peso_max = peso;

    pacote->vet = new int[tamanho];
    pacote->aux = inicializar(tamanho, '0');
    pacote->sentinela = inicializar(tamanho, '1');


    return *pacote;
}
packing Import_Pesos(char *arq){
    packing novo;

    FILE *input;
	char buffer[128], aux[128], *tmp, *tmp2;
	bool flag=true;

	int peso, tamanho, cont = 0;

	input = fopen(arq, "r"); //abre o arquivo para leitura

    printf("\nConteudo do arquivo:\n\n");
 	while(! feof(input)) {
		fgets(buffer, 128, input);
		if(flag){
            strcpy(aux, buffer);
            flag = false;

            tmp  = strtok(aux, " ");
            tmp2 = strtok(0, " ");

            peso    = atoi(tmp);
            tamanho = atoi(tmp2);

            cout<<"peso maximo: "<<peso<<"\n";
            cout<<"qtd_elementos: " <<tamanho<<"\n";

            novo = Createpacking(tamanho, peso);

            cout<<"elementos : "<<"\n";

		}else{
            novo.vet[cont] = atoi(buffer);
            cont++;

            printf("%s", buffer);
		}
	}

 	fclose(input);

	return novo;

}
void SalvarSolucao(char* vet){
    FILE *arq;
    int result;
    int i;

    arq = fopen("saida.txt", "a"); // Cria um arquivo binário para gravação

    if (arq == NULL) // Se não conseguiu criar
    {
        printf("Problemas na CRIACAO do arquivo\n");
        return;
    }
    // Grava 30 números do vetor a partir da posição 50
    //fwrite (vet, sizeof(char), sizeof(vet), arq);
    fprintf(arq, "%s\r\n", vet);

    fclose(arq);
}
//////////////////////////////////////////////////////////fim inicializa��o//////////////////////////////////////////////////////////////




int Fitness(char *solucao, int *vet, int tamanho){
    int fit = 0;


    for(int i = 0; i < tamanho; i++){
        if(solucao[i] == '1'){
            fit += vet[i];

        }
    }


    return fit;
}

packing Atualizacao(packing pacote, char *solucao){
    int cont = 0;
    for(int i = 0; i < pacote.tamanho ; i++){
        if(solucao[i] == '1'){
            pacote.sentinela[i] = '0';
            cont ++;
        }
    }
    pacote.flag -= cont;

    return pacote;
}
void print(int *buffer,int tamanho){
    cout<<"\n";
    for(int i = 0;i < tamanho;i++){
        cout<<buffer[i]<<" ";
    }
}
void printS(char *buffer,int tamanho){
    cout<<"\n";
    for(int i = 0;i < tamanho;i++){
        cout<<buffer[i]<<" ";
    }
}
//função de ordenação o(n)

void shellSort(int *vet, int size) {
    int i , j , value;
    int gap = 1;
    while(gap < size) {
        gap = 3*gap+1;
    }
    while ( gap > 1) {
        gap /= 3;
        for(i = gap; i < size; i++) {
            value = vet[i];
            j = i - gap;
            while (j >= 0 && value < vet[j]) {
                vet [j + gap] = vet[j];
                j -= gap;
            }
            vet [j + gap] = value;
        }
    }
}


//função gulosa de empacotamento
packing Guloso(packing pacote){
    int peso_atual = 0, i = 0;



    while((peso_atual != pacote.peso_max) && (i < pacote.tamanho)){

        iter++;
        // verifica se o elemento atual ja foi empacotado
        if(pacote.sentinela[pacote.tamanho -1 -i] == '1'){
            //se não foi ele é empacotado
            pacote.aux[pacote.tamanho - 1 - i] = '1';
            //e verificado o movo peso da mochila
            peso_atual = Fitness(pacote.aux, pacote.vet, pacote.tamanho);

            //se ele estourou ele é removido, e o algoritmo tenta colocar o proximo peso a mochila
            if(peso_atual > pacote.peso_max) pacote.aux[pacote.tamanho - 1 - i] = '0';
        }

        i++;
    }


    SalvarSolucao(pacote.aux);

    //printS(pacote.aux, pacote.tamanho);
    //cout<<"\n";
    pacote = Atualizacao(pacote, pacote.aux);
    return pacote;
}
int main(){
    remove("saida.txt");

    int n_mochilas = 0;
    char argv[100];

    cout<<"informe o nome do arquivo que deseja enumerar"<<"\n";
    cin.getline(argv, 100);

    if(argv != NULL){
        char *aux = strstr(".txt", argv);
        if(aux == NULL){
            strcat(argv, ".txt");
        }
    }


    //carregando os dados
    packing pacote;
    pacote = Import_Pesos(argv);

    clock_t tInicio, tFim, tDecorrido;
    tInicio = clock();

    shellSort(pacote.vet, pacote.tamanho);
    print(pacote.vet, pacote.tamanho);

    //string.bin.flag e o numero de itens que ainda devem ser impacotados
    //enquanto houver itens a serem empacotados o algoritmo entra
    while (pacote.flag != 0){
        pacote = Guloso(pacote);
        n_mochilas++;
        delete pacote.aux;
        pacote.aux = inicializar(pacote.tamanho, '0');

    }
    tFim = clock();

    /*calcula aproximadamente o tempo em milisegundos gasto entre as duas chamadas de clock()*/
    tDecorrido = ((tFim - tInicio) / (CLOCKS_PER_SEC / 1000));


    cout<<"o numero minimo de pacotes e:" << n_mochilas<<"\n";
    cout<<"numero de iteracoes: "<<iter<<"\n";
    cout<<"tempo gasto: "<<tDecorrido<<" milisegundos \n";

    SalvarSolucao("O numero minimo de pacotes é:\n");
    char str[100];
    itoa(n_mochilas, str, 10);
    SalvarSolucao(str);

}

