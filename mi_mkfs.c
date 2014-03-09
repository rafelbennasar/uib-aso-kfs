#include "mi_mkfs.h"

static int fdin;
static	int fd;



int main (int argc, char **argv) {
	int i;
	char buffer[BLOQSIZE];
	struct superblock sb;
	
	memset(&buffer, 0, BLOQSIZE);

	fd = bmount(argv[1]);
	get_sb(&sb);


	for (i=0; i <= atoi(argv[2]); i++) 
		bwrite((int)i, &buffer);

	initSB(argv[1], atoi(argv[2]));
	initBM();
	initIA();

	
	
	printf("[mi_mkfs.c] [INFO] S'ha creat el fitxer virtual %s. \n[Tamany: %i KB] [Blocs: %i] \n", argv[1], BLOQSIZE*atoi(argv[2])/1024, atoi(argv[2]));  
	bumount();

	return 1;
}	
