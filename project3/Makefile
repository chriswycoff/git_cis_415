sources = $(wildcard *.c)
objects = $(addsuffix .o, $(basename $(sources)))
flags = -g -W -Wall -std=c11 
LDFLAGS = -pthread
target = server

all:server

quacker.o : quacker.c
	gcc $(flags) -c quacker.c


server: quacker.o
	gcc $(LDFLAGS) quacker.o -o server

clean:
	rm -f *.o *~ server