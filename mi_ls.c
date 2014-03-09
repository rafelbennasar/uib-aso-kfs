#include "mi_mkfs.h"
#include "conf.h"

int main (int argc, char **argv) {

	char *buffer2;
	int t_size, i = 0;

	
	bmount(DISK_NAME);
	t_size = mi_dir(argv[1], &buffer2);
	printf("Hi ha %i arxius. \n", t_size/64);
	bumount(DISK_NAME);
	
	if (t_size > 0) {
		for (i = 0; i < t_size; i += 64) {
			printf("%s  \t ", &buffer2[i]);
			if (((i/64) % 4) == 3)
				printf("\n");
		}
	}
	printf("\n");
}
