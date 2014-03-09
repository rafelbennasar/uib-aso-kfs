#include "conf.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <sys/shm.h>


#define START_MB 1
/* |    |         |             |      |
 * | SB | Bit Map | Inode Array | Data |
 * |    |         |             |      |
 * 0     fb     lb fi         li fdb  ldb
 */
/*
* Structure  superblock 
* name: Nombre de la unidad.
* fb: first block: Número del primer bloque del mapa de bits
* lb: last block: Número del último bloque del mapa de bits
* fi: first inode:  Número del primer bloque del array de inodos
* li: last inode: Número del último bloque del array de inodos
* fdb: first data block: Número del primer bloque de datos
* ldb: last data block: Número del último bloque de datos
* irf: inode root folder: Número del inodo del directorio raíz
* ffi: first free inode: Número del primer inodo libre
* qfb: quantity free blocks: Cantidad de bloques libres
* qfi: quantity free inodes: Cantidad de inodos libres
* tqb: total quantity blocks: Cantidad total de bloques
* tqi: total quantity inodes: Cantidad total de inodos
*/

struct superblock {
	char name[32];
	int fb; 
	int lb; 
	int fi;
	int li;
	int fdb;
	int ldb;
	int irf;
	unsigned int ffi;
	int qfb;
	int qfi;
	int tqb;
	int tqi;
};
/*
* i_mode: inode mode: Tipo (libre, directorio o fichero)
* i_size: inode size:  Tamaño en bytes
* i_ctime: inode creation time: Fecha de creación
* i_mtime: inode modification time: Fecha de modificación
* i_blocks: inode blocks: Cantidad de bloques asignados
* i_direct1: direct inode: Varios punteros a bloques directos
* i_idirect: indirect inode: Punteros a bloques indirectos
*
* i_mode: 0: libre. 1: fichero. 2: directorio.
* 
*/
struct inode {
	short  i_mode;        
        unsigned int  i_size;       
        time_t  i_ctime;     
        unsigned int  i_links;    
        unsigned int  i_blocks;  
        unsigned int  i_direct[8]; 
        unsigned int  i_indirect[3];   
};


int pos_bit(int num_bloque);
int pos_byte(int num_bloque);
int pos_byte2(int num_bloque);
int pos_bloc(int num_bloque);
int pos_bloc_inode(int n_inode);
int pos_byte_inode(int n_inode);
int reservar_bloque();
int liberar_bloque(int bloque);
int set_mb(short int value, int block);
int set_mb_bloque(short int value, int bloque);
int escribir_inodo(int pos, struct inode i_inodo);

struct inode leer_inodo(int n_inode);
int reservar_inodo(short i_mode);
int liberar_inodo(int n_inodo);
int liberar_metabloque(int n_bloque, int nivel);
long long int get_level(int level);
long long int raise_level(int level);
int deep_search(unsigned int inodo, unsigned int blogico, int nivel, char reservar);
//int traducir_bloque_inodo(unsigned int inodo, unsigned int blogico, char reservar);
int traducir_bloque_inodoB(unsigned int inodo, unsigned int blogico, unsigned int *bfisico, char reserva);

int traducir_bloque_inodo3(unsigned int inodo, unsigned int blogico, unsigned int *bfisico, char reserva);
int leer_bit(int pos);
int escribir_bit(int pos, int bit);
