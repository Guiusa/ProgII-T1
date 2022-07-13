#include "utils.h"
#include <unistd.h>
#include <stdio.h>
int main(int argc, char **argv) {
	int opt;
	int quantiaLogs;
	char* dir;
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
	printf("A quantia de logs encontrados nesse diretório é %d\n", quantiaLogs);
	
	runs = runAlloc(quantiaLogs);
	
	listBikes = countBikes(dir, &numBikes);
	printf("%d bikes encontradas\n", numBikes);
	
	imprimaBikes(listBikes, numBikes);

	if(!gerarLogs(runs, dir, quantiaLogs)){
		printf("Impossível processar arquivos\n");
		return 1;
	}

	bikesFree(listBikes, numBikes);
	runFree(runs, quantiaLogs);
	return 0;
}
