#include "directorio.c"

int main (int argc,char **argv) {
	int s = 1, n = 0;
	char linea[256];
	struct inode inodo;
	struct dentry entrada;

	bmount("prova");
//	while(s < 2) {
		sprintf(linea, "EntradaDeInode-%i", 1);
		new_entry(1, linea);
//		s++;
//	}


	bumount("prova");
return 1;
}
