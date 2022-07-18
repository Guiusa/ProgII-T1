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
	for(int i = 0; i < files; i++){
		runs[i].bike = malloc(LINESIZE * sizeof(char));
		runs[i].date = malloc(LINESIZE * sizeof(char));
	}
	return runs;
}



char** countBikes(char* dir, int* tam){
	//aloca o espaço para o vetor de strings para o nome das bikes com uma bike, tambḿe aloca espaço para a primeira string e
	//coloca um valor qualquer nela
	char** l = malloc(sizeof(char*));
	l[0] = malloc(LINESIZE * sizeof(char));
	
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
	char* bike = malloc(LINESIZE * sizeof(char));
	
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
			for(int i = 0; i<LINESIZE; i++){
					if(bike[i] == '\n'){
						bike[i] = '\0';
						break;
					}
			}
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
		for(int j = 0; j < LINESIZE-6; j++)
			l[i][j] = l[i][j+6];
	
	return l;
}



void ordenaDist(run_t* runs, int tam){
	run_t aux;

	for(int i = 0; i < tam - 1; i++){
		int menor = i;
		for(int j = i + 1; j < tam; j++){
			if(runs[j].ttl_dist < runs[menor].ttl_dist)
				menor = j;
		}
		aux = runs[i];
		runs[i] = runs[menor];
		runs[menor] = aux;
	}
}


void ordenaSubida(run_t* runs, int tam){
	run_t aux;

	for(int i = 0; i < tam - 1; i++){
		int menor = i;
		for(int j = i + 1; j < tam; j++){
			if(runs[j].ttl_uph < runs[menor].ttl_uph)
				menor = j;
		}
		aux = runs[i];
		runs[i] = runs[menor];
		runs[menor] = aux;
	}
}




void imprimaBikes(char** l, int tam){
	printf("\n\n");
	for(int i = 0; i < tam; i++){
		printf("%d - ", i);
		printf("%s\n", l[i]);
	}
}


void imprimeRunsBikeX(char* bike, run_t* runs, int tam){
	printf("| DATE | BIKE | DISTANCIA  | MAX VEL | MAX HR | TOTAL SUB | HR MÉDIA | VEL MÉDIA\n");
	for(int i = 0; i < tam; i++){
		if(!strcmp(bike, runs[i].bike)){
			imprimaRun(runs[i]);
		}
	}
}



void summary(run_t* runs, int tam){
	printf("\n\n SUMÁRIO FINAL:\n");
	printf("Foram encontradas %d atividades\n", tam);
	double maiorDist = 0;
	double menorDist = 100000;
	double somaDist = 0;
	for(int i = 0; i<tam; i++){
		if(runs[i].ttl_dist > maiorDist)
			maiorDist = runs[i].ttl_dist;
		if(runs[i].ttl_dist < menorDist)
			menorDist = runs[i].ttl_dist;
		somaDist += runs[i].ttl_dist;
	}
	printf("A maior atividade em distância foi de %.1fkm\n", maiorDist);
	printf("A menor atividade em distância foi de %.1fkm\n", menorDist);
	printf("O total percorrido em todas as atividades foi de %.1fkm\n", somaDist);
	printf("A média de distância dentre as atividades foi de %.1fkm\n", somaDist/tam);
}



void histograma(run_t* runs, int tam, char* bike){
	int v[11];
	for(int i = 0; i<11; i++)
		v[i] = 0;
	for(int i = 0; i<tam; i++){
		if(!strcmp(runs[i].bike, bike)){
			int parcial = runs[i].ttl_dist-20;
			v[abs(parcial/10)] += 1;
		}
	}
	for(int i = 0; i<11; i++){
		if(i < 8){
			printf("  %d - %d |", 10*i+20, 10*i+29);
		}
		else{
			printf("%d - %d |", 10*i+20, 10*i+29);
		}
		for(int j = 0; j<v[i]; j++)
			printf("*");
		printf("\n");
	}

}



