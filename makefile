FLAGS = -std=c99 -Wall -D_DEFAULT_SOURCE
gps: gps.c utils.c utils.h
	gcc ${FLAGS} gps.c utils.c -o gps
