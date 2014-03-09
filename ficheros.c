#include "ficheros.h"
#define _GNU_SOURCE


int mi_read_f(unsigned int inodo, void *buf, unsigned int offset, unsigned int nbytes){
	char blk[BLOQSIZE];
	int bytes_count = 0;
	int bytes_jump = 0;
	int blkpos = offset/BLOQSIZE;
	int pos; 
	struct inode ino = leer_inodo(inodo);

	if(ino.i_size == 0){
		return 0;
	}

	if(offset%BLOQSIZE != 0){ 
		pos = traducir_bloque_inodo(blkpos, inodo, '0');
		if(pos <= 0){ 
			if (offset%BLOQSIZE + nbytes < BLOQSIZE){
				bytes_jump += offset%BLOQSIZE;
				bytes_count += offset%BLOQSIZE;
			} else{
				bytes_jump += BLOQSIZE-(offset%BLOQSIZE);
				bytes_count += BLOQSIZE-(offset%BLOQSIZE);
			}
		}else{ 
			bread(pos, &blk);

			if(offset%BLOQSIZE + nbytes < BLOQSIZE){
				memcpy(buf, blk+(offset%BLOQSIZE), nbytes);
				bytes_count += nbytes;
			}else{
				memcpy(buf, blk+(offset%BLOQSIZE), BLOQSIZE-(offset%BLOQSIZE));
				bytes_count += BLOQSIZE-(offset%BLOQSIZE);
			} 
			blkpos++;
		}
	}

	while(bytes_count < nbytes){ 
		pos = traducir_bloque_inodo(blkpos, inodo, '0');
		if(pos == 0){
			if(nbytes-bytes_count < BLOQSIZE){
				bytes_count += nbytes%BLOQSIZE;
				bytes_jump += nbytes%BLOQSIZE;
			}else{
				bytes_count += BLOQSIZE;
				bytes_jump += BLOQSIZE;
			}
		}else{ 
			bread(pos,&blk);
			if(nbytes-bytes_count < BLOQSIZE){
				memcpy(buf+bytes_count, blk,nbytes%BLOQSIZE); 
				bytes_count = nbytes;
			}else{
				memcpy(buf+bytes_count, blk,sizeof(blk)); 
				bytes_count += BLOQSIZE;
			}
			blkpos++;
		} 
	}
	return bytes_count-bytes_jump;
}


int mi_write_f(unsigned int inodo, const void *buf, unsigned int offset, unsigned int nbytes){ 
	char blk[BLOQSIZE];
	int bytes_count = 0, nbloque = -1;
	int blkpos = offset/BLOQSIZE;
	struct inode ino; 

	if(offset%BLOQSIZE != 0){ 
		nbloque = traducir_bloque_inodo(blkpos,inodo, '1');
		bread(nbloque, &blk);

		if(offset%BLOQSIZE + nbytes < BLOQSIZE){
			memcpy(blk+(offset%BLOQSIZE), buf, nbytes);
			bytes_count += nbytes;
		}else{
			memcpy(blk+(offset%BLOQSIZE), buf, BLOQSIZE-(offset%BLOQSIZE));
			bytes_count += BLOQSIZE-(offset%BLOQSIZE);
		} 
		bwrite(nbloque, &blk);
		blkpos++;
	}
	while(bytes_count < nbytes){ 
		nbloque = traducir_bloque_inodo(blkpos,inodo, '1');
		if(nbytes-bytes_count < BLOQSIZE){
			bread(nbloque, &blk);
			memcpy(blk, buf+bytes_count, nbytes-bytes_count); 
			bytes_count += (nbytes-bytes_count);
		}else{
			memcpy(blk, buf+bytes_count, BLOQSIZE); 
			bytes_count += sizeof(blk);
		}
		bwrite(nbloque, &blk);
		blkpos++;
	}

	//printf("Escric a %i, amb offset: %i, la quantitat %i de bytes [%s] \n", nbloque, offset, nbytes, buf);
	ino = leer_inodo(inodo);
	ino.i_links = 0;
	ino.i_size += bytes_count;
	escribir_inodo(inodo, ino);
}



int mi_stat_f(unsigned int inodo,  struct i_stat *p_stat) {

	struct inode ai;
	ai = leer_inodo(inodo);
	p_stat->i_mode = ai.i_mode;
	p_stat->i_size = ai.i_size;
	p_stat->i_ctime = ai.i_ctime;
	p_stat->i_links = ai.i_links;
	p_stat->i_blocks = ai.i_blocks;
}

