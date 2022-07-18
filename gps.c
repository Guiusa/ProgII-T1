#include "utils.h"
#include <unistd.h>
#include <stdio.h>
int main(int argc, char **argv) {
	int opt;
	int quantiaLogs;
	char* dir;
	int option;
	int numBikes; 
	run_t* runs;
	char** listBikes = NULL;
	
	if((opt = getopt(argc, argv, "d:")) != -1){
		switch(opt){
			case 'd':
				printf("O programa está sendo executado...\n");
				dir = optarg;
				break;
			default:
				exit (1);
				printf("Invalid option, please use\n./gps -d <directory_name>\n");
		}		
	}

	quantiaLogs = countArchs(dir);
	runs = runAlloc(quantiaLogs);
	listBikes = countBikes(dir, &numBikes);
	

	if(!gerarLogs(runs, dir, quantiaLogs)){
		printf("Impossível processar arquivos\n");
		return 1;
	}

	//imprimaRuns(runs, quantiaLogs);

	printf("SELECIONE UMA DAS OPÇÕES\n");
	printf("0-Encerrar programa\n");
	printf("1-Bicicletas encontradas\n");
	printf("2-Atividades de uma das bicicletas conforme o índice\n");
	printf("3-Todas as atividades, ordenado pela distância\n");
	printf("4-Todas as atividades, ordenado pela subida\n");
	printf("5-Histograma de uma bicicleta\n");
	printf("opção: ");
	scanf(" %d", &option);
	
	while(1){
		if(option == 0){
			summary(runs, quantiaLogs);
			break;
		}
		if(option==1){
			printf("Foram encontradas %d bicicletas nos logs do diretório dado", numBikes);
			imprimaBikes(listBikes, numBikes);
		}
		else if(option == 2){
			int indiceBike;
			printf("indice da bike: ");
			scanf(" %d", &indiceBike);
			printf("\n");
			imprimeRunsBikeX(listBikes[indiceBike], runs, quantiaLogs);
		}
		else if(option == 3){
			ordenaDist(runs, quantiaLogs);

			for(int i = 0; i < numBikes; i++)
				imprimeRunsBikeX(listBikes[i], runs, quantiaLogs);
		}
		else if(option == 4){
			ordenaSubida(runs, quantiaLogs);
			for(int i = 0; i < numBikes; i++)
				imprimeRunsBikeX(listBikes[i], runs, quantiaLogs);
		}
		else if(option == 5){
			int indiceBike;
			printf("indice da bike: ");
			scanf(" %d", &indiceBike);
			printf("\n");
			histograma(runs, quantiaLogs, listBikes[indiceBike]);
		}

		printf("opção: ");
		scanf(" %d", &option);
	}
	
	bikesFree(listBikes, numBikes);
	runFree(runs, quantiaLogs);
	return 0;
}
