#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <fcntl.h>
#include "mi_mkfs.h"


int main (int argc, char *argv[]) {

	if (argc < 2) {
		printf("[INFO] Mode d'ús: mi_sb <disc virtual> \n");
		return -1;
	}

	bmount(argv[1]);
	struct superblock *sb;
	sb = (struct superblock*)mmap(0, sizeof(struct superblock), PROT_WRITE | PROT_READ, MAP_SHARED, 3, 0);
	printf("fb: first block: Número del primer bloque del mapa de bits: %i\n",sb->fb );
	printf("lb: last block: Número del último bloque del mapa de bits: %i\n",sb->lb );
	printf("fi: first inode:  Número del primer bloque del array de inodos: %i\n",sb->fi );
	printf("li: last inode: Número del último bloque del array de inodos: %i\n",sb->li );
	printf("fdb: first data block: Número del primer bloque de datos: %i\n",sb->fdb );
	printf("ldb: last data block: Número del último bloque de datos: %i\n",sb->ldb );
	printf("irf: inode root folder: Número del inodo del directorio raíz: %i\n",sb->irf );
	printf("ffi: first free inode: Número del primer inodo libre: %i\n",sb->ffi );
	printf("qfb: quantity free blocks: Cantidad de bloques libres: %i\n",sb->qfb );
	printf("qfi: quantity free inodes: Cantidad de inodos libres: %i\n",sb->qfi );
	printf("tqb: total quantity blocks: Cantidad total de bloques: %i\n",sb->tqb );
	printf("tqi: total quantity inodes: Cantidad total de inodos: %i\n",sb->tqi );

	printf("\n");
	printf(" |    |                 |                             |            |\n");
	printf(" | SB |      Bit Map    |         Inode Array         |    Data    |\n");
	printf(" |    |                 |                             |            |\n");
	printf("     %i                  %i                             %i         %i\n", sb->fb, sb->fi, sb->li, sb->ldb);


} 




