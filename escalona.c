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

int contLines(FILE* fp) {
	int lines = 0;
	char ch;

	while(!feof(fp)){
	  ch = fgetc(fp);
	  if(ch == '\n'){
	    lines++;
	  }
	}

	lines++;

	return lines;
}

t_operacao *leEntrada(int *qtdeOperacoes){
		int tamArq = 0;
		t_operacao* operacoes;

		tamArq = contLines(stdin);

		operacoes = (t_operacao*) malloc((tamArq + 1) * sizeof(t_operacao));

		rewind(stdin);

		for(int i = 1; i <= tamArq; i++){
			fscanf(stdin, "%d %d %c %c", &operacoes[i].tempo, &operacoes[i].idT, &operacoes[i].opr, &operacoes[i].atrib);
		}

		*qtdeOperacoes = tamArq;
		
		return operacoes;
}

// int serial_conflito(t_operacao *operacoes, int qtdeOperacoes, int **matrizArestas){

// 	/* Aresta Ti -> Tj para cada r(x) em Tj depois de w(x) em Ti */
// 	for(int i=0; i < qtdeOperacoes; i++){
// 		if(operacoes[i].opr == 'W'){
// 			for(int j=i + 1; j < qtdeOperacoes; j++){
// 				if(operacoes[j].opr == 'C' && operacoes[i].idT == operacoes[j].idT)
// 					break;
// 				if(operacoes[j].opr == 'R' && operacoes[i].atrib == operacoes[j].atrib && operacoes[i].idT != operacoes[j].idT){
// 					matrizArestas[operacoes[i].idT][operacoes[j].idT] = 1;
// 				}
// 			}
// 		}
// 	}

// 	/* Aresta Ti -> Tj para cada w(x) em Tj depois de r(x) em Ti */
// 	for(int i=0; i < qtdeOperacoes; i++){
// 		if(operacoes[i].opr == 'R'){
// 			for(int j=i + 1; j < qtdeOperacoes; j++){
// 				if(operacoes[j].opr == 'C' && operacoes[i].idT == operacoes[j].idT)
// 					break;
// 				if(operacoes[j].opr == 'W' && operacoes[i].atrib == operacoes[j].atrib && operacoes[i].idT != operacoes[j].idT){
// 					matrizArestas[operacoes[i].idT][operacoes[j].idT] = 1;
// 				}
// 			}
// 		}
// 	}

// 	/* Aresta Ti -> Tj para cada w(x) em Tj depois de w(x) em Ti */
// 	for(int i=0; i < qtdeOperacoes; i++){
// 		if(operacoes[i].opr == 'W'){
// 			for(int j=i + 1; j < qtdeOperacoes; j++){
// 				if(operacoes[j].opr == 'C' && operacoes[i].idT == operacoes[j].idT)
// 					break;
// 				if(operacoes[j].opr == 'W' && operacoes[i].atrib == operacoes[j].atrib && operacoes[i].idT != operacoes[j].idT){
// 					matrizArestas[operacoes[i].idT][operacoes[j].idT] = 1;
// 				}
// 			}
// 		}
// 	}
// }

// int** aloca_matriz(t_operacao *operacoes, int qtdeOperacoes, int *tamMatriz){
// 	int maiorTransacao = 0;
// 	int **matrizArestas;

// 	for (int i = 0; i < qtdeOperacoes; ++i){
// 		if(operacoes[i].idT > maiorTransacao){
// 			maiorTransacao = operacoes[i].idT;
// 		}
// 	}

// 	maiorTransacao++;

// 	matrizArestas = (int**) malloc(maiorTransacao * sizeof(int*));

// 	for (int i = 0; i < maiorTransacao; ++i){
// 		matrizArestas[i] = (int*) malloc(maiorTransacao * sizeof(int));
// 		for (int j = 0; j < maiorTransacao; ++j){
// 			matrizArestas[i][j] = 0;
// 		}
// 	}

// 	*tamMatriz = maiorTransacao;

// 	return matrizArestas;
// }

// char *verifica_serial_conflito(int **matrizArestas, int tamMatriz){
// 	int ehSerial = 1;

// 	for(int i = 0; i < tamMatriz; ++i){
// 		for(int j = 0; j < tamMatriz; ++j){
// 			if(matrizArestas[i][j] != 0){
// 				if(matrizArestas[j][i] != 0){
// 					ehSerial = 0;
// 				}
// 			}
// 		}
// 	}

