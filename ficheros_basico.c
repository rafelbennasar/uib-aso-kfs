#include "ficheros_basico.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define SEED "ficheros_basico.c"
static int mutex_w;

long power(int base,int exponent) {
	long int result, pow;
	int i = 0;
	result=1L; pow=base;
	for (i=exponent; i > 0; i=i/2) {
		if (i%2 == 1) result=result*pow;
		pow=pow*pow;
	}
	return result;
}


int pos_bit(int num_bloque) {
	return (num_bloque % 8);
}

int pos_byte(int num_bloque) {
	return (num_bloque / 8)%BLOQSIZE;
}
int pos_byte2(int num_bloque) {
	return num_bloque%BLOQSIZE;
}

int pos_bloc(int num_bloque) {
  return (num_bloque/BLOQSIZE);
}

unsigned int calcMB(int n) {
	if ((n % BLOQSIZE) > 0) 
		return ((n/BLOQSIZE));
	else 
		return (n/BLOQSIZE);
}

int tamMB(struct superblock *sb, int mblocs) {
	return ((sb->qfb) - (sb->li));
}

int tamAI(int mblocs) {
	return (mblocs/PERCENT_OF_INODES);
}

int inodesBloc() {
	return (BLOQSIZE/sizeof(struct inode));
}
/*
 * Devuelve el número del bloque en que se encuentra un inodo.
 */
int pos_bloc_inode(int n_inode) {
        return n_inode/(BLOQSIZE/sizeof(struct inode));
}

int pos_byte_inode(int n_inode) {
        return (n_inode % sizeof(struct inode));
}

/* |    |         |             |      |
 * | SB | Bit Map | Inode Array | Data |
 * |    |         |             |      |
 * 0    1         
 */
/*
* Structure  superblock 
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

/* InitSB: Init Super Block. Inicializa los valores del superbloque. */
void initSB (char *name, int mblocs) {
	struct superblock sb;
	get_sb(&sb);
	sb.tqb = mblocs;
	
	sb.fb = 1;
	sb.lb = calcMB(sb.tqb)+1;
	sb.fi = (sb.lb)+1;
	sb.li = (sb.fi) + tamAI(mblocs);
	sb.fdb = (sb.li) + 1;
	sb.ldb = mblocs;
	sb.irf = -1;
	sb.ffi = 2;
	sb.tqi = tamAI(mblocs) * inodesBloc();
	sb.qfb = mblocs;
	sb.qfi = sb.tqi;

	printf("fb: first block: Número del primer bloque del mapa de bits: %i\n",sb.fb );
	printf("lb: last block: Número del último bloque del mapa de bits: %i\n",sb.lb );
	printf("fi: first inode:  Número del primer bloque del array de inodos: %i\n",sb.fi );
	printf("li: last inode: Número del último bloque del array de inodos: %i\n",sb.li );
	printf("fdb: first data block: Número del primer bloque de datos: %i\n",sb.fdb );
	printf("ldb: last data block: Número del último bloque de datos: %i\n",sb.ldb );
	printf("irf: inode root folder: Número del inodo del directorio raíz: %i\n",sb.irf );
	printf("ffi: first free inode: Número del primer inodo libre: %i\n",sb.ffi );
	printf("qfb: quantity free blocks: Cantidad de bloques libres: %i\n",sb.qfb );
	printf("qfi: quantity free inodes: Cantidad de inodos libres: %i\n",sb.qfi );
	printf("tqb: total quantity blocks: Cantidad total de bloques: %i\n",sb.tqb );
	printf("tqi: total quantity inodes: Cantidad total de inodos: %i\n",sb.tqi );

	printf("\n");
	printf(" |    |                 |                             |            |\n");
	printf(" | SB |      Bit Map    |         Inode Array         |    Data    |\n");
	printf(" |    |                 |                             |            |\n");
	printf("     %i                  %i                             %i         %i\n", sb.fb, sb.fi, sb.li, sb.ldb);
	set_sb(&sb);
}


