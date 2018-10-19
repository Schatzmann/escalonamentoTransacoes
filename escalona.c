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

int main(){
	t_operacao* operacoes;
	int qtdeOperacoes;

	operacoes = le_entrada(&qtdeOperacoes);

	/* Aresta Ti -> Tj para cada r(x) em Tj depois de w(x) em Ti */
	for(int i=0; i < qtdeOperacoes; i++){
		if(operacoes[i].opr == 'W'){
			for(int j=i + 1; j < qtdeOperacoes; j++){
				if(operacoes[j].opr == 'C' && operacoes[i].idT == operacoes[j].idT)
					break;
				if(operacoes[j].opr == 'R' && operacoes[i].atrib == operacoes[j].atrib && operacoes[i].idT != operacoes[j].idT){
					printf("Aresta criada de: %d -> %d \n", operacoes[i].idT, operacoes[j].idT);
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
					printf("Aresta criada de: %d -> %d \n", operacoes[i].idT, operacoes[j].idT);
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
					printf("Aresta criada de: %d -> %d \n", operacoes[i].idT, operacoes[j].idT);
				}
			}
		}
	}
}