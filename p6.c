#include "directorio.c"
void main() {
	char *buffer;
	bmount(DISK_NAME);
	int n = 0;
	printf("Tamany real: %i \n", sizeof("aixo es una prova\n"));
//	while (n < 512) {
	char entrada[8192] = " Port 0 is an open-drain, bidirectional I/O port. Port 0 pins that have 1s written to them float and can be used as high impedance inputs. Port 0 pins must be polarized to Vcc or Vss in order to prevent any parasitic current consumption.  Port 0 is also the multiplexed low-order address and data bus during access to external program and data memory. In this application, it uses strong internal pull-up when emitting 1s. \n Port 1 is an 8-bit bidirectional I/O port with internal pull-ups. Port 1 pins that have 1s written to them are pulled high by the internal pull-ups and can be used as inputs. As inputs, Port 1 pins that are externally pulled low will source current because of the internal pull-ups. \n Port 2: Port 2 is an 8-bit bidirectional I/O port with internal pull-ups. Port 2 pins that have 1s written to them are pulled high by the internal pull-ups and can be used as inputs. As inputs, Port 2 pins that are externally pulled low will source current because of the internal pull-ups. Port 2 emits the high-order address byte during fetches from external program memory and during accesses to external data memory that use 16-bit addresses (MOVX @DPTR).In this application, it uses strong internal pull-ups emitting 1s. During accesses to external data memory that use 8-bit addresses (MOVX @Ri), port 2 emits the contents of the P2 SFR."; 
		mi_write("/prova", &entrada , 0, sizeof(entrada));
	char cadena[20] = "1 2 3 4 5 6 7 8 9\n";
		mi_write("/prova", &cadena , 0, sizeof(cadena));
/*	*cadena = "10 11 12 13 14 15 16";
		mi_write("/prova", &cadena , 19, sizeof(cadena));
	*cadena = "10 11 12 13 14 15 16";
		mi_write("/prova", &cadena , 38, sizeof(cadena));
*/	
//		n += 18;
//		}
	bumount(DISK_NAME);
	return 1;
}
