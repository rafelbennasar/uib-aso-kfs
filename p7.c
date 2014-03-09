#include "directorio.c"
/*
 * Cream un arbre de directoris
 *
 */
int main() {
	int n = 0;
	char nom[32];

	bmount("prova");

	mi_creat("/bin");
	mi_creat("/dev");
	mi_creat("/etc");
	mi_creat("/home/usuari/");
	mi_creat("/home/rafel");
	mi_creat("/home/teresa");
	mi_creat("/lib");
	mi_creat("/media");
	mi_creat("/mnt");
	mi_creat("/opt");
	mi_creat("/root");
	mi_creat("/sys");
	mi_creat("/tmp");
	mi_creat("/usr");
	mi_creat("/var");

	while (n < 512) {
		sprintf(nom,"/home/usuari/directori-%i", n);
		mi_creat(nom);
		n++;
	}
		
	bumount("prova");

	return 1;
}
