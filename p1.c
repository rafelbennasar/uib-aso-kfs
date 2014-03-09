#include "mi_mkfs.h"

/* 
// Per a realitzar les proves de N nivell, substituir el tamany de bloc al conf.h,
// posant-hi:
//
// #define BLOQSIZE 128 
//
*/

int main(int argc, char argv[]) {
	long long bloc = 4000000;
	//long long bloc = 400000000;
	int n = 0;
	unsigned int bf;

	printf("Prova de la funció traducir bloque inodo\n");
	printf("Primer cas: traducció del bloc 4E5 més una sèrie de 512 blocs consecutius. \n");
	printf("Segon cas: traducció del bloc 4E25\n\n");
	bmount("prova");
	while (n < 10) {
		//printf("El resultat de traduir bloc %i es %i \n\n", n, traducir_bloque_inodoB(1, n, &bf, 1));
		printf("El resultat de traduir bloc %i es %i \n\n", n, traducir_bloque_inodo(5, n, '1'));
		n+=1;
	}
/*
	printf("Segona prova: \n");
	bloc = 4000000000000000000000000;
	n = 0;
	while (n++ < 5) 
		printf("El resultat de traduir (offset + %i) es %i \n", n, traducir_bloque_inodo(1, bloc + n, 1));
	
	bumount("prova");
*/
}
