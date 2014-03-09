#include "mi_mkfs.h"
#include "conf.h"

int main (int argc, char **argv) {
	char buffer[50];
	int t_size, i = 0;
	bmount(DISK_NAME);
	sprintf(buffer, "%s%s", argv[1], "/");
	mi_creat(buffer);
	bumount(DISK_NAME);
}
