/**
 * @author  Annelyse Schatzmann           GRR20151731
 * @author  Eduardo Zimermam Pereira      GRR20152952
 */

/**
 * @file functions.c
 * @brief Todas as funções utilizadas nos métodos.
 */

#include "functions.h"

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

char *verificaSerialConflito(int **matrizArestas, int tamMatriz){
	int ehSerial = 1;

	for(int i = 1; i <= tamMatriz; ++i){
		for(int j = 1; j <= tamMatriz; ++j){
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

char *serialConflito(t_operacao *operacoes, int tempoInicio, int tempoFim, int **matrizArestas, int tamMatriz){

	/* Aresta Ti -> Tj para cada r(x) em Tj depois de w(x) em Ti */
	for(int i=tempoInicio; i <= tempoFim; i++){
		if(operacoes[i].opr == 'W'){
			for(int j=i + 1; j <= tempoFim; j++){
				if(operacoes[j].opr == 'C' && operacoes[i].idT == operacoes[j].idT)
					break;
				if(operacoes[j].opr == 'R' && operacoes[i].atrib == operacoes[j].atrib && operacoes[i].idT != operacoes[j].idT){
					matrizArestas[operacoes[i].idT][operacoes[j].idT] = 1;
				}
			}
		}
	}

	/* Aresta Ti -> Tj para cada w(x) em Tj depois de r(x) em Ti */
	for(int i=tempoInicio; i <= tempoFim; i++){
		if(operacoes[i].opr == 'R'){
			for(int j=i + 1; j <= tempoFim; j++){
				if(operacoes[j].opr == 'C' && operacoes[i].idT == operacoes[j].idT)
					break;
				if(operacoes[j].opr == 'W' && operacoes[i].atrib == operacoes[j].atrib && operacoes[i].idT != operacoes[j].idT){
					matrizArestas[operacoes[i].idT][operacoes[j].idT] = 1;
				}
			}
		}
	}

	/* Aresta Ti -> Tj para cada w(x) em Tj depois de w(x) em Ti */
	for(int i=tempoInicio; i <= tempoFim; i++){
		if(operacoes[i].opr == 'W'){
			for(int j=i + 1; j <= tempoFim; j++){
				if(operacoes[j].opr == 'C' && operacoes[i].idT == operacoes[j].idT)
					break;
				if(operacoes[j].opr == 'W' && operacoes[i].atrib == operacoes[j].atrib && operacoes[i].idT != operacoes[j].idT){
					matrizArestas[operacoes[i].idT][operacoes[j].idT] = 1;
				}
			}
		}
	}

	return verificaSerialConflito(matrizArestas, tamMatriz);
}

void printMatriz(int** matrizArestas, int tamMatriz){
	for (int i = 1; i <= tamMatriz; ++i){
		for (int j = 1; j <= tamMatriz; ++j){
			printf("%d ", matrizArestas[i][j]);
		}
		printf("\n");
	}
}

int** alocaMatriz(int tamMatriz){
	int **matriz;

	matriz = (int**) malloc((tamMatriz + 1) * sizeof(int*));

	for (int i = 1; i <= tamMatriz; ++i){
		matriz[i] = (int*) malloc(tamMatriz * sizeof(int));
		for (int j = 1; j <= tamMatriz; ++j){
			matriz[i][j] = 0;
		}
	}

	return matriz;
}

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

char *encontraIdsEscalonamento(t_operacao *operacoes, int tempoInicio, int tempoFim, int *qtdeIds){
	int *ids;
	char *idsRetorno, *aux;
	int auxQtdeIds;

	ids = (int*) malloc(sizeof(int));
	auxQtdeIds = 0;

	for(int i = tempoInicio; i <= tempoFim; ++i){
		if(operacoes[i].opr == 'C'){
			ids[auxQtdeIds] = operacoes[i].idT;
			auxQtdeIds++;
			ids = (int*) realloc(ids, sizeof(int));
		}
	}

	qsort(ids, auxQtdeIds, sizeof(int), cmpfunc);

	idsRetorno = (char*) malloc(auxQtdeIds * sizeof(char));
	strcpy(idsRetorno, "");
	aux = (char*) malloc(sizeof(char));

	for (int i = 0; i < auxQtdeIds - 1; ++i){
		sprintf(aux, "%d", ids[i]);
		strcat(idsRetorno, aux);
		strcat(idsRetorno, ",");
	}

	sprintf(aux, "%d", ids[auxQtdeIds - 1]);
	strcat(idsRetorno, aux);

	free(ids);
	free(aux);

	*qtdeIds = auxQtdeIds;

	return idsRetorno;
}

t_operacao *criaSLinha(t_operacao *operacoes, int tempoInicio, int tempoFim, int qtdeIds){
	t_operacao *sLinha;
	int tamSLinha, index;

	index = 1;

	tamSLinha = tempoFim - qtdeIds;

	sLinha = (t_operacao*) malloc((tamSLinha + 1) * sizeof(t_operacao));

	for (int i = 1; i <= qtdeIds; ++i){
		for(int j = tempoInicio; j <= tempoFim; ++j){
			if(operacoes[j].idT == i && operacoes[j].opr != 'C'){
				sLinha[index].idT = operacoes[j].idT;
				strcpy(&sLinha[index].opr, &operacoes[j].opr);
				strcpy(&sLinha[index].atrib, &operacoes[j].atrib);
				sLinha[index].tempo = index;
				index++;
			}
		}
	}

	return sLinha;
}

int vetoresIguais(t_equivale *primeiroVetor, t_equivale *segundoVetor, int tamanho){
	int ehIgual = 1;

	for(int i = 1; i <= tamanho; ++i){
		if((primeiroVetor[i].read != segundoVetor[i].read) || (primeiroVetor[i].write != segundoVetor[i].write)){
			ehIgual = 0;
			break;
		}
	}

	return ehIgual;
}

char *equivalenciaVisao(t_operacao *sLinha, t_operacao *s, int tamEscalonamento){
	int ehEquivalente, ultWriteS, ultWriteLinha, tamEquivS;
	t_equivale *sReads, *sLinhaReads;

	ehEquivalente = 0;
	ultWriteS = 0;
	ultWriteLinha = 0;
	tamEquivS = 1;

	sReads = (t_equivale*) malloc(tamEscalonamento * sizeof(t_equivale));
	sLinhaReads = (t_equivale*) malloc(tamEscalonamento * sizeof(t_equivale));

	for(int i = tamEscalonamento; i >= 1; --i){
		if(s[i].opr == 'W'){
			ultWriteS = i;
			break;
		}
	}

	for(int i = tamEscalonamento; i >= 1; --i){
		if(sLinha[i].opr == 'W'){
			ultWriteLinha = i;
			break;
		}
	}

	if((s[ultWriteS].atrib == sLinha[ultWriteLinha].atrib) && (s[ultWriteS].idT == sLinha[ultWriteLinha].idT)){

		for(int i = 1; i <= tamEscalonamento; ++i)	{
			if(s[i].opr == 'R'){
				sReads[tamEquivS].read = s[i].idT;
				for(int j = i; j > 0; --j){
					if((s[j].opr == 'W') && (s[j].atrib == s[i].atrib)){
						sReads[tamEquivS].write = s[j].idT;
					}
	
					if((j == 1) && (s[j].opr != 'W')){
						sReads[tamEquivS].write = 0;	
					}
				}
				tamEquivS++;
			}
		}

		tamEquivS = 1;
	
		for(int i = 1; i <= tamEscalonamento; ++i)	{
			if(sLinha[i].opr == 'R'){
				sLinhaReads[tamEquivS].read = sLinha[i].idT;
				for(int j = i; j > 0; --j){
					if((sLinha[j].opr == 'W') && (sLinha[j].atrib == sLinha[i].atrib)){
						sLinhaReads[tamEquivS].write = sLinha[j].idT;
					}
	
					if((j == 1) && (sLinha[j].opr != 'W')){
						sLinhaReads[tamEquivS].write = 0;	
					}
				}
				tamEquivS++;
			}
		}
	
		tamEquivS--;
	
		if(vetoresIguais(sReads, sLinhaReads, tamEquivS))
			ehEquivalente = 1;
	}


	if (ehEquivalente){
		return "SV";
	} else {
		return "NV";
	}
}