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

int countBikes(char* dir, char** l){
	char directory[50];
	strcpy(directory, dir);
	strcat(directory, "/");

	char** list = malloc(40 * sizeof(char*));
	list[0] = malloc(80 * sizeof(char));
	strcpy(list[0], " ");
	
	DIR* dirLogs;
	struct dirent* entry;
	dirLogs = opendir(dir);
	FILE* arch;

	int c = 0;
	int ok;
	
	char proxLog[80];
	char* bike = malloc(5 * sizeof(char));
	
	while((entry = readdir(dirLogs))){
		strcpy(proxLog, directory);
		
		if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){
			strcat(proxLog, entry->d_name);
			arch = fopen(proxLog, "r");
			if(!arch){
				perror("Erro ao abrir arquivo de nome:\n");
				exit (1);
			}

			fgets(bike, 80, arch);
			ok = 1;
			for(int i = 0; i < c; i++){
				if(strcmp(list[i], bike)==0)
					ok = 0;
			}
			if(ok){
				if((c+1)%5==0)
					list = realloc(list, (c+1) * sizeof(char*));
				
				strcpy(list[c], bike);
				c++;
				list[c] = malloc(80 * sizeof(char));
				strcpy(list[c], " ");
			}
		}
	}
	l = malloc(c * sizeof(char*));
	for(int i = 0; i < c; i++){
		l[i] = malloc(80 * sizeof(char));
		l[i] = list[i];
	}
	l = list;
	for(int i = 0; i < c; i++)
		free(list[i]);
	free(bike);
	free(list);
	return c;
}

run_t* runAlloc(int files) {
	run_t* runs = malloc(files * sizeof(run_t));
	return runs;
}

int imprimaBikes(char** l, int tam){
	for(int i = 0; i < tam; i++)
		printf("%s\n", l[i]);
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
