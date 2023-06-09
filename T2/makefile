CFLAGS = -Wall -g

CC = gcc

	objects = separa.o

all: separa.o
	$(CC) $(CFLAGS) separa.o -o separa 

separa.o: separa.c 
	$(CC) $(CFLAGS) separa.c -c

clean:
	-rm -f $(objects)

purge: 
	-rm -f $(objects) separa