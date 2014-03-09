#include "mi_mkfs.h"

int main (int argc,char **argv) {
	int s = 1, n = 0;
	unsigned int buffer[BLOQSIZE];

	bmount("prova");
	
		bread(argv[1], &buffer);
		for (s = 0; s < BLOQSIZE; s++)
			printf("%i, ", buffer[s]);

	bumount("prova");
return 1;
}
