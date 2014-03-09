#include <string.h>
#include <stdio.h>
#include "directorio.h"
#include "conf.h"
#include "semaforos.c"
#define FITXER 1
#define DIRECTORI 2



//int mutex = 0, mutex2= 0;
/*
 * Dado un path /ca/mi/no, sea el resultado:
 * first: /ca
 * resto: /mi/no
 */
int extraer_camino(char *path, char *first, char *resto) {
	int i = 0, j = 0, tipus = 0;
	if (path[0] == '/')
		*path++;
	if (path[0] == '\0')
		return -1;	
	while ((path[i] != '/') && (path[i] != '\0'))
		first[i] = path[i++];
	first[i] = '\0';

	if (path[i] == '/') { tipus = DIRECTORI; } else { tipus = FITXER; }
		
	while (path[i] != '\0')
		resto[j++] = path[i++];
	resto[j++] = '\0';

	return tipus;
}


/*
 * Dado un path /ca/mi/no, sea el resultado:
 * first: /ca/mi
 * resto: /no
 */
int extraer_final(const char *camino, char *inicial, char *final){
	int i, f;
	strcpy(inicial, camino);

	for (i = 0; camino[i] != '\0'; i++);

	if (inicial[i] == '/')
		inicial[i] = '\0';

	while (inicial[i] != '/') {
		inicial[i] = '\0';
		i--;
	}

	strcpy(final, camino+i+1);
}


/*
 * Elimina los caracteres '/' del final del string.
 *
 */

int remove_slash(char *string){
	int i = 0;
	for (i = 0; string[i] != '\0'; i++); 
		for (i = i - 1; (string[i] == '/') && (i > 0); i--)  
			string[i] = '\0';
}





int buscar_entrada(char *camino_parcial){
	char path[256], inicial[256], final[256];
	struct inode inodo;
	struct superblock sb;
	struct dentry entrada;
	get_sb(&sb);

	int t_entrada = sizeof(struct dentry);
	unsigned int *p_entrada;
	int i = 0, size = 0;
	int n_inode = sb.irf;

	
	camino_parcial++;
	strcpy(path, camino_parcial);

	while ((strlen(path) > 1) && (*path != '\0')) {
                        extraer_camino(path, inicial, final);
                        n_inode = search_entry(n_inode, inicial);
                        if (n_inode == -1)  // No existeix s'entrada!
                                return -1;
                        strcpy(path, final);
        } 	
		return n_inode;
}

/*
 * Crea un fichero/directorio especificado en path. Si existe devuelve el valor 
 * de la entrada anterior, en caso contrario, crea una nueva.
 * 
 * Si crea una entrada nueva, retorna 0.
 * si ya existia, retorna 1.
 */

int get_entrada(int n_inode, char nom) {
        struct inode i_inodo;
        struct dentry entrada;
        int n = 0;
	
	i_inodo = leer_inodo(n_inode);
	while (n < i_inodo.i_size) {
		mi_read_f(n_inode, &entrada, n, 64);
		if (strcmp(entrada.nombre, nom) == 0) {
			return entrada.inodo;
		}
		n += 64;
	}
	return -1;
}

int new_entry(int n_inode, char *nom) {
	struct inode i_inodo;
	struct dentry entrada;
	memset(&entrada, 0, 64);
	i_inodo = leer_inodo(n_inode);
	entrada.inodo = reservar_inodo(1);
	strcpy(entrada.nombre, nom);
	mi_write_f(n_inode, &entrada, i_inodo.i_size, sizeof(struct dentry));
	return entrada.inodo;
}

int search_entry(int n_inode, char *nom){
	struct inode i_inodo;
	struct dentry entrada;
	int n = 0;
	i_inodo = leer_inodo(n_inode);
	while (n < i_inodo.i_size) {        
		mi_read_f(n_inode, &entrada, n, 64);
		if (strcmp(entrada.nombre, nom) == 0) {
		      return entrada.inodo;
		}
		n += 64;
	}
	return -1;
}

int mi_creat(char *camino){
	struct superblock sb;
	get_sb(&sb);
	char *path = malloc(1024), *actual = malloc(1024), *resto = malloc(1024);
	struct dentry entrada;
	struct inode i_inodo;
	int n_inode = sb.irf, nou_inode = 0, next = -1, e = 0, n = 0;
	strcpy(path, camino);

	while (extraer_camino(path, actual, resto) != -1) {
			next = search_entry(n_inode, actual);
			if (next == -1) {
				next = new_entry(n_inode, actual); 
				printf("Nova entrada per %s, a l'inode %i \n", actual, n_inode);
			}
		strcpy(path, resto);
		n_inode = next;
	}
        return n_inode;
}


