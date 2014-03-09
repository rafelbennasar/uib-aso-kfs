#include "mi_mkfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv) {
	struct inode e;
	char *unitats = "bytes";
	int n = 0;
	float tam = 0;

		bmount(DISK_NAME);
	mi_stat_f(atoi(argv[1]), &e);
	printf("Mi STAT: \n");
	printf("Tipus inode: %i \n", e.i_mode);

	tam = e.i_size;

	if (tam > 1024) { tam /= 1024; unitats = "Kbytes"; } 
	if (tam > 1024) { tam /= 1024; unitats = "Mbytes"; }
	if (tam > 1024) { tam /= 1024; unitats = "Gbytes"; }
	if (unitats == "bytes")
		printf("Tamany: %2.3f %s  \n", tam, unitats);
	else
		printf("Tamany: %2.3f %s  (%i bytes)\n", tam, unitats, e.i_size);
	printf("Blocs assignats: %i \n", e.i_blocks);
	
//	printf("Data creació: %i \n", ctime(&e.i_ctime));
//	printf("Data modificació: %i \n", ctime(&(e.i_mtime)));
		bumount(DISK_NAME);
	
	return -1;

}