// 	if (ehSerial){
// 		return "SS";
// 	} else {
// 		return "NS";
// 	}
// }

// void printMatriz(int** matrizArestas, int tamMatriz){
// 	for (int i = 1; i < tamMatriz; ++i){
// 		for (int j = 1; j < tamMatriz; ++j){
// 			printf("%d ", matrizArestas[i][j]);
// 		}
// 		printf("\n");
// 	}
// }

void encontraEscalonamento(t_operacao *operacoes, int qtdeOperacoes, int *tempoInicio, int *tempoFim){
	int i;

	for (i = *tempoInicio + 1; i <= qtdeOperacoes; ++i){
		if((operacoes[i].opr == 'C') && (operacoes[i].idT == operacoes[*tempoInicio].idT)){
			*tempoFim = i;
			break;
		}
	}
}

int cmpfunc(const void *a, const void *b) {
   return ( *(int*)a - *(int*)b );
}

char *encontraIdsEscalonamento(t_operacao *operacoes, int tempoInicio, int tempoFim){
	int *ids, qtdeIds;
	char *idsRetorno, *aux;

	ids = (int*) malloc(sizeof(int));
	qtdeIds = 0;

	for(int i = tempoInicio; i <= tempoFim; ++i){
		if(operacoes[i].opr == 'C'){
			ids[qtdeIds] = operacoes[i].idT;
			qtdeIds++;
			ids = (int*) realloc(ids, sizeof(int));
		}
	}

	qsort(ids, qtdeIds, sizeof(int), cmpfunc);

	idsRetorno = (char*) malloc(qtdeIds * sizeof(char));
	aux = (char*) malloc(sizeof(char));

	for (int i = 0; i < qtdeIds - 1; ++i){
		sprintf(aux, "%d", ids[i]);
		strcat(idsRetorno, aux);
		strcat(idsRetorno, ",");
	}

	sprintf(aux, "%d", ids[qtdeIds - 1]);
	strcat(idsRetorno, aux);

	return idsRetorno;
}

int main(){
	t_operacao *operacoes;
	int qtdeOperacoes, tamMatriz, ehSerial, indexOpr, tamEscalonamento, numEscalonamento;
	int **matrizArestas, tempoInicio, tempoFim;
	char *serialConfl, *idsEscalona, *resultado, *aux;

	operacoes = leEntrada(&qtdeOperacoes);
	tempoInicio = 1;
	tempoFim = qtdeOperacoes;
	resultado = (char *) malloc(sizeof(char));
	aux = (char *) malloc(sizeof(char));
	numEscalonamento = 1;

	for (int i = tempoInicio; i < qtdeOperacoes; i += tempoFim){
		encontraEscalonamento(operacoes, qtdeOperacoes, &tempoInicio, &tempoFim);

		aux = encontraIdsEscalonamento(operacoes, tempoInicio, tempoFim);


		sprintf(resultado, "%d", numEscalonamento);
		strcat(resultado, " ");
		strcat(resultado, aux);

		printf("%s\n", resultado);



		numEscalonamento++;
		tempoInicio = tempoFim + 1;
	}


	// indexOpr = 0;

	// for(int i = 1; indexOpr < qtdeOperacoes; ++i){

	// 	serialConfl = (char*) malloc(200 * sizeof(char));
	// 	idsEscalona = (char*) malloc(200 * sizeof(char));
	// 	resultado = (char*) malloc(200 * sizeof(char));

	// 	escalonamento = 

	// 	matrizArestas = aloca_matriz(escalonamento, tamEscalonamento, &tamMatriz);
	// 	serial_conflito(escalonamento, tamEscalonamento, matrizArestas);
	
	// 	serialConfl = verifica_serial_conflito(matrizArestas, tamMatriz);
		
	// 	sprintf(aux, "%d", i);

	// 	strcpy(resultado, aux);
	// 	strcat(resultado, " ");
	// 	strcat(resultado, idsEscalona);
	// 	strcat(resultado, " ");
	// 	strcat(resultado, serialConfl);

	// 	printf("%s\n", resultado);

	// 	free(resultado);
	// 	free(idsEscalona);
	// 	for (int i = 0; i < tamEscalonamento - 1; ++i){
	// 		free(matrizArestas[i]);
	// 	}

	// 	free(matrizArestas);
	// }
}