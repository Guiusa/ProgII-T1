#ifndef __UTILS__
#define __UTILS__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

struct run {
	int ttl_dist;
	int avg_velo;
	int max_velo;
	int avg_hr;
	int max_hr;
	int max_cad;
	int ttl_uph;
};
typedef struct run run_t;

int countBikes(char* dir);

run_t* runAlloc(int files);
int runFree(run_t* runs);

int countArchs(char* dir);

#endif
