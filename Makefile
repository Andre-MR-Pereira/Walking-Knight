#  Compiler
CC = gcc

#  Compiler Flags
CFLAGS = -Wall -std=c99 -O3

#  Sources
SOURCES = main.c Pather.c Testing.c Graph.c Short.c    			Functions.h

#  Objects
OBJECTS = main.o Testing.o Pather.o Graph.o Short.o

tuktuk: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

main.o: Functions.h main.c
	$(CC) -O3 -c main.c

Testing.o: Functions.h Testing.c
	$(CC) -O3 -c Testing.c

Pather.o: Functions.h Pather.c
	$(CC) -O3 -c Pather.c

Graph.o: Functions.h Graph.c
	$(CC) -O3 -c Graph.c

Short.o: Functions.h Short.c
	$(CC) -O3 -c Short.c


