/**
 *  @author Annelyse Schatzmann           GRR20151731
 *  @author Eduardo Zimermam Pereira      GRR20152952
 */

/**
 * @file functions.h
 * @brief Todas as funções utilizadas nos métodos.
 */

#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct t_operacao {
	int tempo;
	int idT;
	char opr;
	char atrib;
} t_operacao;

typedef struct t_equivale {
	int read;
	int write;
} t_equivale;

int contLines(FILE*);
t_operacao *leEntrada(int*);
char *verificaSerialConflito(int**, int);
char *serialConflito(t_operacao*, int, int, int**, int);
void printMatriz(int**, int);
int** alocaMatriz(int);
void encontraEscalonamento(t_operacao*, int, int*, int*);
int cmpfunc(const void *, const void *);
char *encontraIdsEscalonamento(t_operacao*, int, int, int*);
t_operacao *criaSLinha(t_operacao*, int, int, int);
int vetoresIguais(t_equivale*, t_equivale*, int);
char *equivalenciaVisao(t_operacao*, t_operacao*, int);

#endif