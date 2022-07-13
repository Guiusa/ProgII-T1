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

	int ttl_dist;
	int avg_velo;
	int max_velo;
	int avg_hr;
	int max_hr;
	int max_cad;
	int ttl_uph;
};

typedef struct run run_t;

char** countBikes(char* dir, int* tam);

void imprimaBikes(char** l, int tam);
void imprimaRuns(run_t* runs, int tam);

void bikesFree(char** l, int tam);
run_t* runAlloc(int files);
int runFree(run_t* runs, int tam);

int countArchs(char* dir);
int gerarLogs(run_t* runs, char* dir, int qLogs);

#endif
