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
				quantiaLogs = countArchs(optarg);
				dir = optarg;
				break;
			default:
				printf("Invalid option, please use\n./gps -d <directory_name>\n");
		}		
	}

	printf("A quantia de logs encontrados nesse diretório é %d\n", quantiaLogs);
	
	runs = runAlloc(quantiaLogs);
	
	numBikes = countBikes(dir, listBikes);
	printf("%d bikes encontradas\n", numBikes);
	
	imprimaBikes(listBikes, numBikes);
	//bikesFree(listBikes, numBikes);
	runFree(runs);
	return 0;
}
