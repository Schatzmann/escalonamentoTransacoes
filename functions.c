/**
 * @author  Annelyse Schatzmann           GRR20151731
 * @author  Eduardo Zimermam Pereira      GRR20152952
 */

/**
 * @file functions.c
 * @brief Todas as funções utilizadas nos métodos.
 */

#include "functions.h"

/**
 * @brief Conta as linhas do arquivo passado como parâmetro.
 * @param fp    Ponteiro para um arquivo.
 * @return Um valor inteiro que é a quantidade de linhas do arquivo.
 *
 * Foi implementado para devolver a quantidade de operações do 
 * escalonamento.
 */
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

/**
 * @brief Lê a entrada através de stdin.
 * @param qtdeOperacoes    Ponteiro para um inteiro representando a quantidade de operações no escalonamento.
 * @return Um vetor de contendo todas as operações.
 *
 */
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

/**
 * @brief Método que itera uma matriz verificando seus valores.
 * @param matrizArestas    Ponteiro para ponteiros de inteiros.
 * @param tamMatriz        Inteiro com o tamanho da matriz.
 * @return Ponteiro para char com o resultado da iteração.
 *
 * O método verifica se dentro da matriz de adjacência passada como parâmetro
 * existe um ciclo, mudando o valor da váriavel ehSerial.
 * "SS" = Serial
 * "NS" = Não Serial
 */
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

/**
 * @brief Verifica as regras para um escalonamento. 
 * @param operacoes        Ponteiro para a struct t_operacao.
 * @param tempoInicio      Inteiro usado como índice.
 * @param tempoFim         Inteiro usado como índice.
 * @param matrizArestas    Ponteiro para ponteiros de inteiros.
 * @param tamMatriz        Inteiro com o tamanho da matriz.
 * @return Ponteiro para char com o resultado da iteração.
 *
 * O método é responsável por verificar regra por regra para a serialidade por conflito
 * fazendo uso do método verificarSerialConflito para retornar o resultado correto.
 */
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

/**
 * @brief Aloca uma matriz de inteiros. 
 * @param tamMatriz        Inteiro com o tamanho da matriz.
 * @return Ponteiro para ponteiro de inteiros.
 *
 */
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

/**
 * @brief Define S'. 
 * @param operacoes        Ponteiro para a struct t_operacao.
 * @param qtdeOperacoes    Inteiro com a quantidade de operações do escalonamento.
 * @param tempoInicio      Ponteiro para inteiro usado como índice.
 * @param tempoFim         Ponteiro para inteiro usado como índice.
 *
 * O método é responsável por encontrar onde um escalonamento inicia e termina. Foi
 * implementado para que pudesse separar dois escalonamento que viessem no mesmo arquivo
 * de entrada.
 */
void encontraEscalonamento(t_operacao *operacoes, int qtdeOperacoes, int *tempoInicio, int *tempoFim){
	int i;

	for (i = *tempoInicio + 1; i <= qtdeOperacoes; ++i){
		if((operacoes[i].opr == 'C') && (operacoes[i].idT == operacoes[*tempoInicio].idT)){
			*tempoFim = i;
			break;
		}
	}
}

/**
 * @brief Compara dois elementos. 
 * @param a     Elemento a ser comparado.
 * @param b     Elemento a ser comparado.
 * @return Inteiro que representa um booleano, sendo 0 falso e 1 positivo.
 *
 * Método apenas compara dois elementos, usada para fazer a ordenação das operações.
 */
int cmpfunc(const void *a, const void *b) {
   return ( *(int*)a - *(int*)b );
}

/**
 * @brief Verifica quais são as transações participantes no escalonamento. 
 * @param operacoes        Ponteiro para a struct t_operacao.
 * @param tempoInicio      Ponteiro para inteiro usado como índice.
 * @param tempoFim         Ponteiro para inteiro usado como índice.
 * @param qtdeIds          Inteiro com a quantidade de transações do escalonamento.
 * @return Ponteiro para char contendo os id's da Transação separados por vírgula.
 *
 * Método verifica cada operação de commit na transação para capturar seus id's.
 */
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

/**
 * @brief Cria um escalonamento a partir de outro. 
 * @param operacoes        Ponteiro para a struct t_operacao.
 * @param tempoInicio      Ponteiro para inteiro usado como índice.
 * @param tempoFim         Ponteiro para inteiro usado como índice.
 * @param qtdeIds          Inteiro com a quantidade de transações do escalonamento.
 * @return Um ponteiro do tipo t_operacao para o escalonamento S'. 
 *
 * Método cria um escalonamento secundário a partir do escalonamento passado como parâmetro.
 */
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

/**
 * @brief Compara dois vetores para verificar a sua igualdade. 
 * @param primeiroVetor    Ponteiro para a struct t_equivale.
 * @param SegundoVetor     Ponteiro para a struct t_equivale.
 * @param tamanho          Inteiro com o tamanho dos vetores passados como parâmetro.
 * @return Inteiro que representa um booleano, sendo 0 falso e 1 positivo.
 *
 * O método verifica se cada elemento do primeiro vetor é igual ao segundo, a igualdade
 * é verificada em termos de posição e de conteúdo.
 */
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

/**
 * @brief Aplica as regras de equivalência por visão. 
 * @param sLinha             Ponteiro para a struct t_operacao.
 * @param s                  Ponteiro para a struct t_operacao.
 * @param tamEscalonamento   Inteiro com o tamanho dos escalonamentos passados como parâmetro.
 * @return Um ponteiro para char com o resultado da iteração.
 *
 * O método verifica se os vetores de operações são iguais, baseado no retorno da função
 * vetoresIguais. Sendo o retorno os dois casos abaixo:
 * "SV" = Equivalente por Visão
 * "NV" = Não Equivalente por Visão
 */
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