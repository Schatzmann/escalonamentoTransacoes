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

int le_entrada(){
	FILE *arq;

	arq = fopen("teste.in", "r");
	
	if(arq == NULL){
		perror ("Erro ao abrir arquivo \n");
		exit (1);
	} else {
		t_operacao* operacoes;
		int tamArq = 0;

		tamArq = cont_Lines(arq);

		printf("%d\n", tamArq);

		operacoes = (t_operacao*) malloc((tamArq - 1) * sizeof(t_operacao));

		rewind(arq);

		for(int i = 0; i <= tamArq; i++){
			fscanf(arq, "%d %d %c %c", &operacoes[i].tempo, &operacoes[i].idT, &operacoes[i].opr, &operacoes[i].atrib);
		}

		printf("%d %d %c %c", operacoes[20].tempo, operacoes[20].idT, operacoes[20].opr, operacoes[20].atrib);
	}

	fclose(arq);
	
	return 0;
}

// void escreveSaida(char* arqSaida){
// 	FILE *arq;

// 	arq = fopen(arqSaida, "w");

// 	if(!arq){
// 		printf("ERRO: erro ao abrir arquivo de saída.\n");

// 	} else {

// 		fprintf(arq, "# ans15  Annelyse Schatzmann\n");
// 		fprintf(arq, "# ezp15  Eduardo Zimermam Pereira\n");

// 		fprintf(arq, "# \n");


// 		fclose(arq);
// 	}
// }



int main(){
	le_entrada();
	//escreveSaida(argv[]);
}