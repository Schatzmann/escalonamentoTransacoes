/**
 * @author  Annelyse Schatzmann           GRR20151731
 * @author  Eduardo Zimermam Pereira      GRR20152952  
*/

#include "functions.h"

/**
 * @file escalona.c
 * @brief Programa principal.
 *
 * A função principal tem o objetivo de fazer a chamada de todas as funções e organizar os
 * parâmetros de cada função para que sejam executadas de forma correta e tenham o retorno
 * esperado.
 *
 *O resultado final é todo concatenado nesta função para ser exibido ao usuário, sendo de
 *monstrado em stdout.
*/
int main(){
	t_operacao *operacoes, *sLinha;
	int **matrizArestas, tempoInicio, tempoFim, qtdeId, qtdeOperacoes, numEscalonamento, diff;
	char *serialConfl, *idsEscalona, *resultado, *aux, *eqVisao;

	operacoes = leEntrada(&qtdeOperacoes);
	matrizArestas = alocaMatriz(qtdeOperacoes);
	zeraMatriz(matrizArestas, qtdeOperacoes);
	tempoInicio = 1;
	tempoFim = qtdeOperacoes;
	resultado = (char *) malloc(sizeof(char));
	numEscalonamento = 1;
	qtdeId = 0;
	diff = 0;

	for (int i = tempoInicio; i < qtdeOperacoes; i = tempoFim){
		idsEscalona = encontraEscalonamento(operacoes, qtdeOperacoes, &tempoInicio, &tempoFim, &qtdeId, &diff);

		alteraIdsTransacao(operacoes, tempoInicio, tempoFim, diff);

		serialConfl = serialConflito(operacoes, tempoInicio, tempoFim, matrizArestas, qtdeId);

		zeraMatriz(matrizArestas, qtdeId);

		sprintf(resultado, "%d", numEscalonamento);
		strcat(resultado, " ");
		strcat(resultado, idsEscalona);
		strcat(resultado, " ");
		strcat(resultado, serialConfl);

		numEscalonamento++;

		sLinha = criaSLinha(operacoes, tempoInicio, tempoFim, qtdeId);

		// qtdeOperacoes = tempoFim - qtdeCommits;

		// eqVisao = equivalenciaVisao(sLinha, operacoes, qtdeOperacoes);
		
		// strcat(resultado, " ");
		// strcat(resultado, eqVisao);

		// printf("%s\n", resultado);

		free(idsEscalona);
		tempoInicio = tempoFim + 1;
	}

}