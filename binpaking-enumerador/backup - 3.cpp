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
    char *aux;
}packing;
#pragma pack(pop)

double iter = 0;
/////////////////////////////////////////////////////////inicializa��o/////////////////////////////////////////////////////////////////////
char* inicializar(int tamanho){
    char *novo;
    novo = new char[tamanho];
    for(int i= 0;i < tamanho; i++){
        novo[i] = '0';
    }
    return novo;
}

packing Createpacking(int tamanho, int peso){
    packing *pacote = new packing[0];

    pacote->flag = tamanho;
    pacote->tamanho = tamanho;
    pacote->peso_max = peso;

    pacote->vet = new int[tamanho];
    pacote->aux = inicializar(tamanho);


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


char* Incremento_bin(char *buffer, int tamanho){
    int i = 0;

    while ((buffer[i] == '1')&&(i < tamanho)){
        buffer[i] = '0';
        if(i < tamanho - 1) i++;
    }
    buffer[i] = '1';
    iter ++;
    return buffer;
}

int* Fitness(char *solucao, int *vet, int aux){
    int fit = 0, cont = 0;
    int *tmp = new int[2];

    for(int i = 0; i < aux; i++){
        if(solucao[i] == '1'){
            fit += vet[i];
            cont++;
        }
    }
    tmp[0] = cont;
    tmp[1] = fit;

    return tmp;
}

packing Atualizacao(packing pacote, char *solucao){
    int cont = 0;
    for(int i = 0; i < pacote.tamanho ; i++){
        if(solucao[i] == '1'){
            pacote.vet[i] = pacote.peso_max + 1;
            cont ++;
        }
    }
    pacote.flag -= cont;

    return pacote;
}
void print(char *buffer,int tamanho){
    cout<<"\n";
    for(int i = 0;i < tamanho;i++){
        cout<<buffer[i];
    }
}

packing Enumera(packing pacote){
    int *peso_atual, peso_max = 0, n_itens_max = pacote.tamanho, n_itens_atual = pacote.tamanho;

    double Enum = pow(2, pacote.tamanho);
    char buffer[strlen(pacote.aux)];

    for(double k = 0; k < Enum ; k++){
        //incrementa +1 na string binaria
        pacote.aux = Incremento_bin(pacote.aux, pacote.tamanho);

        peso_atual = Fitness(pacote.aux, pacote.vet, pacote.tamanho);

        n_itens_atual = peso_atual[0];

        //solução com mais elementos
        if((peso_atual[1] >= peso_max)&& (peso_atual[1] <= pacote.peso_max)){

            if(peso_atual[1] == peso_max){

                if(n_itens_atual < n_itens_max){

                    n_itens_max = n_itens_atual;
                    peso_max = peso_atual[1];
                    strcpy(buffer , pacote.aux);
                }
            }else{

                peso_max = peso_atual[1];
                strcpy(buffer , pacote.aux);
                n_itens_max = peso_atual[0];

            }

        }
        //if(peso_max == pacote.peso_max) break;

    }
    SalvarSolucao(buffer);

    print(buffer,  pacote.tamanho);
    cout<<"\n";
    pacote = Atualizacao(pacote, buffer);
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

    //string.bin.flag e o numero de itens que ainda devem ser impacotados
    while (pacote.flag != 0){
        pacote = Enumera(pacote);
        n_mochilas++;
        delete pacote.aux;
        pacote.aux = inicializar(pacote.tamanho);

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

