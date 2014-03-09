#include "mi_mkfs.h"
#include <stdio.h>
#include <time.h>

int main (int argc, char **argv) {
	char *buffer2;
	int t_size, i = 0;
	char linea[50];	
	bmount(DISK_NAME);
	int k = 0;	
	time_t temps;
	struct tm t;
	mi_creat(argv[1]);

	for(i = 0; i < 100; i++) {					
		temps = time(NULL);
		localtime_r(&temps, &t);
		sprintf(linea,"%d:%d:%d Linea numero %i del proceso %i \n", t.tm_hour, t.tm_min, t.tm_sec, i, getpid());
		printf("Escriuré: %s \n", linea);
		mi_write(argv[1], &linea, k, strlen(linea));
		k += strlen(linea);
	}



	bumount(DISK_NAME);
	return 0;
}
