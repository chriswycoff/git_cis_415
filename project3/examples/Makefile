sources = $(wildcard *.c)
objects = $(addsuffix .o, $(basename $(sources)))
flags = -g -W -Wall -std=c11 -pthread -DDEBUG=0 -DTEST=0
target = example_server


$(target) : $(objects)
	gcc -o $(target) $(objects)

%.o : %.c
	gcc -c $(flags) $< -o $@
clean :
	rm $(target) $(objects)
