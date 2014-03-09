#include "mi_mkfs.h"

int main() {
	int i;
	struct superblock sb;
	get_sb(&sb);

	bmount("prova");

	while (i++ <= 100) 
		printf("Reserv inode: %i \n", reservar_inodo(2));
	
	while (--i > 0) { 
		printf("Alliber inode: %i \n", i);
		liberar_inodo(i);
	}
	bumount("prova");
}