int initIA() {
	struct superblock sb;
	char buffer[BLOQSIZE];
	struct inode inodo;
	struct inode *p_inodo;
	

	get_sb(&sb);
	int i = 0, j = 0, offset = 0;
	unsigned int k = 0;
	int n_inodes = inodesBloc();

	inodo.i_mode = 0; 
	inodo.i_size = 0;
	inodo.i_ctime = time(NULL);
	inodo.i_links = 0; 
	inodo.i_blocks = 0;

	for(i = 0; i< N_DIRECTS_INODES; i++){
		inodo.i_direct[i] = 0;
	}
	for(i = 0; i< N_INDIRECTS_INODES; i++){
		inodo.i_indirect[i] = 0;
	}

	for(j = 0; j < sb.tqi; j++){
		inodo.i_mode = 0;
		inodo.i_links = k + 1;
		escribir_inodo(j, inodo);
		k++;
	}
	sb.irf = 1;
	set_sb(&sb);

	return 0;
}

void initBM() {
	struct superblock sb;
	int k = 0;

	get_sb(&sb);

	for (k = 0; k < sb.fdb; k++) {  
		printf("reserv %i \n ", reservar_bloque());
	}

	set_sb(&sb);
	printf("mi_mkfs.c] [INFO] S'ha inicialitzat el Mapa de Bits!  \n");
	printf("mi_mkfs.c] [INFO] Blocs ocupats fins %i!  \n", sb.fdb);
	printf("mi_mkfs.c] [INFO] S'ha establert els blocs de metainformació del FS: (ocupats fins %i) \n", (sb.fdb)-1);
}
/*
 * Reserva un bloque y devuelve el número del bloque reservado.
 *
 */
int reservar_bloque() {
	struct superblock sb;
	get_sb(&sb);
	unsigned char mask = 0x80; 
	unsigned char bm[BLOQSIZE];

	int n = 0;
	int nb = 0;
	int i,pos = 0;
	for(n = sb.fb; n <= sb.lb; n++){
		bread(n, &bm);
		nb = 0;
		for(i = 0; i < BLOQSIZE/8; i++){
			if(bm[i] < 255){
				while(bm[i] & mask){
					bm[i] <<= 1;
					nb++;
				}
				pos = (BLOQSIZE*(n - sb.fb))+(i*8)+nb; 
				escribir_bit(pos,1);
				sb.qfb = sb.qfb - 1;
				set_sb(&sb);
				printf("Reserv: %i \n", pos);
				return pos;
			}
		}
	}
	return -1;
}


/*
 * Libera *bloque* y augmenta la cantidad de bloques libres.
 *
 */
int liberar_bloque(int bloque) {
	struct superblock sb;
	get_sb(&sb);

	int qmb = (sb.tqb)/sizeof(unsigned char);
	unsigned char *mb = malloc(qmb);

	char buffer[BLOQSIZE];
	memset(&buffer, 0, BLOQSIZE);
	bwrite(bloque, buffer);
	if (DMODE == 0)
		printf("[INFO] Alliber bloc %i \n", bloque);
	mb[bloque] = 0;

	sb.qfb = ++(sb.qfb);
	set_sb(&sb);
}

struct inode leer_inodo(int pos){
	struct superblock sb;
	get_sb(&sb);
	int offset = pos/inodesBloc();
	int aux = 0;

	int n_bloc = sb.fi + offset;
	struct inode inodes[sizeof(struct inode)];
	bread(n_bloc, &inodes);
	aux = pos % inodesBloc();
	return inodes[aux];
}

int escribir_inodo(int pos, struct inode ino){
	struct superblock sb; 
	int offset = pos/inodesBloc();
	int aux = 0;
	get_sb(&sb); 

	int n_bloc = sb.fi + offset;
	aux = pos % inodesBloc();
	struct inode inodes[sizeof(struct inode)];

	bread(n_bloc, &inodes);
	inodes[aux] = ino;
	bwrite(n_bloc, &inodes);
} 


int leer_bit(int pos){
	int nblock = pos/BLOQSIZE;
	int nbyte = (pos%BLOQSIZE)/8;
	unsigned char bytes[BLOQSIZE*8];
	bread(nblock, &bytes);

	return bytes[nbyte] && (128>>(pos%8));
}

int escribir_bit(int pos, int bit){
	struct superblock sb;
	get_sb(&sb);

	int nblock = sb.fb + (pos/BLOQSIZE);
	int nbyte = (pos % BLOQSIZE) /8;
	unsigned char bytes[BLOQSIZE*8];

	bread(nblock, &bytes);
	if (bit == 0){ 
		bytes[nbyte] &= ~(128>>(pos%8));
	}else{
		bytes[nbyte] |= (128>>(pos%8));
	}

	return bwrite(nblock, &bytes);
}
/*
 * Reserva un inodo y devuelve su número.
 * En caso de error, devuelve -1.
 */
