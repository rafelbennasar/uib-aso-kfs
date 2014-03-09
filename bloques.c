#include "bloques.h"

static	int fd;
static struct superblock sb;

struct superblock get_sb(){
	struct superblock sb;
	char buffer[BLOQSIZE];
	bread(0, buffer);
	memcpy(&sb, &buffer, sizeof(struct superblock));
	return sb;
}

void set_sb(struct superblock *sb){
	char buffer[BLOQSIZE];
	memcpy(&buffer, sb,sizeof(struct superblock)); 
	bwrite(0, buffer);
}

int set_fd(int *new_fd) {
	fd = *new_fd;
	printf("FD nou assignat: %i \n", fd);
}

/*
 * Monta la unidad *name* y retorna su descriptor de fichero.
 *
 */
int bmount (char *name) 
{
	if ((fd = open(name, O_CREAT|O_RDWR,S_IRUSR|S_IWUSR)) < 0) { 
		printf("[bloques.c] [ERROR] S'ha produït un error intentant montar l'unitat \n");
		return -1;
	} else if (DMODE == 0) { 
		printf("[bloques.c] [INFO] S'ha montat l'unitat: %s \n", name); 
		printf("[bloques.c] [INFO] El nou descriptor és: %i \n", fd); 
	}
	return fd;
}



/*
 * Desmonta la unidad montada.
 *
 */
void bumount (void)
{
	if (close(fd) < 0) {
		 printf("[bloques.c] [ERROR] S'ha produït un error intentant desmontar l'unitat assignada al descriptor: %i \n", fd);
	} else if (DMODE == 0) { 
		printf("[INFO] S'ha desmontat l'unitat del descriptor: %i \n", fd);
	};
}


/*
 * Lee el bloque *nbloque* y carga su contenido en el buffer.
 *
 */
int bread (int nbloque, void *buffer) {
	ssize_t fsize;

	if (nbloque < 0) {
		printf("[bloques.c] [ERROR] (lectura) S'ha intentat apuntar un bloc fora de rang! (bloc: %i)\n", nbloque);
		return -1;
	}	
        if (lseek(fd, nbloque*BLOQSIZE, SEEK_SET) < 0){
		printf ("[bloques.c] [ERROR] (lectura) S'ha produït un error a lseek! (Bloc: %i) \n", nbloque);
		return -1;
  	}
	if ((fsize = read(fd, buffer, BLOQSIZE)) < 0){
		printf("[bloques.c] [ERROR] [bread] S'ha produït un error en la lectura del descriptor: %i \n", fd); 
		return -1;
  	}
	if (DMODE == 0)
		printf("[bloques.c] [INFO] S'han llegit %i bytes de informació del bloc número %i \n", fsize, nbloque);
	return fsize;
}


/*
 * Escribe el contenido del buffer en la posición *nbloque*
 *
*/
int bwrite (int nbloque, void *buffer) {
	ssize_t fsize;
	if (nbloque < 0) {
		printf("[bloques.c] [ERROR] (escritura) S'ha intentat apuntar un bloc fora de rang! (bloc: %i)\n", nbloque);
		return -1;
	}	
        if (lseek(fd, nbloque*BLOQSIZE, SEEK_SET) < 0){
		printf ("[bloques.c] [ERROR] (escritura) S'ha produït un error a lseek!\n");
    		return -1;
	}
        if ((fsize = write(fd, buffer, BLOQSIZE)) < 0){
		printf("[bloques.c] [ERROR] [bwrite] S'ha produït un error en la lectura del descriptor: %i \n", fd); 
    		return -1;
 	}

	if (DMODE == 0)
		printf("[bloques.c] [INFO] S'han escrit %i bytes de informació al bloc número %i \n", fsize, nbloque);

	return fsize;
}

