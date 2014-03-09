//#include "../mi_mkfs.h"
#include "directorio.c"

int main() {
	char origen[1024] = "... then the kernel chooses the address at which to create the mapping; this is the most portable method of creating a new mapping.  If addr is not NULL, then the kernel takes it as a hint about where to place the mapping; on Linux, the mapping will be created at the next higher page boundary.  The address of the new mapping is returned as the result of the call.";

	char buffer[1024];

	printf("Prova d'escritures a un fitxer\n");
	bmount("prova");

	mi_write("/home/rafel/prova.txt", &origen, 0, 2048);

	mi_read("/home/rafel/prova.txt", &buffer, 0, 2048);

	printf("\n Resultat: %s \n", &buffer);
	
	bumount("prova");

}


	
