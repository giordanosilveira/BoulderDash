LDFLAGS = `pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 --libs --cflags`

all: main.o lib_init_dest.o lib_geral.o lib_graficos.o lib_move.o libDefines.h
	gcc -Wall -g main.o lib_init_dest.o lib_geral.o lib_graficos.o lib_move.o libDefines.h -o main $(LDFLAGS)

main.o: main.c
	gcc -Wall -g -c main.c $(LDFLAGS)

lib_init_dest.o: lib_init_dest.c lib_init_dest.h libDefines.h
	gcc -Wall -g -c lib_init_dest.c $(LDFLAGS)

lib_graficos.o: lib_graficos.c lib_graficos.h libDefines.h
	gcc -Wall -g -c lib_graficos.c $(LDFLAGS)

lib_move.o: lib_move.c lib_move.h libDefines.h
	gcc -Wall -g -c lib_move.c $(LDFLAGS)

lib_geral.o: lib_geral.c lib_geral.h lib_graficos.c lib_graficos.h lib_move.c lib_move.h libDefines.h
	gcc -Wall -g -c lib_geral.c

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f main


