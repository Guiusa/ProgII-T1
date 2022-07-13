#include "utils.h"

int countArchs(char* dir) {
    //estruturas para o diretório e os arquivos
	DIR* dirLogs;
	struct dirent *entry;
	dirLogs = opendir(dir);
	if(!dirLogs){
		perror("ERRO ao abrir diretório, insira um diretório válido\n");
		exit(1);
	}
	
	int count = 0;
	//Testa todas as entradas alcansáveis pelo ponteiro do diretório, descarta os diretórios . e .. para não contar como arquivos de log
	while((entry=readdir(dirLogs))){
		if(strcmp(entry->d_name,".") && strcmp(entry->d_name,".."))
			count++;
	}
	closedir(dirLogs);
	return count;
}



run_t* runAlloc(int files) {
	//aloca espaço para n(calculado em countArchs()) arquivos do tipo run_t*
	run_t* runs = malloc(files * sizeof(run_t));
	
	return runs;
}



char** countBikes(char* dir, int* tam){
	//aloca o espaço para o vetor de strings para o nome das bikes com uma bike, tambḿe aloca espaço para a primeira string e
	//coloca um valor qualquer nela
	char** l = malloc(sizeof(char*));
	l[0] = malloc(LINESIZE * sizeof(char));
	strcpy(l[0], "");
	
	//structs padrão para diretório, entrada e arquivo
	DIR* dirLogs;
	FILE* arch;
	struct dirent* entry;
	dirLogs = opendir(dir);
	if(!dirLogs){
		perror("ERRO ao abrir o diretório");
		exit (1);
	}
	//c vai contar quantas bicicletas há no vetor, ok dirá se a bike lida deve ou não ser colocada no vetor
	int c = 0;
	int ok;
	
	//malloc temporário pra ler a primeira linha do arquivo e armazenar o "gear"
	char* bike = malloc(80 * sizeof(char));
	
	while((entry = readdir(dirLogs))){
		//ignorando as entradas dos diretórios . e ..
		if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){
			//proxLog está salvando o arquivo a se abrir na forma diretório/xxxxxxxxxxx.log
			char proxLog[LINESIZE];
			strcpy(proxLog, dir);
			strcat(proxLog, "/");
			strcat(proxLog, entry->d_name);

			arch = fopen(proxLog, "r");
			if(!arch){
				perror("Erro ao abrir arquivo de nome:\n");
				exit (1);
			}

			//lê a primeira linha do arquivo, onde deve conter a icicleta daquele log
			fgets(bike, LINESIZE, arch);
			//por padrão, ok é true porque a bicicleta deve ser coloca no vetor, no laço for ele procura por uma bicicleta igual e se
			//achar, ok recebe false
			ok = 1;
			for(int i = 0; i < c; i++){
				if(strcmp(l[i], bike)==0)
					ok = 0;
			}

			//se não houve bicicleta igual, realoca o vetor, aloca o próximo nodo e coloca o valor lido em "bike" no vetor
			if(ok){
				strcpy(l[c], bike);
				c++;
				l = realloc(l, (c+1) * sizeof(char*));
				l[c] = malloc(LINESIZE * sizeof(char));
				strcpy(l[c], "");
			}
			fclose(arch);
		}
	}
	//ponteiro recebido como parâmetro recebe o tamanho do vetor que armazenou as bikes
	*tam = c;
	
	//liberando espaço para os allocs temporários
	free(l[c]);
	free(bike);
	closedir(dirLogs);
	
	//esse laço tira a substring "Gear: " do nome de cada uma das bicicletas encontradas, faz isso deslocando todos os char 6 espaços para a esquerda
	for(int i = 0; i < c; i++)
		for(int j = 0; j < LINESIZE; j++)
			l[i][j] = l[i][j+6];
	
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
	//desaloca o vetor de strings que contêm o nome das bicicletas
	for(int i = 0; i < tam; i++)
		free(l[i]);
	free(l);
	return;
}



int runFree(run_t* runs, int tam){
	//desaloca as estrututuras do tipo run_t
	for(int i = 0; i < tam; i++){
		free(runs[i].bike);
	}
	free(runs);
	return 0;
}



int gerarLogs(run_t* runs, char* dir, int qLogs){
	char proxLog[LINESIZE];
	
	DIR* dirLogs;
	dirLogs = opendir(dir);
	if(!dirLogs){
		perror("Impossível abrir diretório");
		exit (1);
	}

	FILE* arch;
	struct dirent* entry;


	char linha[LINESIZE];
	for(int i = 0; i < qLogs; i++){
		entry = readdir(dirLogs);
		if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){
			strcpy(proxLog, dir);
			strcat(proxLog, "/");
			strcat(proxLog, entry->d_name);
			arch = fopen(proxLog, "r");

			if(!arch){
				perror("Impossível abrir arquivo log");
				exit(1);
			}
			
			int countLinhas = 0;
			while(fgets(linha, LINESIZE, arch) != NULL){
				if(countLinhas==0){
					for(int j = 0; j < LINESIZE; j++)
						linha[j] = linha[j+6];
					runs[i].bike = malloc(LINESIZE * sizeof(char));
					strcpy(runs[i].bike, linha);
				}


				countLinhas++;
			}

			fclose(arch);
		}
	}


	closedir(dirLogs);

	return 1;
}