int reservar_inodo(short i_mode) {
	struct superblock sb;
	struct inode inodo;	
	get_sb(&sb);
	unsigned char buffer[BLOQSIZE];
	unsigned int ifree = 0;
	int i = 0;

	if ((sb.qfi) > 0) {
		ifree = sb.ffi;
		inodo = leer_inodo(ifree);
		sb.ffi = inodo.i_links;
		sb.qfi = (sb.qfi) - 1;
		inodo.i_size = 0;
		inodo.i_blocks = 0;
		inodo.i_links = 0;
		inodo.i_ctime = time(NULL);
		inodo.i_ctime = time(NULL);
		set_sb(&sb);
		escribir_inodo(ifree, inodo);
		return ifree;
	} else {
		printf("[ficheros_basico.c] [ERROR] No queden inodes lliures! %i \n", ifree);
		return -1;
	}
}



int liberar_inodo(int n_inodo) {
	struct superblock *sb;
	sb = (struct superblock*)mmap(0, sizeof(struct superblock), PROT_WRITE | PROT_READ, MAP_SHARED, 3, 0);
	struct inode inodo;

	int i = 0;
	unsigned int buffer[BLOQSIZE];
	inodo = leer_inodo(n_inodo);	
		printf("Alliber els directes...");
		for (i = 0; i < N_DIRECTS_INODES; i++) {
			if (inodo.i_direct[i] > 0) {
				liberar_bloque(inodo.i_direct[i]);
			}
			inodo.i_direct[i] = 0;
		}

		printf("Alliber els indirectes...");
		if (inodo.i_indirect[0] > 0) {
			bread(inodo.i_indirect[0], buffer);
			for (i = 0; i < BLOQSIZE/sizeof(unsigned int); i++) {
				if (buffer[i] != 0)
					liberar_bloque(buffer[i]);
			}
			liberar_bloque(inodo.i_indirect[0]);
		}
		
	inodo.i_mode = 0;
	inodo.i_size = 0;
	inodo.i_blocks = 0;
	for (i = 1; i < N_DIRECTS_INODES; i++)
		inodo.i_direct[i] = 0;
	for (i = 0; i < N_INDIRECTS_INODES; i++)
		inodo.i_indirect[i] = 0;
	inodo.i_links = sb->ffi;
	sb->ffi = n_inodo;
	sb->qfi = sb->qfi + 1;

	printf("[ficheros_basico.c] [INFO] Alliber l'inode %i \n", n_inodo);
	return 1;
}




/*
 * Función necesaria para traducir_bloque_inodo.
 * Dado un nivel para bloques indirectos, retorna el umbral máximo de bloques
 * que puede direccionar.
 *
 */
long long int get_level(int level) {
	long long int offset = 0, offset2 = 1;
	int i, k; 

	for (i = 1; i <= level; i++) {
		offset2 *= BLOQSIZE;
		offset += offset2;
	}
	if (level >= 0)
		offset += 8;

	return offset;
}


/*
 * Función necesaria para traducir_bloque_inodo.
 * Dado un nivel, retorna el valor umbral máximo de un solo bloque
 * en un cierto nivel.
 */
long long int raise_level(int level) {
	long long int offset2 = 1;
	int i, k; 

	for (i = 1; i <= level; i++) {
		offset2 *= BLOQSIZE;
	}
	if (level == 0)
		offset2 = 8;

	return offset2;
}



int buscar_bloque_fisico(unsigned int *bloque,unsigned posicion, unsigned nivel, char reserva, int *reservado) {

	unsigned int ab[BLOQSIZE/sizeof(int)];
	struct superblock sb;
		get_sb(&sb);
	
	
	if (*bloque == 0) {
		if (reserva == 1 && sb.qfb>0) {	/* entonces debemos antes crear el siguiente nivel */
		
			memset(ab,0,sizeof(ab)); 
			*bloque = reservar_bloque();
			++*reservado;
			bwrite(*bloque,ab);
		} else {
			return -1;
		}
	}
	
	if(nivel==1) {	
	
		bread(*bloque,ab);	
		
		if (ab[posicion] != 0) {			
			return ab[posicion];
		} else {
			
			if(reserva == 1 && sb.qfb>0) {
				ab[posicion] = reservar_bloque();
				++*reservado;
				bwrite(*bloque,ab);
				return ab[posicion];
			} else {
				return -1;
			}
		}
		
	} else {
		bread(*bloque,ab);
		int intervalos  = (int)power(BLOQSIZE/sizeof(int),nivel-1);
		int bloqueFisico = buscar_bloque_fisico(&ab[posicion/intervalos],posicion%intervalos,nivel-1,reserva,reservado);
		bwrite(*bloque,ab);
		return bloqueFisico;
	}
	
}

