#include "ficheros.h"
#include "semaforos.h"
#include "conf.h"

#define _FILE_OFFSET_BITS   64
#define FUSE_USE_VERSION    22

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct dentry {
	char nombre[60];
	unsigned int inodo;
};


int buscar_entrada(char *camino_parcial);
int extraer_camino(char *path, char *first, char *resto); 
int mi_creat(char *camino);

//int mi_unlink(const char *camino);
int  mi_dir(char *path, char **buffer);
int mi_stat(char *path, struct i_stat *p_stat);
int mi_write(const char *path, const void *buf, unsigned int offset, unsigned int nbytes);
int mi_read(const char *path, void *buf, unsigned int offset, unsigned int nbytes);
//static int mi_read2( const char *path, void *buf, size_t nbytes, off_t offset, struct fuse_file_info *info );
