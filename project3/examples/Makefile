sources = $(wildcard *.c)
objects = $(addsuffix .o, $(basename $(sources)))
flags =  
LDFLAGS = -pthread
target = example_server

all: example_server

pub-sub-circular.o : pub-sub-circular.c
	gcc $(flags) -c pub-sub-circular.c


example_server: pub-sub-circular.o
	gcc $(LDFLAGS) pub-sub-circular.o -o example_server

clean:
	rm -f *.o *~ example_server