int traducir_bloque_inodoB(unsigned int inodo, unsigned int blogico, unsigned int *bfisico, char reserva) {
	struct inode i;
	struct superblock sb;
	i = leer_inodo(inodo);				
		get_sb(&sb);
	
	int bloqueFisico = -1;
	int reservado	 = 0;

	/* variables para los intervalos, el máximo quiere decir que puede llegar a él
 * 	pero no rebasarlo [0,maxPrimerNivel] */
	
	int maxPrimerNivel	= get_level(1);
	int maxSegundoNivel	= get_level(2);
	int maxTercerNivel	= get_level(3);
	
	if (blogico>=0 && blogico<N_DIRECTS_INODES) {
		if(i.i_direct[blogico]==0) {
			if(reserva==1 && sb.qfb>0) {
				bloqueFisico = i.i_direct[blogico] = reservar_bloque();
				++reservado;
			}
		} else {
			bloqueFisico = i.i_direct[blogico];
		}
	
	} else if(blogico>=N_DIRECTS_INODES && blogico<=maxPrimerNivel) {	/* nivel 1 */
		printf("primer");	
		bloqueFisico = buscar_bloque_fisico(&i.i_indirect[1],blogico-N_DIRECTS_INODES,1,reserva,&reservado);
	
	} else if(blogico>maxPrimerNivel && blogico<=maxSegundoNivel) {		/* nivel 2 */
		printf("segon");	
		
		bloqueFisico = buscar_bloque_fisico(&i.i_indirect[2],blogico-(maxPrimerNivel +1),2,reserva,&reservado);	
		
	} else if(blogico>maxSegundoNivel && blogico<=maxTercerNivel) {		/* nivel 3 */
		printf("tercer");	
		
		bloqueFisico = buscar_bloque_fisico(&i.i_indirect[3],blogico-(maxSegundoNivel +1),3,reserva,&reservado);

	}
	
	if(bloqueFisico != -1)
		escribir_inodo(inodo, i);	/* ERROR */
			
	*bfisico = bloqueFisico;		/* si no encuentra el bloqueFisico acaba mandando -1 */
	return reservado;
}


