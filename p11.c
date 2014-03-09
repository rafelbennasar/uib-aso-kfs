#include "directorio.c"

int main() {
	char buffer[4096];
//	char entrada[256] = "prueba un poco más larga";

	char entrada[8192] = " Port 0 is an open-drain, bidirectional I/O port. Port 0 pins that have 1s written to them float and can be used as high impedance inputs. Port 0 pins must be polarized to Vcc or Vss in order to prevent any parasitic current consumption.  Port 0 is also the multiplexed low-order address and data bus during access to external program and data memory. In this application, it uses strong internal pull-up when emitting 1s. \n Port 1 is an 8-bit bidirectional I/O port with internal pull-ups. Port 1 pins that have 1s written to them are pulled high by the internal pull-ups and can be used as inputs. As inputs, Port 1 pins that are externally pulled low will source current because of the internal pull-ups. \n Port 2: Port 2 is an 8-bit bidirectional I/O port with internal pull-ups. Port 2 pins that have 1s written to them are pulled high by the internal pull-ups and can be used as inputs. As inputs, Port 2 pins that are externally pulled low will source current because of the internal pull-ups. Port 2 emits the high-order address byte during fetches from external program memory and during accesses to external data memory that use 16-bit addresses (MOVX @DPTR).In this application, it uses strong internal pull-ups emitting 1s. During accesses to external data memory that use 8-bit addresses (MOVX @Ri), port 2 emits the contents of the P2 SFR."; 
	bmount(DISK_NAME);
	mi_write("/home/rafel/prova.txt", &entrada, 0, 2048);
	mi_write("/home/rafel/prova1.txt", &entrada, 0, 2048);
	mi_write("/home/rafel/prova2.txt", &entrada, 0, 2048);
	mi_write("/home/rafel/prova3.txt", &entrada, 0, 2048);
	mi_write("/home/rafel/prova4.txt", &entrada, 0, 2048);
	mi_write("/home/rafel/prova5.txt", &entrada, 0, 2048);
	mi_write("/home/rafel/prova6.txt", &entrada, 0, 2048);
	mi_write("/home/rafel/prova7.txt", &entrada, 0, 2048);
	mi_write("/home/rafel/prova8.txt", &entrada, 0, 2048);
	mi_write("/home/rafel/prova9.txt", &entrada, 0, 2048);
	mi_write("/home/rafel/prova10.txt", &entrada, 0, 2048);
	mi_write("/home/rafel/prova11.txt", &entrada, 0, 2048);
	mi_read("/home/rafel/prova11.txt", &buffer, 0, 4096);
	printf("%s \n", buffer);

	bumount(DISK_NAME);
return 1;
}
