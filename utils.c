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

	while(!0){
		entry = readdir(dirLogs);
		if(!entry)
			break;

		if(strcmp(entry->d_name,".") && strcmp(entry->d_name,".."))
			count++;
	}
	closedir(dirLogs);
	return count;
}

int countBikes(char* dir){
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
	char* bike = malloc(sizeof(char));
	
	while((entry = readdir(dirLogs))){
		strcpy(proxLog, directory);
		
		if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){
			strcat(proxLog, entry->d_name);
			arch = fopen(proxLog, "r");
			if(!arch){
				perror("Erro ao abrir arquivo de nome:\n");
				printf("%s\n", entry->d_name);
				exit (1);
			}

			fgets(bike, 80, arch);
			ok = 1;
			for(int i = 0; i < c; i++)
				printf("%s%s\n%d\n", list[i], bike, c);
			
			printf("\n\n\n\n");
			for(int i = 0; i < c; i++){
				if(strcmp(list[i], bike)==0)
					ok = 0;
			}
			if(ok){
				strcpy(list[c], bike);
				c++;
				list = realloc(list, (c+1) * sizeof(char*));
				list[c] = malloc(80 * sizeof(char));
				strcpy(list[c], " ");
			}
		}
	}

	free(bike);
	free(list);
	return c;
}

run_t* runAlloc(int files) {
	run_t* runs = malloc(files * sizeof(run_t));
	return runs;
}

int runFree(run_t* runs){
	free(runs);
	return 0;
}
