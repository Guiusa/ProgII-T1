#ifndef __UTILS__
#define __UTILS__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#define LINESIZE 80

struct run {
	char* bike;
	char* date;

	double ttl_dist;
	double avg_velo;
	double max_velo;
	double avg_hr;
	double max_hr;
	double ttl_uph;
};

typedef struct run run_t;

char** countBikes(char* dir, int* tam);

void imprimaBikes(char** l, int tam);
void imprimaRun(run_t run);

void ordenaDist(run_t* runs, int tam);
void ordenaSubida(run_t* runs, int tam);

void bikesFree(char** l, int tam);
run_t* runAlloc(int files);
int runFree(run_t* runs, int tam);

void summary(run_t* runs, int tam);

void histograma(run_t* runs, int tam, char* bike);

int countArchs(char* dir);
int gerarLogs(run_t* runs, char* dir, int qLogs);
int separaAtributos(run_t* runs, char* atributo, double valor, int i, double* cadAux, double* avgHRAux, double* avgVeloAux);

void imprimeRunsBikeX(char* bike, run_t* runs, int tam);

#endif
