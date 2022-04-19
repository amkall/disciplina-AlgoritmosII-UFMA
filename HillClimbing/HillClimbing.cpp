
#include<iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#pragma pack(push, 1)
typedef struct _packing_{
    int flag, peso_max, tamanho, n_mochilas;
    int  *vet;
}packing;
#pragma pack(pop)

double iter = 0;
/////////////////////////////////////////////////////////inicializa��o/////////////////////////////////////////////////////////////////////
int* inicializar(int tamanho){
    int *novo;
    novo = new  int[tamanho];
    for(int i= 0;i < tamanho; i++){
        novo[i] = 0;
    }
    return novo;
}
int* Cpy(int* vet, int tam){
    int *novo = new int[tam];

    for(int i = 0;i < tam;i++){
        novo[i] = vet[i];
    }return novo;
}
packing Createpacking(int tamanho, int peso, int n_mochilas){
    packing *pacote = new packing[0];

    pacote->flag = tamanho;
    pacote->tamanho = tamanho;
    pacote->peso_max = peso;
    pacote->n_mochilas = n_mochilas;
    pacote->vet = new int[tamanho];


    return *pacote;
}
void print(int *buffer,int tamanho){
    cout<<"\n";
    for(int i = 0;i < tamanho;i++){
        cout<<buffer[i]<<" ";
    }
}
packing Import_Pesos(char *arq){
    packing novo;

    FILE *input;
	char buffer[128], aux[128], *tmp, *tmp2;
	bool flag=true;

	int peso, tamanho, n_mochilas, cont = 0;

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

            tmp = strtok(0, " ");
            n_mochilas = atoi(tmp);

            tamanho = atoi(tmp2);

            cout<<"peso maximo: "<<peso<<"\n";
            cout<<"qtd_elementos: " <<tamanho<<"\n";
            cout<<"n_mochilas: " <<n_mochilas<<"\n";

            novo = Createpacking(tamanho, peso, n_mochilas);

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
int* GeradorRandom(packing pacote){
    int *nova = new int[pacote.tamanho];

    for(int i = 0; i < pacote.tamanho;i++){
        nova[i] = rand() % pacote.n_mochilas;
    }return nova;
}

int Residuo(int *solucao, packing pacote){
    int *aux = inicializar(pacote.n_mochilas);

    if(solucao == NULL) return INT_MAX;

    for(int i = 0; i < pacote.tamanho; i++){
        aux[solucao[i]] += pacote.vet[i];
    }


   // print(aux, pacote.n_mochilas);
    int residuo = 0;
    for(int i = 0; i < pacote.n_mochilas;i++){
        if(aux[i] > pacote.peso_max){
            residuo += aux[i] - pacote.peso_max;
        }
    }
    return residuo;
}
int* VizinhoRandom(int *solucao, packing pacote){
    int* novo = Cpy(solucao, pacote.tamanho);
    // pega o valor correspondente a 10% da qtd de elementos
    int b,  aux = (0.2*pacote.tamanho) + 1;

    //sorteia um numero entre os 10%
    aux = (rand() % aux) + 1;

    //altera o numero sorteado dentre os 10%
    for (int i = 0; i < aux;i++){

        //recebe a possição que sera alterada
        b = rand() % pacote.tamanho;

        //altera
        novo[b] = rand() % pacote.n_mochilas;
    }
    return novo;
}

int* HillClimbing(packing pacote, int n){
    int *solucao, *nova, aux, tmp;
    solucao = GeradorRandom(pacote);

    aux = Residuo(solucao, pacote);
    for(int i = 0 ; i < n; i++){
        iter++;
        nova = VizinhoRandom(solucao, pacote);

        tmp = Residuo(nova, pacote);

        if(tmp <= aux){

            solucao = nova;
            aux = tmp;

        }
    }
    cout<<"\n"<<"residuo "<<aux<<"\n";
    return solucao;
}








int main(){
    remove("saida.txt");
    srand( (unsigned)time(NULL) );
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

    //string.bin.flag e o numero de itens que ainda devem ser impacotados
    int *a = HillClimbing(pacote, 1000);
    print(a, pacote.tamanho);

    tFim = clock();

    /*calcula aproximadamente o tempo em milisegundos gasto entre as duas chamadas de clock()*/
    tDecorrido = ((tFim - tInicio) / (CLOCKS_PER_SEC / 1000));


    cout<<"numero de iteracoes: "<<iter<<"\n";
    cout<<"tempo gasto: "<<tDecorrido<<" milisegundos \n";

    SalvarSolucao("O numero minimo de pacotes é:\n");
    char str[100];
    itoa(n_mochilas, str, 10);
    SalvarSolucao(str);

}

