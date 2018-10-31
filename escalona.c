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
	int **matrizArestas, tempoInicio, tempoFim, qtdeCommits, qtdeOperacoes, numEscalonamento;
	char *serialConfl, *idsEscalona, *resultado, *aux, *eqVisao;

	operacoes = leEntrada(&qtdeOperacoes);
	tempoInicio = 1;
	tempoFim = qtdeOperacoes;
	resultado = (char *) malloc(sizeof(char));
	numEscalonamento = 1;

	for (int i = tempoInicio; i < qtdeOperacoes; i += tempoFim){
		encontraEscalonamento(operacoes, qtdeOperacoes, &tempoInicio, &tempoFim);

		aux = encontraIdsEscalonamento(operacoes, tempoInicio, tempoFim, &qtdeCommits);

		matrizArestas = alocaMatriz(qtdeCommits);

		serialConfl = serialConflito(operacoes,tempoInicio, tempoFim, matrizArestas, qtdeCommits);

		sprintf(resultado, "%d", numEscalonamento);
		strcat(resultado, " ");
		strcat(resultado, aux);
		strcat(resultado, " ");
		strcat(resultado, serialConfl);


		for(int i = 1; i <= qtdeCommits; ++i){
			free(matrizArestas[i]);
		}

		free(matrizArestas);

		numEscalonamento++;


		sLinha = criaSLinha(operacoes, tempoInicio, tempoFim, qtdeCommits);
		qtdeOperacoes = tempoFim - qtdeCommits;

		eqVisao = equivalenciaVisao(sLinha, operacoes, qtdeOperacoes);
		
		strcat(resultado, " ");
		strcat(resultado, eqVisao);


		tempoInicio = tempoFim + 1;

		printf("%s\n", resultado);
	}
}