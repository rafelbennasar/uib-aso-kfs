all:
	gcc -c bloques.c 
	gcc -c ficheros_basico.c  
	gcc -c ficheros.c  
	gcc -c directorio.c  
	gcc -c mi_ls.c  
	gcc -c mi_rm.c  
	gcc -c mi_cp.c  
	gcc -c mi_mkdir.c  
	gcc -c mi_cat.c  
	gcc -c mi_sb.c
	gcc p1.c bloques.o ficheros_basico.o -o p1  
	gcc p2.c bloques.o ficheros_basico.o ficheros.o -o p2  
	gcc p3.c bloques.o ficheros_basico.o ficheros.o -o p3  
	gcc p4.c bloques.o ficheros_basico.o ficheros.o -o p4  
	gcc p5.c bloques.o ficheros_basico.o ficheros.o -o p5  
	gcc p6.c bloques.o ficheros_basico.o ficheros.o -o p6  
	gcc p7.c bloques.o ficheros_basico.o ficheros.o -o p7  
	gcc p8.c bloques.o ficheros_basico.o ficheros.o -o p8  
	gcc p9.c bloques.o ficheros_basico.o ficheros.o -o p9  
	gcc p10.c bloques.o ficheros_basico.o ficheros.o -o p10  
	gcc p11.c bloques.o ficheros_basico.o ficheros.o -o p11  
	gcc p13.c bloques.o ficheros_basico.o ficheros.o -o p13  
	gcc mi_mkfs.c bloques.o ficheros_basico.o -o mi_mkfs -lm -g 
	gcc mi_ls.c bloques.o ficheros_basico.o ficheros.o directorio.o  -o mi_ls -lm -g 
	gcc mi_escriu.c bloques.o ficheros_basico.o ficheros.o directorio.o  -o mi_escriu -lm -g 
	gcc mi_rm.c bloques.o ficheros_basico.o ficheros.o directorio.o  -o mi_rm -lm -g 
	gcc mi_du.c bloques.o ficheros_basico.o ficheros.o directorio.o  -o mi_du -lm -g 
	gcc mi_stat.c bloques.o ficheros_basico.o ficheros.o directorio.o  -o mi_stat -lm -g 
	gcc mi_mkdir.c bloques.o ficheros_basico.o ficheros.o directorio.o  -o mi_mkdir -lm -g 
	gcc mi_cat.c bloques.o ficheros_basico.o ficheros.o directorio.o  -o mi_cat -lm -g 
	gcc mi_cp.c bloques.o ficheros_basico.o ficheros.o directorio.o  -o mi_cp -lm -g 
	gcc simulacio.c bloques.o ficheros_basico.o ficheros.o directorio.o  -o simulacio -lm -g 


clean:
	rm -f *.o

run:
	./mi_mkfs filesystem 1024

edit:
	vim *.c *.h -p
