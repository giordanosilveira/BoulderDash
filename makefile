LDFLAGS = `pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags`

all: boulder.o libfisica.o lib_estados_jogo.o lib_inicializacao.o libgraficos.o libdefines.h
	gcc -Wall -g boulder.o libfisica.o lib_estados_jogo.o lib_inicializacao.o libgraficos.o libdefines.h -o main $(LDFLAGS)

boulder.o: boulder.c 
	gcc -Wall -g -c boulder.c $(LDFLAGS)

libfisica.o: libfisica.c libfisica.h
	gcc -Wall -g -c libfisica.c $(LDFLAGS)

lib_estados_jogo.o: lib_estados_jogo.c lib_estados_jogo.h libgraficos.c libgraficos.h libfisica.c libfisica.h
	gcc -Wall -g -c lib_estados_jogo.c $(LDFLAGS)

lib_inicializacao.o: lib_inicializacao.c lib_inicializacao.h
	gcc -Wall -g -c lib_inicializacao.c $(LDFLAGS)

libgraficos.o: libgraficos.c libgraficos.h
	gcc -Wall -g -c libgraficos.c $(LDFLAGS)

clean:
	-rm -f *~ *.o
	
purge: clean
	-rm -f main
