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
    int *aux;
}packing;
#pragma pack(pop)

double iter = 0;
/////////////////////////////////////////////////////////inicializa��o/////////////////////////////////////////////////////////////////////
int* inicializar(int tamanho){
    int *novo;
    novo = new int[tamanho];
    for(int i= 0;i < tamanho; i++){
        novo[i] = -1;
    }
    return novo;
}

packing Createpacking(int tamanho, int peso){
    packing *pacote = new packing[0];

    pacote->flag = tamanho;
    pacote->tamanho = tamanho;
    pacote->peso_max = peso;

    pacote->vet = new int[tamanho];
    pacote->aux = new int[tamanho];


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
int* pop(int *buffer, int a, int tam){
    if(buffer != NULL){
        for(int i = 1; i < tam; i++){
            if (!((buffer[i] == 0)||(buffer[i]== 1))){
                buffer[i] = a;
                break;
            }
        }
        return buffer;
    }exit(0);
}



int Fitness(char *solucao, int *vet, int aux){
    int fit = 0;
    if(solucao != NULL){
        for(int i = 0; i < aux; i++){
            if(solucao[i] == '1'){
                fit += vet[i];
            }
        }

        return fit;
    }exit(0);
}

packing Atualizacao(packing pacote, int *solucao){
    int cont = 0, i, j;
    for(i = pacote.tamanho, j = 0; i > 0 ; i--, j++){
        if(solucao[i] == 1){
            pacote.vet[j] = pacote.peso_max + 1;
            cont ++;
        }
    }
    pacote.flag -= cont;

    return pacote;
}
void print(int *buffer,int tamanho){
    cout<<"\n";
    for(int i = 0;i < tamanho;i++){
        cout<<buffer[i]<<"\n";
    }
}

int* maior(int peso1, int peso2, int peso_max, int tam){
    int *aux, *tmp;

    aux = inicializar(tam+1);
    tmp = inicializar(tam+1);

    aux[0] = peso1;
    aux[1] = 1;

    tmp[0] = peso2;
    tmp[1] = 0;

    if(peso1<=peso_max){
        if(peso2<=peso_max){
            if(peso1>=peso2){
                return aux;
            }else{
                return tmp;
            }
        }else{
            return aux;
        }
    }else if(peso2<=peso_max){
        return tmp;
    }
}
int contaElementos(int *vet, int tam){
    int i, cont = 0;
    for (i = tam; i > 0; i--){
        if(vet[i] == 1){
            cont ++;
        }
    }
}
int* Branch_and_Bound(int *vet, int peso_atual, int peso_max,
                              int posicao, int tam, int tamanho){


    int *solucao_dir, *solucao_esq;
    int peso1, peso2;
    peso1 = peso2 = 0;

    solucao_dir = solucao_esq = NULL;

    peso1 = vet[posicao] + peso_atual;
    peso2 = peso_atual;

    if(tam > 1){
        if(peso1 <= peso_max){
            solucao_dir = Branch_and_Bound(vet, peso1, peso_max, posicao + 1, tam - 1, tamanho);
        }
        if(peso2 <= peso_max){
            solucao_esq = Branch_and_Bound(vet, peso2, peso_max, posicao + 1, tam - 1, tamanho);
        }
        if((solucao_dir!=NULL) && solucao_esq != NULL){
            int n_elementos_dir, n_elementos_esq;

            if(solucao_dir[0] > solucao_esq[0]){
                solucao_dir = pop(solucao_dir, 1, tam + 1);
               // print(solucao_dir, tam + 1);
                return solucao_dir;
            }else{
                solucao_esq = pop(solucao_esq, 0, tam + 1);
                //print(solucao_esq, tam + 1);
                return solucao_esq;
            }

        }else{
            if(solucao_dir == NULL){

                solucao_esq = pop(solucao_esq, 0, tam + 1);
                //print(solucao_esq, tam + 1);
                return solucao_esq;

            }else{
                solucao_dir = pop(solucao_dir, 1, tam + 1);
                //print(solucao_dir, tam + 1);
                return solucao_dir;

            }
        }
    }
    solucao_dir = maior(peso1, peso2, peso_max, tamanho + 1);
    return solucao_dir;
}


int main(){
    remove("saida.txt");
    int *solucao;

    int n_mochilas = 0;
    char argv[100];

    cout<<"informe o nome do arquivo que deseja enumerar"<<"\n";
    cin.getline(argv, 100);

    if(argv != NULL){
        char *aux = strstr(argv, ".txt");
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
        solucao = Branch_and_Bound(pacote.vet, 0, pacote.peso_max, 0, pacote.tamanho, pacote.tamanho);
        print(solucao, pacote.tamanho + 1);

        if (solucao != NULL) pacote = Atualizacao(pacote, solucao);
        n_mochilas ++;



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



