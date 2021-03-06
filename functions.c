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
		if(operacoes[i].opr == 'w' || operacoes[i].opr == 'W'){
			for(int j=i + 1; j <= tempoFim; j++){
				if(operacoes[j].opr == 'c' || operacoes[j].opr == 'C')
					break;
				if((operacoes[j].opr == 'R' || operacoes[j].opr == 'r') && operacoes[i].atrib == operacoes[j].atrib && operacoes[i].idT != operacoes[j].idT){
					matrizArestas[operacoes[i].idT][operacoes[j].idT] = 1;
				}
			}
		}
	}

	/* Aresta Ti -> Tj para cada w(x) em Tj depois de r(x) em Ti */
	for(int i=tempoInicio; i <= tempoFim; i++){
		if(operacoes[i].opr == 'R' || operacoes[i].opr == 'r'){
			for(int j=i + 1; j <= tempoFim; j++){
				if(operacoes[j].opr == 'C' || operacoes[j].opr == 'c')
					break;
				if((operacoes[j].opr == 'W' || operacoes[j].opr == 'w') && operacoes[i].atrib == operacoes[j].atrib && operacoes[i].idT != operacoes[j].idT){
					matrizArestas[operacoes[i].idT][operacoes[j].idT] = 1;
				}
			}
		}
	}

	/* Aresta Ti -> Tj para cada w(x) em Tj depois de w(x) em Ti */
	for(int i=tempoInicio; i <= tempoFim; i++){
		if(operacoes[i].opr == 'W' || operacoes[i].opr == 'w'){
			for(int j=i + 1; j <= tempoFim; j++){
				if(operacoes[j].opr == 'C' || operacoes[j].opr == 'c')
					break;
				if((operacoes[j].opr == 'W' || operacoes[j].opr == 'w') && operacoes[i].atrib == operacoes[j].atrib && operacoes[i].idT != operacoes[j].idT){
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
		matriz[i] = (int*) malloc((tamMatriz + 1) * sizeof(int));
	}

	return matriz;
}

/**
 * @brief Ordenação de um vetor de inteiro pelo algoritmo de Seleção. 
 * @param vetor Ponteiro para inteiros como um vetor.
 * @param tam   Tamanho do ponteiro para inteiros.
 *
 */
void selectionSort(int* vetor, int tam){  
  int min, aux;

  for(int i = 1; i <= tam; i++){ 
    min = i; 
    for(int j = i + 1; j <= tam; j++) { 
      if(vetor[j] < vetor[min]) { 
        min = j; 
      } 
    } 
    if(i != min) { 
      aux = vetor[min]; 
      vetor[min] = vetor[i]; 
      vetor[i] = aux; 
    } 
  } 
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
char *transformaIds(int* vetorIds, int qtdeIds){
	char *idsRetorno, *aux;

	selectionSort(vetorIds, qtdeIds);

	idsRetorno = (char*) malloc(qtdeIds * sizeof(char));
	strcpy(idsRetorno, "");
	aux = (char*) malloc(sizeof(char));

	for (int i = 1; i <= qtdeIds - 1; ++i){
		sprintf(aux, "%d", vetorIds[i]);
		strcat(idsRetorno, aux);
		strcat(idsRetorno, ",");
	}

	sprintf(aux, "%d", vetorIds[qtdeIds]);
	strcat(idsRetorno, aux);

	return idsRetorno;
}

/**
 * @brief Define S'. 
 * @param operacoes        Ponteiro para a struct t_operacao.
 * @param qtdeOperacoes    Inteiro com a quantidade de operações do escalonamento.
 * @param tempoInicio      Ponteiro para inteiro usado como índice.
 * @param tempoFim         Ponteiro para inteiro usado como índice.
 * @param qtdeId           Ponteiro para inteiro para receber a quantidade de Id's no escalonamento.
 * @return Uma string com os ids já ordenados e prontos para serem concatenados ao resultado final.
 *
 * O método é responsável por encontrar onde um escalonamento inicia e termina. Foi
 * implementado para que pudesse separar dois ou mais escalonamentos que viessem no mesmo arquivo
 * de entrada.
 */
char *encontraEscalonamento(t_operacao *operacoes, int qtdeOperacoes, int *tempoInicio, int *tempoFim, int *qtdeId, int* diff){
	int i, *vetorId, tamDefault, tamRealIds, jaContemId;
	char *vetorIdString;

	jaContemId = 0;
	tamDefault = 5;
	tamRealIds = 1;

	vetorId = (int*) malloc(tamDefault * sizeof(int));

	for (i = *tempoInicio; i <= qtdeOperacoes; ++i){
		if(tamRealIds == tamDefault){
			vetorId = (int*) realloc(vetorId, tamDefault * sizeof(int));
		}

		for(int j = 1; j <= tamRealIds; ++j){
			if(operacoes[i].idT == vetorId[j]){
				jaContemId = 1;
			}
		}

		if(!jaContemId){
			vetorId[tamRealIds] = operacoes[i].idT;
			tamRealIds++;
		}

		if(operacoes[i].opr == 'c' || operacoes[i].opr == 'C'){
			tamRealIds--;
			*qtdeId = tamRealIds;
			*tempoFim = i - 1;
			*tempoFim += tamRealIds;
			break;
		}

		jaContemId = 0;
	}

	vetorIdString =  transformaIds(vetorId, *qtdeId);

	*diff = vetorId[1] - 1;

	free(vetorId);

	return vetorIdString;
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

	tamSLinha = (tempoFim - tempoInicio) - (qtdeIds - 1);

	sLinha = (t_operacao*) malloc((tamSLinha + 1) * sizeof(t_operacao));

	for (int i = 1; i <= qtdeIds; ++i){
		for(int j = tempoInicio; j <= tempoFim; ++j){
			if(operacoes[j].idT == i && operacoes[j].opr != 'c'){
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

	// printf("\n\n\n");
	// for(int i = 1; i <= tamanho; ++i){
	// 	printf("%d %d\n", primeiroVetor[i].read, primeiroVetor[i].write);
	// }

	// printf("\n\n");
	// for(int i = 1; i <= tamanho; ++i){
	// 	printf("%d %d\n", segundoVetor[i].read, segundoVetor[i].write);
	// }

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
char *equivalenciaVisao(t_operacao *sLinha, t_operacao *s, int tempoInicio, int tempoFim){
	int ehEquivalente, ultWriteS, ultWriteLinha, tamEquivS;
	t_equivale *sReads, *sLinhaReads;

	ehEquivalente = 0;
	ultWriteS = 0;
	ultWriteLinha = 0;
	tamEquivS = 1;

	for(int i = tempoFim; i >= tempoInicio; --i){
		if(s[i].opr == 'w' || s[i].opr == 'W'){
			ultWriteS = i;
			break;
		}
	}

	for(int i = (tempoFim - tempoInicio) + 1; i >= 1; --i){
		if(sLinha[i].opr == 'w' || sLinha[i].opr == 'W'){
			ultWriteLinha = i;
			break;
		}
	}

	if((s[ultWriteS].atrib == sLinha[ultWriteLinha].atrib) && (s[ultWriteS].idT == sLinha[ultWriteLinha].idT)){

		sReads = (t_equivale*) malloc(((tempoFim - tempoInicio) + 1) * sizeof(t_equivale));
		sLinhaReads = (t_equivale*) malloc(((tempoFim - tempoInicio) + 1) * sizeof(t_equivale));

		for(int i = tempoInicio; i <= tempoFim; ++i)	{
			if(s[i].opr == 'R' || s[i].opr == 'r'){
				sReads[tamEquivS].read = s[i].idT;
				for(int j = i; j >= tempoInicio; --j){
					if((s[j].opr == 'W' || s[j].opr == 'w') && (s[j].atrib == s[i].atrib)){
						sReads[tamEquivS].write = s[j].idT;
						break;
					}
	
					if((j == tempoInicio) && s[j].opr != 'w'){
						sReads[tamEquivS].write = 0;	
						break;
					}
				}
				tamEquivS++;
			}
		}

		tamEquivS = 1;
	
		for(int i = 1; i <= (tempoFim - tempoInicio) + 1; ++i)	{
			if(sLinha[i].opr == 'R' || sLinha[i].opr == 'r'){
				sLinhaReads[tamEquivS].read = sLinha[i].idT;
				for(int j = i; j >= 1; --j){
					if((sLinha[j].opr == 'W' || sLinha[j].opr == 'w') && (sLinha[j].atrib == sLinha[i].atrib)){
						sLinhaReads[tamEquivS].write = sLinha[j].idT;
						break;
					}
	
					if((j == 1) && (sLinha[j].opr != 'W' || sLinha[j].opr != 'w')){
						sLinhaReads[tamEquivS].write = 0;	
						break;
					}
				}
				tamEquivS++;
			}
		}
	
		tamEquivS--;

		selectionSortEquivVisao(sReads, tamEquivS);
		selectionSortEquivVisao(sLinhaReads, tamEquivS);

		if(vetoresIguais(sReads, sLinhaReads, tamEquivS))
			ehEquivalente = 1;

		free(sReads);
		free(sLinhaReads);
	}

	if (ehEquivalente){
		return "SV";
	} else {
		return "NV";
	}
}

/**
 * @brief Faz a alteração dos Id's das transações.  
 * @param operacoes     Ponteiro para a struct t_operacao.
 * @param tempoInicio   Inteiro representando o tempo de inicio do escalonamento.
 * @param tempoFim      Inteiro representando o tempo de fim do escalonamento.
 * @param diff          Inteiro com a diferença a ser subtraída.
 *
 * Método para alterar os id's das transações de 1 até o tamanho do escalonamento
 * para que possamos usar os id's nos indices da matriz de adjacência.
 */
void alteraIdsTransacao(t_operacao *operacoes, int tempoInicio, int tempoFim, int diff){

	for (int i = tempoInicio; i <= tempoFim; ++i){
		operacoes[i].idT -= diff;
	}
}

/**
 * @brief Atribui o valor 0 (zero) a toda a matriz. 
 * @param matriz   Ponteiro para ponteiros de inteiros representando a matriz.
 * @param tam      Inteiro com o tamanho da matriz.
 */
void zeraMatriz(int** matriz, int tam){

	for(int i = 1; i <= tam; ++i){
		for(int j = 1; j <= tam; ++j){
			matriz[i][j] = 0;
		}
	}
}

/**
 * @brief Faz a ordenação de um vetor. 
 * @param vetor      Ponteiro do tipo t_equivale representando um vetor.
 * @param tamVetor   Inteiro com o tamanho do vetor.
 */
void selectionSortEquivVisao(t_equivale* vetor, int tamVetor){
	int min; 
	t_equivale aux;

	for(int i = 1; i <= tamVetor; i++){ 
	  min = i; 
	  for(int j = i + 1; j <= tamVetor; j++) { 
	    if(vetor[j].read < vetor[min].read) { 
	      min = j; 
	    } 
	  } 
	  if(i != min) { 
	    aux.read = vetor[min].read;
	    aux.write = vetor[min].write; 
	    vetor[min].read = vetor[i].read;
	    vetor[min].write = vetor[i].write; 
	    vetor[i].read = aux.read;
	    vetor[i].write = aux.write; 
	  } 
	} 
}