sources = $(wildcard *.c)
objects = $(addsuffix .o, $(basename $(sources)))
flags = -g -W -Wall -std=c11  -DDEBUG=0 -DTEST=0
LDFLAGS = -pthread
target = example_server


$(target) : $(objects)
	gcc -o $(LDFLAGS) $(target) $(objects) 


clean :
	rm $(target) $(objects)
