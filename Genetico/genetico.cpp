

#include<iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#pragma pack(push, 1)
typedef struct _packing_{
    int peso_max, tamanho, n_mochilas;
    int  *vet;
}packing;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _individuo_{
    int fit;
    int *solucao;
}individuo;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _populacao_{
    individuo *pop;
    int tamanho;
    int fator;
}populacao;
#pragma pack(pop)



int* GeradorRandom(packing *pacote);
int Fitness(int *solucao, packing *pacote);
void print(int *buffer,int tamanho);

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


individuo CreateIndividuo(packing *pacote, int *solucao){
    individuo *novo = new individuo[0];

    novo->solucao = solucao;
    novo->fit = Fitness(novo->solucao, pacote);

    return *novo;
}

populacao* CreatePopulacao(int tamanho, packing *pacote){
    populacao *nova = new populacao[0];
    int i, *aux;

    nova->pop = new individuo[tamanho];
    nova->tamanho = tamanho;
    nova->fator = tamanho * 0.4;

    for(i = 0; i < tamanho; i++){
        aux = GeradorRandom(pacote);
        nova->pop[i] = CreateIndividuo(pacote, aux);
    }

    return nova;

}






packing* Createpacking(int tamanho, int peso, int n_mochilas){
    packing *pacote = new packing[0];

    pacote->tamanho = tamanho;
    pacote->peso_max = peso;
    pacote->n_mochilas = n_mochilas;
    pacote->vet = new int[tamanho];


    return pacote;
}
void print(int *buffer,int tamanho){
    cout<<"\n";
    if(buffer != NULL){
        for(int i = 0;i < tamanho;i++){
            cout<<buffer[i]<<" ";
        }
    }else{
        cout<<"ponteiro nulo"<<"\n";
    }
}
packing* Import_Pesos(char *arq){
    packing *novo;

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
            novo->vet[cont] = atoi(buffer);
            cont++;

            printf("%s", buffer);
		}
	}

 	fclose(input);

	return novo;

}
void Delete(populacao *pop, int tamanho){
    for(int i = 0 ; i < tamanho;i++){
        delete pop->pop[i].solucao;

    }

    delete pop->pop;

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
//////////////////////////////////////////////////////////fim inicializa��o/////////////////////////////////////////////////////////////


int* GeradorRandom(packing *pacote){
    int *nova = new int[pacote->tamanho];

    for(int i = 0; i < pacote->tamanho;i++){
        nova[i] = rand() % pacote->n_mochilas;
    }
    //print(nova, pacote->tamanho);
    return nova;
}


int* Mutacao(int *solucao, packing *pacote){
    int* novo = Cpy(solucao, pacote->tamanho);
    // pega o valor correspondente a 10% da qtd de elementos
    int b,  aux = (0.3*(pacote->tamanho)) + 1;

    //sorteia um numero entre os 10%
    aux = (rand() % aux) + 1;

    //altera o numero sorteado dentre os 10%
    for (int i = 0; i < aux;i++){

        //recebe a possição que sera alterada
        b = rand() % (pacote->tamanho);

        //altera
        novo[b] = rand() % (pacote->n_mochilas);
    }
    return novo;
}

int Fitness(int *solucao, packing *pacote){
    int *aux = inicializar(pacote->n_mochilas);


    if(solucao == NULL) return INT_MAX;
    //print(solucao, pacote->tamanho);


    for(int i = 0; i < pacote->tamanho; i++){
        aux[solucao[i]] += pacote->vet[i];
    }


   // print(aux, pacote->n_mochilas);
    int residuo = 0;
    for(int i = 0; i < pacote->n_mochilas;i++){
        if(aux[i] > pacote->peso_max){
            residuo += aux[i] - pacote->peso_max;
        }
    }
    return residuo;
}
void shellSort(individuo *vet, int Size) {
    int i , j;
    individuo value;
    int gap = 1;
    while(gap < Size) {
        gap = 3*gap+1;
    }
    while ( gap > 1) {
        gap /= 3;
        for(i = gap; i < Size; i++) {
            value = vet[i];
            j = i - gap;
            while (j >= 0 && value.fit < vet[j].fit) {
                vet [j + gap] = vet[j];
                j -= gap;
            }
            vet [j + gap] = value;
        }
    }
}

void Selecao(packing *pacote, populacao *pop){
    int i;
    shellSort(pop->pop, pop->tamanho);


    for(i = pop->fator ; i < pop->tamanho; i++){
        delete pop->pop[i].solucao;
    }

}
int* Crossover(int *a, int *b, int tamanho){
    int *novo = new int[tamanho];

    int i, aux;
    aux = rand() % 3;

    for (i = 0 ; i < tamanho ; i++){
        if(aux == 0){
            if((i % 2) == 0)novo[i] = a[i];
            else  novo[i] = b[i];
        }else if(aux == 1){
            if((i % 2) == 1)novo[i] = a[i];
            else  novo[i] = b[i];
        }else{
            if (i < (tamanho/2)) novo[i] = a[i];
            else  novo[i] = b[i];
        }
    }return novo;
}

void Genetico(packing *pacote, int tamanho, int geracoes){
    int i, *aux, j = 0, a, b, k;
    populacao *nova;
    nova = CreatePopulacao(tamanho, pacote);

    for( i = 0 ; i < geracoes ; i++){
        Selecao(pacote, nova);

        j = nova->fator;
        while(j < tamanho){
            a = rand() % (nova->fator);
            b = rand() % (nova->fator);

            aux = Crossover(nova->pop[a].solucao, nova->pop[b].solucao, pacote->tamanho);

            if((rand()%100) < 10) aux = Mutacao(aux, pacote);


            nova->pop[j] = CreateIndividuo(pacote, aux);
            j++;

        }iter++;
       // cout<<nova->pop[0].fit<<"\n";
        if (nova->pop[0].fit == 0) break;
    }
    cout<<"-------------------"<<"n";
    cout<<"Residuo: "<<nova->pop[0].fit<<"\n";
    print(nova->pop[0].solucao, pacote->tamanho);
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
    packing *pacote;
    pacote = Import_Pesos(argv);

    clock_t tInicio, tFim, tDecorrido;
    tInicio = clock();

    //string.bin.flag e o numero de itens que ainda devem ser impacotados
    Genetico(pacote, 100, 1000);

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


