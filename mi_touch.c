#include "mi_mkfs.h"

int main (int argc, char **argv) {
	char *buffer2;
	int t_size, i = 0;
	bmount(argv[1]);
	
	mi_creat(argv[2]);
	bumount(argv[1]);
}

