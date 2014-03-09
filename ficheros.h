#include "conf.h"
//#include "bloques.h"
#include "ficheros_basico.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <fcntl.h>
#include <string.h>

struct i_stat {
	unsigned int  i_mode;        
        unsigned int  i_size;       
        time_t  i_ctime;     
        int  i_links;    
        unsigned int  i_blocks;  
};




int mi_write_f(unsigned int inodo, const void *buf, unsigned int offset, unsigned int nbytes);
int mi_read_f(unsigned int inodo, void *buf, unsigned int offset, unsigned int nbytes);
//int mi_truncar_f(unsigned int inodo,  unsigned int nbytes);
int mi_stat_f(unsigned int inodo, struct i_stat *p_stat);