/*
 * Copia en **bufer los ficheros/archivos contenidos en la entrada de directorios.
 * Retorna el tamaño del buffer.
 */

int mi_dir(char *path, char **buffer){
	struct inode ai;

	int n_inode = 0, readed = 0;
	n_inode = buscar_entrada(path);

	if (n_inode == -1)
		return -1;
	ai = leer_inodo(n_inode);
	*buffer = malloc(ai.i_size);
	memset(*buffer, 0, ai.i_size);

	while (readed < ai.i_size) {
		mi_read_f(n_inode, *buffer + readed, readed, ai.i_size - readed);
		readed += BLOQSIZE; // + sizeof(struct dentry);
	}

	return ai.i_size;
}


int mi_dir2(int n_inode, char **buffer){
	struct inode ai;

	int readed = 0;

	if (n_inode == -1)
		return -1;
	ai = leer_inodo(n_inode);
	*buffer = malloc(ai.i_size);
	memset(*buffer, 0, ai.i_size);

	while (readed < ai.i_size) {
		mi_read_f(n_inode, *buffer + readed, readed, ai.i_size - readed);
		readed += BLOQSIZE; // + sizeof(struct dentry);
	}

	return ai.i_size;
}
/*
 * Extrae la información asociada a un fichero/directorio.
 */
int mi_stat(char *path, struct i_stat *p_stat) {
	int b;
	int i_inode = 0;
	i_inode = buscar_entrada(path);
	mi_stat_f(i_inode, p_stat);
}

int del_entry(int n_inode, char *nom) {
	struct inode ai;
	int n = 0;
	struct dentry *entrada2 = malloc(sizeof(struct dentry));
	struct dentry *entrada3 = malloc(sizeof(struct dentry));
	ai = leer_inodo(n_inode);
	printf("Borraré %s de l'node %i \n", nom, n_inode);
	while (n < ai.i_size) {	
		mi_read_f(n_inode, entrada2, n, sizeof(struct dentry));

		if (strcmp(entrada2->nombre, nom) == 0) {
			liberar_inodo(entrada2->inodo);
			if (ai.i_size >= (2*sizeof(struct dentry))) {
				mi_read_f(n_inode, entrada2, ai.i_size - sizeof(struct dentry), sizeof(struct dentry));
				memcpy(entrada3,  entrada2, sizeof(struct dentry));
				mi_write_f(n_inode, entrada3, n, sizeof(struct dentry));
				ai.i_size -= sizeof(struct dentry);
				escribir_inodo(n_inode, ai);
				free(entrada2); free(entrada3);
				return 1;
			} else { 
				memset(entrada3, '\0', sizeof(struct dentry));
				mi_write_f(n_inode, entrada3, 0, sizeof(struct dentry));
				ai.i_size -= 2*sizeof(struct dentry); 
				free(entrada2); free(entrada3);
				return 1;				
			}
		}
		n += 64;
	}
}

/*
 * Borra un archivo/fichero de la entrada de directorios,
 * liberando el espacio ocupado.
 */
int mi_unlink(char *path) {

	int b = 0, n = 0, n_inode = 0; 
	char *first = malloc(sizeof(char)), *last = malloc(sizeof(char));

	struct superblock sb;
	get_sb(&sb);
	struct inode ai;
	extraer_final(path, first, last);

	n_inode = buscar_entrada(first);
	ai = leer_inodo(n_inode);

	// Si se ha encontrado la entrada:	
	if (n_inode > -1) {
		del_entry(n_inode, last);
	}
	return -1;
}



/*
 * Guarda los datos del buffer en el fichero asociado, teniendo un offset.
 *
 *
 */
int mi_write(const char *path, const void *buf, unsigned int offset, unsigned int nbytes) {

	int b;
	int i_inode = mi_creat(path);
	if (DMODE == 0) {	
		printf("[directorio.c] [INFO] Escrits %i bytes! \n", nbytes);
	}

	return mi_write_f(i_inode, buf, offset, nbytes);
}



/*
 * Lee  los datos asociados a un fichero y los guarda en el buffer.
 * read: int (*read) (const char *, char *, size_t, off_t, struct fuse_file_info *);
 */
int mi_read(const char *path, void *buf, unsigned int offset, unsigned int nbytes) {
	int a, b;
	int i_inode = 0;
	i_inode = buscar_entrada(path);
	if (i_inode != -1) 
		mi_read_f(i_inode, buf, offset, nbytes);
	if (DMODE == 0)	
		printf("[directorio.c] [INFO] Llegits %i bytes! \n", nbytes);
}

