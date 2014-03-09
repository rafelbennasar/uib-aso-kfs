#include "mi_mkfs.h"

int main() {
	int n_inodo = 100;

	char origen[1024] = "... then the kernel chooses the address at which to create the mapping; this is the most portable method of creating a new mapping.  If addr is not NULL, then the kernel takes it as a hint about where to place the mapping; on Linux, the mapping will be created at the next higher page boundary.  The address of the new mapping is returned as the result of the call.";
	char buffer[1024];

	printf("Prova d'escritures mapejades\n");
	bmount("prova");

	mi_write_f(n_inodo, &origen, 0, 1024);
	mi_read_f(n_inodo, &buffer, 0, 1024);

	printf("\n Resultat: %s \n", &buffer);
	
	bumount("prova");

}

