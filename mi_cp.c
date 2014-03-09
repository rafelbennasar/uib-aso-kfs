#include "conf.h"
#include <unistd.h>
#include "mi_mkfs.h"
int main (int argc, char **argv) {
	char buffer[BLOQSIZE];
	int fd = open(argv[1], O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	int fsize = 0, offs = 0;

	bmount(DISK_NAME);
	printf("Ob %s, té el FD: %i \n", argv[1], fd);
	while ((fsize = read(fd, &buffer, BLOQSIZE)) >= 0) {
		//printf("Copiaré %i bytes a\n", fsize);
		mi_write(argv[2], &buffer, offs, fsize);
		offs += fsize;
		if (fsize == 0)
			break;
		}
	close(fd);
	bumount(DISK_NAME);
}


/*
 *
	printf("Ob %s, té el FD: %i \n", argv[1], fd);
	fsize = read(fd, &buffer, BLOQSIZE);
	printf("Copiaré %i bytes a\n", fsize);
	mi_write(argv[2], &buffer, 0, fsize);
	close(fd);
	*/
