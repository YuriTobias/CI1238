CFLAGS = -Wall -g

CC = gcc

	objects = producao.o

all: producao.o
	$(CC) $(CFLAGS) producao.o -o producao 

producao.o: producao.c 
	$(CC) $(CFLAGS) producao.c -c

clean:
	-rm -f $(objects)

purge: 
	-rm -f $(objects) producao