#include "mi_mkfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct i_stat {
	unsigned int  i_mode;        
        unsigned int  i_size;       
        time_t  i_ctime;     
        time_t  i_mtime;    
        unsigned int  i_blocks;  
};

int main(int argc, char **argv) {
	struct i_stat e;
	int n = 0, lin = 0;
	
	bmount(DISK_NAME);
	mi_stat(argv[1], &e);
	printf("Tamany de l'arxiu: %i \n", e.i_size);
	char *buffer = malloc(e.i_size);

	if(argc != 2) {
		printf("Nombre incorrecte de paràmetres!\n");
		return -1;
	}

	if (e.i_size != 0) {
		lin++;
		mi_read(argv[1], buffer, 0, e.i_size);
		printf("%s", buffer);
	}
	bumount(DISK_NAME);

}
