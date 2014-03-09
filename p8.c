#include "directorio.c"

int list_entrades(int n_inode){
	struct superblock sb;
	get_sb(&sb);

	struct dentry entrada;
	struct inode i_inodo;
	int nou_inode = 0, e = 0, n = 0;

	i_inodo = leer_inodo(n_inode);
	n = 0;
	printf("==== inode %i ====\n", n_inode);
	while (n <= 512) {        
		memset(&entrada, 0, 64);
		mi_read_f(n_inode, &entrada, n, 64);
		printf("%i: [(%s)] (inode: %i) \n", n, entrada.nombre, entrada.inodo);
		n += 64;
	}
	printf("\n\n\n");
	return 1;
}
int main() {
	int s = 1;
	bmount("prova");
	while(s < 9) {
		list_entrades(s);
		s++;
	}

	bumount("prova");
return 1;
}
