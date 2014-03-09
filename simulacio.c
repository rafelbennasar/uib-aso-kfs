#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "directorio.h"
#include "conf.h"

static int acabados;
static int mutex_p;

void reaper(int s) {

	while(wait3(NULL,WNOHANG,NULL) > 0) {
		acabados++;
		}
}



int proceso (char *directori, int n) {
	char linea[MAXLONG];
	
	char nombreFichero[MAXLONG];
	struct i_stat estado;
	int i;
	int k = 0;


	time_t temps;
	struct tm t;

	printf("Comença client: %i", n);
	sprintf(nombreFichero, "%s/proceso-%i/prueba.dat", directori, n);
	sprintf(linea,"Inicio LOG proceso %i\n", getpid());
	mi_creat(nombreFichero);
	mi_write(nombreFichero, &linea, k, strlen(linea));
	k += strlen(linea);

	for(i = 0; i < 100; i++) {					
		temps = time(NULL);
		localtime_r(&temps, &t);
		sprintf(linea,"%d:%d:%d Linea numero %i del proceso %i \n", t.tm_hour, t.tm_min, t.tm_sec, i, getpid());
		printf("Escriuré: %s \n", linea);
		mi_write(nombreFichero, &linea, k, strlen(linea));
		k += strlen(linea);
		sleep(1000000);
	}

	
	sprintf(linea,"Fin LOG proceso %i\n", getpid());
	mi_write(nombreFichero, &linea, k, strlen(linea));
//	k += strlen(linea);
	printf("Client %i ha acabat! \n", n);
}

void cleanup() {
	int i;	
	char nombreFichero[MAXLONG];

	for(i=0; i <= PROCESOS; i++) {
		sprintf(nombreFichero,"/proceso-%i.dat",i);

		if (mi_unlink(nombreFichero) == 1)
			printf("Eliminado %s\n", nombreFichero);
	}
}

void mi_ls2() {
	char *buffer2;
	int t_size, i = 0;
	struct i_stat e;
	
	t_size = mi_dir("/", &buffer2);
	printf("Hi ha %i arxius. \n", t_size/64);
	
	if (t_size > 0) {
		for (i = 0; i < t_size; i += 64) {
			mi_stat(&buffer2[i], &e);
			printf("%s  [%i bytes]\t \t ", &buffer2[i], e.i_size);
			if (((i/64) % 4) == 3)
				printf("\n");
		}
	}
	printf("\n");

}

int main (int argc,char **argv) {

	time_t temps;
	struct tm t;

	temps = time(NULL);
	localtime_r(&temps, &t);

	int i = 0;
	char dir_actual[MAXLONG];
	acabados = 0;		

	bmount(DISK_NAME);
	signal(SIGCHLD,reaper);
//	printf("Llistam contingut abans de començar: \n");
//	mi_ls2();

//	printf("Esperant 2 segons abans de començar... \n");
//	sleep(2);

//	cleanup();
		
	sprintf(dir_actual, "simul_%d%d%d", t.tm_hour, t.tm_min, t.tm_sec);
	for (i = 0;i<PROCESOS; i++) {
//		if(fork() == 0) {
			proceso(dir_actual, i);
//			exit(0);
//		}
		sleep(1);		
	}	
//	while (acabados<PROCESOS)
//			pause();

	printf("Llistam contingut després d'haver acabat: \n");
	mi_ls2();
	
	bumount(DISK_NAME);
}
