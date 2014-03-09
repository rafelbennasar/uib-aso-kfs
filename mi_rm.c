#include "mi_mkfs.h"
#include "conf.h"

int main (int argc, char **argv) {
	bmount(DISK_NAME);
		printf("Es borrarà %s, n'està segur? (S/N) \n", argv[1]);

		mi_unlink(argv[1]);
	bumount(DISK_NAME);
}
