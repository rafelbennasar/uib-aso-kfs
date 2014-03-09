#include "directorio.c"
void main() {
	char *primer = malloc(1024), *darrer = malloc(1024);
	int k = 0;

	printf("\n[Prova extraer_camino] de la cadena /primer/segon/tercer/quart \n");
	extraer_camino("/primer/segon/tercer/quart", primer, darrer);
	printf("Primera part: %s \ndarrera part: %s \n \n", primer, darrer);

	printf("\n[Prova extraer_final] de la cadena /primer/segon/tercer/quart \n");
	extraer_final("/primer/segon/tecer/quart", primer, darrer);
	printf("Primera part: %s \ndarrera part: %s \n \n", primer, darrer);


}