int traducir_bloque_inodo(int inodo, int bloque, char reservar){
	int NUM_PUNTEROS = (BLOQSIZE/4);
	struct superblock sb;
	get_sb(&sb);

	//Número máximo de punteros (bloques lógicos) como máximo que hay en nuestro sistema de ficheros
	int maximo_punteros = N_DIRECTS_INODES + NUM_PUNTEROS + (NUM_PUNTEROS*NUM_PUNTEROS) + (NUM_PUNTEROS*NUM_PUNTEROS*NUM_PUNTEROS);
	int bloque_logico = 0;

	int buffer_punteros[NUM_PUNTEROS], buffer_punteros2[NUM_PUNTEROS];

	//Leemos el inodo a tratar
	struct inode in;
	in = leer_inodo(inodo);

	//Comprobamos sea menor que 16843015 (0..16843014)
	if(bloque < maximo_punteros){

		//CASO CONSULTA
		if(reservar == '0'){

			//Si el bloque lógico está entre 0 y 7 (Punteros Directos) 
			if(bloque >= 0 && bloque<N_DIRECTS_INODES){
				return in.i_direct[bloque]; //Devolvemos "directamente" el bloque fisico
			}

			//Si el bloque lógico esta entre 8 y (8+256)-1 --> 8..263 (Puntero Indirecto 0)
			else if((bloque >=N_DIRECTS_INODES) && (bloque < (N_DIRECTS_INODES + NUM_PUNTEROS))){
				if(in.i_indirect[0] > 0){
					bread(in.i_indirect[0], buffer_punteros);
					bloque_logico = bloque - N_DIRECTS_INODES;
					printf("Puntero 1 (%d) está en el Bloque:  %d \n", bloque_logico, in.i_indirect[0]);	///////
					return buffer_punteros[bloque_logico]; //Devolvemos el bloque fisico
				}

				//Si no, devolvemos error
				return -1;
			}

			//Si el bloque lógico está entre (8+256) y (8+256+(256*256))-1 --> 264..65799 (Puntero Indirecto 1)
			else if((bloque >= N_DIRECTS_INODES + NUM_PUNTEROS) && (bloque < N_DIRECTS_INODES + NUM_PUNTEROS + 					(NUM_PUNTEROS*NUM_PUNTEROS))){
				
				if(in.i_indirect[1] > 0){
					bread(in.i_indirect[1], buffer_punteros);
					bloque_logico = bloque - (N_DIRECTS_INODES + NUM_PUNTEROS);					
			
					//printf("Puntero 1 (%d) está en el Bloque: %d \n", bloque_logico/NUM_PUNTEROS, in.i_indirect[1]);	///////

					if(buffer_punteros[bloque_logico/NUM_PUNTEROS] > 0){
						bread(buffer_punteros[bloque_logico/NUM_PUNTEROS], buffer_punteros2);

						//printf("Puntero 2 (%d) está en el Bloque: %d \n", bloque_logico%NUM_PUNTEROS, buffer_punteros[bloque_logico/NUM_PUNTEROS]);	///////
						return buffer_punteros2[bloque_logico % NUM_PUNTEROS]; //Devolvemos el bloque fisico
					}
					return -1;
				}
				return -1;
			}		

			//Si bloque lógico está entre (8+256+(256*256)) y (8+256+(256*256)+(256*256*256))-1 --> 65800..16843015 (P.Indirecto 2)
			else if((bloque >= N_DIRECTS_INODES + NUM_PUNTEROS + (NUM_PUNTEROS * NUM_PUNTEROS)) && (bloque < N_DIRECTS_INODES + 					NUM_PUNTEROS + (NUM_PUNTEROS*NUM_PUNTEROS) + (NUM_PUNTEROS*NUM_PUNTEROS*NUM_PUNTEROS))){
				
				if(in.i_indirect[2] > 0){
					bread(in.i_indirect[2], buffer_punteros);
					bloque_logico = bloque - (N_DIRECTS_INODES + NUM_PUNTEROS + (NUM_PUNTEROS*NUM_PUNTEROS));
					//printf("Puntero 1 (%d) está en el Bloque: %d \n", bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS), in.i_indirect[2]);	///////

					if((buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)]) > 0){
						//int print = (buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)]);///////////////
						bread(buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)], buffer_punteros);
						bloque_logico = bloque_logico % (NUM_PUNTEROS*NUM_PUNTEROS);
						//printf("Puntero 2 (%d) está en el Bloque %d \n", bloque_logico/NUM_PUNTEROS, print);/////////////
						//print = buffer_punteros[bloque_logico / NUM_PUNTEROS]; /////

						if(buffer_punteros[bloque_logico/NUM_PUNTEROS] > 0 ){
							bread(buffer_punteros[bloque_logico/NUM_PUNTEROS], buffer_punteros); //2
							//printf("Puntero 3 (%d) está en el Bloque %d \n", bloque_logico%NUM_PUNTEROS, print);/////////////
							return buffer_punteros[bloque_logico % NUM_PUNTEROS]; //Devolvemos bloque fisico 2
						}
						return -1; 
					}
					return -1; 
				}
				return -1; 
			}
			return -1;

	printf("consulta");
		}
		//CASO ESCRITURA (Reservar)
		else if(reservar == '1'){

			int bloque_reservado = 0;

			//Si el bloque lógico está entre 0 y 7 (Punteros Directos) 
			if(bloque >= 0 && bloque<N_DIRECTS_INODES){
				//Existe bloque fisico
				if(in.i_direct[bloque] > 0){
					return in.i_direct[bloque];
				}
				//NO existe bloque fisico
				else if(in.i_direct[bloque] == 0){
					in.i_direct[bloque] = reservar_bloque();
					in.i_blocks = in.i_blocks + 1;
					escribir_inodo(inodo, in);	
					return in.i_direct[bloque];
				}
			}

			//Si el bloque logico está entre 8 y (8+256)-1 --> 8..263 (1º Puntero Indirecto)
			else if((bloque >=N_DIRECTS_INODES) && (bloque < (N_DIRECTS_INODES + NUM_PUNTEROS))){

				if(in.i_indirect[0] > 0){

					bread(in.i_indirect[0], buffer_punteros);
					bloque_logico = bloque - N_DIRECTS_INODES;

					//printf("Puntero 1 (%d) está en el Bloque: %d \n", bloque_logico, in.i_indirect[0]);	///////

					if(buffer_punteros[bloque_logico] == 0){
						bloque_reservado = reservar_bloque();
						buffer_punteros[bloque_logico] = bloque_reservado;
						bwrite(in.i_indirect[0], buffer_punteros);

						in.i_blocks = in.i_blocks + 1;
						escribir_inodo(inodo, in);
					}

					return buffer_punteros[bloque_logico]; //Devolvemos bloque fisico

				}
				else if(in.i_indirect[0] == 0){				

					bloque_reservado = reservar_bloque();
					in.i_indirect[0] = bloque_reservado; 

					bread(in.i_indirect[0], buffer_punteros);
					memset(buffer_punteros, 0, BLOQSIZE);

					bloque_logico = bloque - N_DIRECTS_INODES; 

					//printf("Puntero 1 (%d) está en el Bloque:  %d \n", bloque_logico, in.i_indirect[0]);	///////

					bloque_reservado = reservar_bloque(); 
					buffer_punteros[bloque_logico] = bloque_reservado; 

					in.i_blocks = in.i_blocks + 1;
					bwrite(in.i_indirect[0], buffer_punteros);
					escribir_inodo(inodo, in);

					return buffer_punteros[bloque_logico]; //Devolvemos bloque fisico
				}
			}

			//Si el bloque lógico está entre (8+256) y (8+256+(256*256))-1 --> 264..65799 (Puntero Indirecto 1)
			else if((bloque >= N_DIRECTS_INODES + NUM_PUNTEROS + 1) && (bloque < N_DIRECTS_INODES + NUM_PUNTEROS + 					(NUM_PUNTEROS*NUM_PUNTEROS))){

				if(in.i_indirect[1] > 0){

					bread(in.i_indirect[1], buffer_punteros);
					bloque_logico = bloque - (N_DIRECTS_INODES + NUM_PUNTEROS);					

					if(buffer_punteros[bloque_logico/NUM_PUNTEROS] > 0){

						//printf("Puntero 1 (%d) está en el Bloque:  %d \n", bloque_logico/NUM_PUNTEROS, in.i_indirect[1]);	///////			
						bread(buffer_punteros[bloque_logico/NUM_PUNTEROS], buffer_punteros2);

						//printf("Puntero 2 (%d) está en el Bloque:  %d \n", bloque_logico%NUM_PUNTEROS, buffer_punteros[bloque_logico/NUM_PUNTEROS]);	///////

						if(buffer_punteros2[bloque_logico % NUM_PUNTEROS] == 0){
							bloque_reservado = reservar_bloque();
							buffer_punteros2[bloque_logico % NUM_PUNTEROS] = bloque_reservado;
							bwrite(buffer_punteros[bloque_logico/NUM_PUNTEROS], buffer_punteros2);

							in.i_blocks = in.i_blocks + 1;
							escribir_inodo(inodo, in);
						}

						return buffer_punteros2[bloque_logico % NUM_PUNTEROS]; //Devolvemos bloque fisico
					}
					else{ 

						bloque_reservado = reservar_bloque();
						buffer_punteros[bloque_logico/NUM_PUNTEROS] = bloque_reservado;

						bwrite(in.i_indirect[1], buffer_punteros);

						//printf("Puntero 1 (%d) está en el Bloque:  %d \n", bloque_logico/NUM_PUNTEROS, in.i_indirect[1]);	///////////			

						bread(buffer_punteros[bloque_logico/NUM_PUNTEROS], buffer_punteros2);

						//printf("Puntero 2 (%d) está en el Bloque:  %d \n", bloque_logico%NUM_PUNTEROS, buffer_punteros[bloque_logico/NUM_PUNTEROS]);	//////////

						bloque_reservado = reservar_bloque();
						buffer_punteros2[bloque_logico % NUM_PUNTEROS] = bloque_reservado;

						in.i_blocks = in.i_blocks + 1;
						bwrite(buffer_punteros[bloque_logico/NUM_PUNTEROS], buffer_punteros2);

						escribir_inodo(inodo, in);

						return buffer_punteros2[bloque_logico % NUM_PUNTEROS]; //Devolvemos bloque fisico

					}		

				}
				else if(in.i_indirect[1] == 0){

					bloque_reservado = reservar_bloque();
					in.i_indirect[1] = bloque_reservado;

					bread(in.i_indirect[1], buffer_punteros);
					bloque_logico = bloque - (N_DIRECTS_INODES + NUM_PUNTEROS);	

					bloque_reservado = reservar_bloque();				
					buffer_punteros[bloque_logico/NUM_PUNTEROS] = bloque_reservado;

					bwrite(in.i_indirect[1], buffer_punteros);

					//printf("Puntero 1 (%d) está en el Bloque:  %d \n", bloque_logico/NUM_PUNTEROS, in.i_indirect[1]);	///////////			

					bread(buffer_punteros[bloque_logico/NUM_PUNTEROS], buffer_punteros2);

					//printf("Puntero 2 (%d) está en el Bloque:  %d \n", bloque_logico%NUM_PUNTEROS, buffer_punteros[bloque_logico/NUM_PUNTEROS]);	//////////

					bloque_reservado = reservar_bloque();
					buffer_punteros2[bloque_logico % NUM_PUNTEROS] = bloque_reservado;
					in.i_blocks = in.i_blocks + 1;

					bwrite(buffer_punteros[bloque_logico/NUM_PUNTEROS], buffer_punteros2);

					escribir_inodo(inodo, in);

					return buffer_punteros2[bloque_logico % NUM_PUNTEROS]; //Devolvemos bloque fisico

				}
			}
			//Si bloque lógico está entre (8+256+(256*256)) y (8+256+(256*256)+(256*256*256))-1 --> 65800..16843015 (P.Indirecto 2)
			else if((bloque >= N_DIRECTS_INODES + NUM_PUNTEROS + (NUM_PUNTEROS * NUM_PUNTEROS)) && (bloque < N_DIRECTS_INODES + 					NUM_PUNTEROS + (NUM_PUNTEROS*NUM_PUNTEROS) + (NUM_PUNTEROS*NUM_PUNTEROS*NUM_PUNTEROS))){

				if(in.i_indirect[2] > 0){

					bread(in.i_indirect[2], buffer_punteros);
					bloque_logico = bloque - (N_DIRECTS_INODES + NUM_PUNTEROS + (NUM_PUNTEROS*NUM_PUNTEROS));

					if((buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)]) > 0){

						//printf("Puntero 1 (%d) está en el Bloque:  %d \n", bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS), in.i_indirect[2]);	///////
						//int print = (buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)]);///////////////

						bread(buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)], buffer_punteros2);
						int bloque_aux = bloque_logico % (NUM_PUNTEROS*NUM_PUNTEROS);

						//print = buffer_punteros2[bloque_aux/NUM_PUNTEROS];

						if(buffer_punteros2[bloque_aux/NUM_PUNTEROS] > 0){

							//printf("Puntero 2 (%d) está en el Bloque:  %d \n", (bloque_aux/NUM_PUNTEROS), buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)]);	///////

							bread(buffer_punteros2[bloque_aux/NUM_PUNTEROS], buffer_punteros);

							//printf("Puntero 3 (%d) está en el Bloque %d \n", bloque_aux%NUM_PUNTEROS, print);

							if(buffer_punteros[bloque_logico % NUM_PUNTEROS] == 0){
								bloque_reservado = reservar_bloque();	
								buffer_punteros[bloque_logico % NUM_PUNTEROS] = bloque_reservado;
								bwrite(buffer_punteros2[bloque_aux/NUM_PUNTEROS], buffer_punteros);

								in.i_blocks = in.i_blocks + 1;
								escribir_inodo(inodo, in);
							}

							return buffer_punteros[bloque_logico % NUM_PUNTEROS]; 
						}
						else{ 

							bloque_reservado = reservar_bloque();
							buffer_punteros2[bloque_aux/NUM_PUNTEROS] = bloque_reservado;

							//printf("Puntero 2 (%d) está en el Bloque:  %d \n", (bloque_aux/NUM_PUNTEROS), buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)]);	///////

							bwrite(buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)], buffer_punteros2);

							bread(buffer_punteros2[bloque_aux/NUM_PUNTEROS], buffer_punteros);

							//printf("Puntero 3 (%d) está en el Bloque:  %d \n", (bloque_aux%NUM_PUNTEROS), buffer_punteros2[bloque_aux/NUM_PUNTEROS]);	///////

							bloque_reservado = reservar_bloque();
							buffer_punteros[bloque_logico%NUM_PUNTEROS] = bloque_reservado;
							in.i_blocks = in.i_blocks + 1;

							bwrite(buffer_punteros2[bloque_aux/NUM_PUNTEROS], buffer_punteros);
							escribir_inodo(inodo,in);

							return (buffer_punteros[bloque_logico % NUM_PUNTEROS]);	//Devolvemos bloque fisico	

						}					


					}
					else{ 
						//printf("Puntero 1 (%d) está en el Bloque:  %d \n", bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS), in.i_indirect[2]);	///////

						bloque_reservado = reservar_bloque();
						buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)] = bloque_reservado;

						bwrite(in.i_indirect[2], buffer_punteros);

						bread(buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)], buffer_punteros2);

						int bloque_aux = bloque_logico % (NUM_PUNTEROS*NUM_PUNTEROS);

						//printf("Puntero 2 (%d) está en el Bloque:  %d \n", (bloque_aux/NUM_PUNTEROS), buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)]);	///////

						bloque_reservado = reservar_bloque();
						buffer_punteros2[bloque_aux/NUM_PUNTEROS] = bloque_reservado;

						bwrite(buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)], buffer_punteros2);

						bread(buffer_punteros2[bloque_aux/NUM_PUNTEROS], buffer_punteros);

						//printf("Puntero 3 (%d) está en el Bloque:  %d \n", (bloque_aux%NUM_PUNTEROS), buffer_punteros2[bloque_aux/NUM_PUNTEROS]);	///////

						bloque_reservado = reservar_bloque();
						buffer_punteros[bloque_logico % NUM_PUNTEROS] = bloque_reservado;
						in.i_blocks = in.i_blocks + 1;

						bwrite(buffer_punteros2[bloque_aux/NUM_PUNTEROS], buffer_punteros);

						escribir_inodo(inodo,in);

						return (buffer_punteros[bloque_logico % NUM_PUNTEROS]);	//Devolvemos bloque fisico	

					}

				}
				else if(in.i_indirect[2] == 0){

					bloque_reservado = reservar_bloque();
					in.i_indirect[2] = bloque_reservado;

					bread(in.i_indirect[2], buffer_punteros);
					bloque_logico = bloque - (N_DIRECTS_INODES + NUM_PUNTEROS + (NUM_PUNTEROS*NUM_PUNTEROS));

					//printf("Puntero 1 (%d) está en el Bloque:  %d \n", bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS), in.i_indirect[2]);	///////

					bloque_reservado = reservar_bloque();
					buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)] = bloque_reservado;

					bwrite(in.i_indirect[2], buffer_punteros);

					bread(buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)], buffer_punteros2);

					int bloque_aux = bloque_logico % (NUM_PUNTEROS*NUM_PUNTEROS);

					//printf("Puntero 2 (%d) está en el Bloque:  %d \n", (bloque_aux/NUM_PUNTEROS), buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)]);	///////

					bloque_reservado = reservar_bloque();
					buffer_punteros2[bloque_aux/NUM_PUNTEROS] = bloque_reservado;

					bwrite(buffer_punteros[bloque_logico/(NUM_PUNTEROS*NUM_PUNTEROS)], buffer_punteros2);

					bread(buffer_punteros2[bloque_aux/NUM_PUNTEROS], buffer_punteros);

					//printf("Puntero 3 (%d) está en el Bloque:  %d \n", (bloque_aux%NUM_PUNTEROS), buffer_punteros2[bloque_aux/NUM_PUNTEROS]);	///////

					bloque_reservado = reservar_bloque();
					buffer_punteros[bloque_logico % NUM_PUNTEROS] = bloque_reservado;
					in.i_blocks = in.i_blocks + 1;

					bwrite(buffer_punteros2[bloque_aux/NUM_PUNTEROS], buffer_punteros);

					escribir_inodo(inodo,in);


					return (buffer_punteros[bloque_logico % NUM_PUNTEROS]);	//Devolvemos bloque fisico	

				}
			}

		}
		else{
			printf("ERROR!!! traducir_bloque_inodo: Valor Reservar Erroneo!! \n");				

			return -1;
		}
	}
	else{
		printf("ERROR!!! traducir_bloque_inodo: Numero de bloque LOGICO no valido!! \n");				

		return -1;
	}

}

