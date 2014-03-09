#include "directorio.c"

int list_entrades(int n_inode){
	struct superblock sb;
	get_sb(&sb);
	char buffer[BLOQSIZE];

	struct dentry entrada;
	struct inode i_inodo;
	int nou_inode = 0, e = 0, n = 0;

	i_inodo = leer_inodo(n_inode);
	n = 0;
//	while (n <= 8) {        
		memset(&buffer, 0, BLOQSIZE);
		//mi_read_f(i_inodo.i_direct[n], &buffer, n, BLOQSIZE);
		bread(n_inode, &buffer);
		printf("Bloc %i: [(%s)] \n", n_inode, buffer);
//		n++;
//	}
	printf("\n\n\n");
	return 1;
}
int main() {
	int s = 1;
	bmount("prova");
	while(s < 100) {
		list_entrades(s);
		s++;
	}

	bumount("prova");
return 1;
}
