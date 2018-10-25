/**
 * @author  Annelyse Schatzmann           GRR20151731
 * @author  Eduardo Zimermam Pereira      GRR20152952  
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct t_operacao {
	int tempo;
	int idT;
	char opr;
	char atrib;
} t_operacao;

int cont_Lines(FILE* fp) {
	int lines = 0;
	char ch;

	while(!feof(fp)){
	  ch = fgetc(fp);
	  if(ch == '\n'){
	    lines++;
	  }
	}

	return lines;
}

t_operacao *le_entrada(int *qtdeOperacoes){
		int tamArq = 0;
		t_operacao* operacoes;

		tamArq = cont_Lines(stdin);

		operacoes = (t_operacao*) malloc((tamArq - 1) * sizeof(t_operacao));

		rewind(stdin);

		for(int i = 0; i <= tamArq; i++){
			fscanf(stdin, "%d %d %c %c", &operacoes[i].tempo, &operacoes[i].idT, &operacoes[i].opr, &operacoes[i].atrib);
		}


		*qtdeOperacoes = tamArq;
		
		return operacoes;
}

int serial_conflito(t_operacao *operacoes, int qtdeOperacoes, int **matrizArestas){

	/* Aresta Ti -> Tj para cada r(x) em Tj depois de w(x) em Ti */
	for(int i=0; i < qtdeOperacoes; i++){
		if(operacoes[i].opr == 'W'){
			for(int j=i + 1; j < qtdeOperacoes; j++){
				if(operacoes[j].opr == 'C' && operacoes[i].idT == operacoes[j].idT)
					break;
				if(operacoes[j].opr == 'R' && operacoes[i].atrib == operacoes[j].atrib && operacoes[i].idT != operacoes[j].idT){
					matrizArestas[operacoes[i].idT][operacoes[j].idT] = 1;
				}
			}
		}
	}

	/* Aresta Ti -> Tj para cada w(x) em Tj depois de r(x) em Ti */
	for(int i=0; i < qtdeOperacoes; i++){
		if(operacoes[i].opr == 'R'){
			for(int j=i + 1; j < qtdeOperacoes; j++){
				if(operacoes[j].opr == 'C' && operacoes[i].idT == operacoes[j].idT)
					break;
				if(operacoes[j].opr == 'W' && operacoes[i].atrib == operacoes[j].atrib && operacoes[i].idT != operacoes[j].idT){
					matrizArestas[operacoes[i].idT][operacoes[j].idT] = 1;
				}
			}
		}
	}

	/* Aresta Ti -> Tj para cada w(x) em Tj depois de w(x) em Ti */
	for(int i=0; i < qtdeOperacoes; i++){
		if(operacoes[i].opr == 'W'){
			for(int j=i + 1; j < qtdeOperacoes; j++){
				if(operacoes[j].opr == 'C' && operacoes[i].idT == operacoes[j].idT)
					break;
				if(operacoes[j].opr == 'W' && operacoes[i].atrib == operacoes[j].atrib && operacoes[i].idT != operacoes[j].idT){
					matrizArestas[operacoes[i].idT][operacoes[j].idT] = 1;
				}
			}
		}
	}
}

int** aloca_matriz(t_operacao *operacoes, int qtdeOperacoes, int *tamMatriz){
	int maiorTransacao = 0;
	int **matrizArestas;

	for (int i = 0; i < qtdeOperacoes; ++i){
		if(operacoes[i].idT > maiorTransacao){
			maiorTransacao = operacoes[i].idT;
		}
	}

	maiorTransacao++;

	matrizArestas = (int**) malloc(maiorTransacao * sizeof(int*));

	for (int i = 0; i < maiorTransacao; ++i){
		matrizArestas[i] = (int*) malloc(maiorTransacao * sizeof(int));
		for (int j = 0; j < maiorTransacao; ++j){
			matrizArestas[i][j] = 0;
		}
	}

	*tamMatriz = maiorTransacao;

	return matrizArestas;
}

char *verifica_serial_conflito(int **matrizArestas, int tamMatriz){
	int ehSerial = 1;

	for(int i = 0; i < tamMatriz; ++i){
		for(int j = 0; j < tamMatriz; ++j){
			if(matrizArestas[i][j] != 0){
				if(matrizArestas[j][i] != 0){
					ehSerial = 0;
				}
			}
		}
	}

	if (ehSerial){
		return "SS";
	} else {
		return "NS";
	}
}

void printMatriz(int** matrizArestas, int tamMatriz){
	for (int i = 1; i < tamMatriz; ++i){
		for (int j = 1; j < tamMatriz; ++j){
			printf("%d ", matrizArestas[i][j]);
		}
		printf("\n");
	}
}

t_operacao *divide_operacoes(t_operacao *operacoes, int qtdeOperacoes, int *indexOpr, int *tamEscalonamento, char *idsEscalona){

	t_operacao *escalonamento;

	escalonamento = (t_operacao*) malloc(4 * sizeof(t_operacao));

	escalonamento[0].tempo = 1;
	escalonamento[0].idT = 1;
	escalonamento[0].opr = 'R';
	escalonamento[0].atrib = 'X';

	escalonamento[1].tempo = 2;
	escalonamento[1].idT = 2;
	escalonamento[1].opr = 'R';
	escalonamento[1].atrib = 'X';

	escalonamento[2].tempo = 3;
	escalonamento[2].idT = 2;
	escalonamento[2].opr = 'W';
	escalonamento[2].atrib = 'X';

	escalonamento[3].tempo = 4;
	escalonamento[3].idT = 1;
	escalonamento[3].opr = 'W';
	escalonamento[3].atrib = 'X';

	*indexOpr = qtdeOperacoes;

	*tamEscalonamento = 4;

	idsEscalona = "1,2";

	return escalonamento;
}

int main(){
	t_operacao *operacoes, *escalonamento;
	int qtdeOperacoes, tamMatriz, ehSerial, indexOpr, tamEscalonamento;
	int **matrizArestas;
	char *serialConfl, *idsEscalona, *resultado, aux[10];

	operacoes = le_entrada(&qtdeOperacoes);
	indexOpr = 0;

	for(int i = 1; indexOpr < qtdeOperacoes; ++i){

		serialConfl = (char*) malloc(200 * sizeof(char));
		idsEscalona = (char*) malloc(200 * sizeof(char));
		resultado = (char*) malloc(200 * sizeof(char));

		escalonamento = divide_operacoes(operacoes, qtdeOperacoes, &indexOpr, &tamEscalonamento, idsEscalona);
	
		matrizArestas = aloca_matriz(escalonamento, tamEscalonamento, &tamMatriz);
		serial_conflito(escalonamento, tamEscalonamento, matrizArestas);
	
		serialConfl = verifica_serial_conflito(matrizArestas, tamMatriz);
		
		sprintf(aux, "%d", i);

		strcpy(resultado, aux);
		strcat(resultado, " ");
		strcat(resultado, idsEscalona);
		strcat(resultado, " ");
		strcat(resultado, serialConfl);

		printf("%s\n", resultado);

		free(serialConfl);
		free(idsEscalona);
		free(resultado);
		for (int i = 0; i < tamEscalonamento; ++i){
			free(matrizArestas[i]);
		}

		free(matrizArestas);
	}
}