void imprimaRun(run_t run){
		printf("%s", run.date);
		printf(" | ");
		printf("%s", run.bike);
		printf(" | ");
		printf("%.2f", run.ttl_dist);
		printf(" | ");
		printf("%.2f", run.max_velo);
		printf(" | ");
		printf("%.0f", run.max_hr);
		printf(" | ");
		printf("%.2f", run.ttl_uph);
		printf(" | ");
		printf("%.2f", run.avg_hr);
		printf(" | ");
		printf("%.2f", run.avg_velo);

		printf("\n");
}



int separaAtributos(run_t* runs, char* atributo, double valor, int i, double* cadAux, double* avgHRAux, double* avgVeloAux){
	if(!strcmp(atributo, "distance")){
		runs[i].ttl_dist = valor/1000;
	}
	else if(!strcmp(atributo, "speed")){
		runs[i].max_velo = (valor*3.6 > runs[i].max_velo) ? valor * 3.6 : runs[i].max_velo;
		runs[i].avg_velo += valor*3.6;
		*avgVeloAux+=1;
	}
	else if(!strcmp(atributo, "heart_rate")){
		runs[i].max_hr = (valor > runs[i].max_hr) ? valor : runs[i].max_hr;
		runs[i].avg_hr += valor;
		*avgHRAux+=1;
	}
	else if(!strcmp(atributo, "altitude")){
		if(*cadAux != -1)
			runs[i].ttl_uph = (valor > *cadAux) ? runs[i].ttl_uph + (valor-*cadAux) : runs[i].ttl_uph;
		*cadAux = valor;	
	}
	return 1;
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


	char* linha = malloc(LINESIZE * sizeof(char));
	int i = 0;
	while(i < qLogs){

		entry = readdir(dirLogs);
		if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){
			runs[i].max_velo = 0; 
			runs[i].max_hr = 0;
			runs[i].ttl_uph = 0;
			runs[i].avg_velo = 0;
			runs[i].avg_hr = 0;
			double avgHRAux = 0;
			double avgVeloAux = 0;
			double cadAux = -1;
			
			
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
					
					for(int j = 0; j < LINESIZE-6; j++)
						linha[j] = linha[j+6];
					for(int k = 0; k < LINESIZE-6; k++){
						if(linha[k] == '\n'){
							linha[k] = '\0';
							break;
						}
					}
					strcpy(runs[i].bike, linha);
				}
				
				else if(countLinhas==1){
					if(linha[11] == ','){
						runs[i].date[0] = '0';
						runs[i].date[1] = linha[10];
						runs[i].date[2] = '/';
						for(int j = 0; j < 3; j++)
							runs[i].date[3+j] = linha[6+j];
						runs[i].date[6] = '/';
						for(int j = 0; j < 4; j++)
							runs[i].date[7+j] = linha[13+j];
					}

					else{
						runs[i].date[0] = linha[10];
						runs[i].date[1] = linha[11];
						runs[i].date[2] = '/';
						for(int j = 0; j < 3; j++)
							runs[i].date[3+j] = linha[6+j];
						runs[i].date[6] = '/';
						for(int j = 0; j < 4; j++)
							runs[i].date[7+j] = linha[14+j];
					}
					runs[i].date[11] = '\0';
				}

				else{
					char* atributo;
					char* valor;
					double v1;
					atributo = strtok(linha, ":");
					valor = strtok(NULL, " :");
					if(valor && atributo){
						v1 = atof(valor);
						separaAtributos(runs, atributo, v1, i, &cadAux, &avgHRAux, &avgVeloAux);
					}			

				}

				countLinhas++;
			}
			runs[i].avg_hr = runs[i].avg_hr / avgHRAux;
			runs[i].avg_velo = runs[i].avg_velo / avgVeloAux;
			fclose(arch);
			i++;
		}
	}
	free(linha);

	closedir(dirLogs);

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
		free(runs[i].date);
	}
	free(runs);
	return 0;
}
