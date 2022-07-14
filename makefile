FLAGS = -std=c99 -Wall -D_DEFAULT_SOURCE -g
OBJ = gps.o utils.o
gps: ${OBJ}
	gcc -o gps ${FLAGS} ${OBJ} utils.h 

gps.o: gps.c utils.h
	gcc -c gps.c ${FLAGS}

utils.o: utils.c utils.h
	gcc -c utils.c ${FLAGS} 

clean:
	rm *.o

purge:
	rm *.o gps