#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <fcntl.h>
#include "ficheros_basico.h"


int main (int argc, char *argv[]) {
	int fdin, i = 0;
	superblock *sb;

	if (argc < 2) 
		return -1;
	if ((fdin = open (argv[1], O_RDWR)) < 0)
		printf("[ERROR] No se pot obrir %s \n", argv[1]);
	if ((sb = mmap(0, sizeof(superblock), PROT_READ | PROT_WRITE, MAP_SHARED, fdin, 0)) < 0)
		printf("[ERROR] Error a l'assignació mmap!");
	

	printf ("Nom de la unitat: %s\n",sb->name);

	printf(" \t \t \t lliures \t ocupats \t total \n");
	printf("Blocs d'Inodes: \t %i \t %i \t %i \n", sb->qfb, (sb->tqb)-(sb->qfb), sb->tqb);
	printf("Blocs de Dades: \t %i \t %i \t %i \n", sb->qfb, (sb->tqb)-(sb->qfb), sb->tqb);

	printf(" \t \t lliures \t ocupats \n");
	printf("Blocs d'Inodes: \t %i \t %i \n", ((sb->qfb)/(sb->tqb)), 1-((sb->qfb)/(sb->tqb)) );
	printf("Blocs de dades: \t %i \t %i \n", ((sb->qfb)/(sb->tqb)), 1-((sb->qfb)/(sb->tqb)) );
} 




