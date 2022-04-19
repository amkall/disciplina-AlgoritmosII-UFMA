
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
}packing;
#pragma pack(pop)

double iter = 0;
/////////////////////////////////////////////////////////inicializa��o/////////////////////////////////////////////////////////////////////
int* inicializar(int tamanho, int a){
    int *novo;
    novo = new int[tamanho];
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


int knapSackRec(int W, int wt[], int val[], int i, int** dp)
{
    iter++;
    if (i < 0) return 0;

    if (dp[i][W] != -1) return dp[i][W];

    if (wt[i] > W) {

        dp[i][W] = knapSackRec(W, wt, val, i - 1, dp);

        return dp[i][W];
    }

    else { dp[i][W] = max(val[i] + knapSackRec(W - wt[i], wt, val, i - 1, dp),
                knapSackRec(W, wt, val, i - 1, dp));

            return dp[i][W];
    }
}



int knapSack(int W, int wt[], int val[], int n)
{
    int** dp;
    int M=0;
    dp = new int*[n];

    for (int i = 0; i < n; i++)
        dp[i] = new int[W + 1];

    while(n>0)  {
        for (int i = 0; i < n; i++)
                for (int j = 0; j < W + 1; j++)
                    dp[i][j] = -1;
        n -= knapSackRec(W, wt, val, n - 1, dp);
        // atualiza a quantidade de itens retirando aqueles que foram empacotados
        M++;  // cada vez que passar por aqui incrementa o numero de mochilas
        }
    return M;
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
    int *val = inicializar(pacote.tamanho, 1);

    tInicio = clock();
    n_mochilas = knapSack(pacote.peso_max, pacote.vet, val, pacote.flag);

    tFim = clock();

    /*calcula aproximadamente o tempo em milisegundos gasto entre as duas chamadas de clock()*/
    tDecorrido = ((tFim - tInicio) / (CLOCKS_PER_SEC / 1000));


    cout<<"o numero minimo de pacotes e:" << n_mochilas<<"\n";
    cout<<"numero de iteracoes: "<<iter<<"\n";
    cout<<"tempo gasto: "<<tDecorrido<<" milisegundos \n";



}

