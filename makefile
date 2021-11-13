LDFLAGS = `pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 --libs --cflags`

all: main.o lib_micro.o lib_macro.o libDefines.h
	gcc -Wall -g main.o lib_micro.o lib_macro.o libDefines.h -o main $(LDFLAGS)

main.o: main.c 
	gcc -Wall -g -c main.c $(LDFLAGS)

lib_micro.o: lib_micro.c lib_micro.h libDefines.h
	gcc -Wall -g -c lib_micro.c $(LDFLAGS)

lib_macro.o: lib_macro.c lib_macro.h lib_micro.c lib_micro.h libDefines.h
	gcc -Wall -g -c lib_macro.c $(LDFLAGS)

clean:
	-rm -f *~ *.o
	
purge: clean
	-rm -f main
