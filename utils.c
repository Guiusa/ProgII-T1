#include "utils.h"

int countArchs(char* dir) {
    	DIR* dirLogs;
	struct dirent *entry;

	int count = 0;

	dirLogs = opendir(dir);
	if(!dirLogs){
		perror("ERRO ao abrir diretório, insira um diretório válido\n");
		exit(1);
	}

	while((entry=readdir(dirLogs))){
		if(strcmp(entry->d_name,".") && strcmp(entry->d_name,".."))
			count++;
	}
	closedir(dirLogs);
	return count;
}

run_t* runAlloc(int files) {
	run_t* runs = malloc(files * sizeof(run_t));
	return runs;
}

char** countBikes(char* dir, int* tam){
	char** l = malloc(sizeof(char*));
	l[0] = malloc(80 * sizeof(char));
	strcpy(l[0], "");
	
	DIR* dirLogs;
	struct dirent* entry;
	dirLogs = opendir(dir);
	FILE* arch;

	int c = 0;
	int ok;
	
	char* bike = malloc(80 * sizeof(char));
	
	while((entry = readdir(dirLogs))){
		if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){
			char proxLog[80];
			strcpy(proxLog, dir);
			strcat(proxLog, "/");
			strcat(proxLog, entry->d_name);
			arch = fopen(proxLog, "r");
			if(!arch){
				perror("Erro ao abrir arquivo de nome:\n");
				exit (1);
			}

			fgets(bike, 80, arch);
			ok = 1;
			for(int i = 0; i < c; i++){
				if(strcmp(l[i], bike)==0)
					ok = 0;
			}
			if(ok){
				strcpy(l[c], bike);
				c++;
				l = realloc(l, (c+1) * sizeof(char*));
				l[c] = malloc(80 * sizeof(char));
				strcpy(l[c], "");
			}
			fclose(arch);
		}
	}
	*tam = c;
	free(l[c]);
	free(bike);
	closedir(dirLogs);
	return l;
}

int imprimaBikes(char** l, int tam){
	printf("\n\n");
	for(int i = 0; i < tam; i++){
		printf("%d - ", i);
		printf("%s", l[i]);
	}
	return 1;
}

void bikesFree(char** l, int tam){
	for(int i = 0; i < tam; i++)
		free(l[i]);
	free(l);
	return;
}

int runFree(run_t* runs){
	free(runs);
	return 0;
}
