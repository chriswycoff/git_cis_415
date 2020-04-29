sources = $(wildcard *.c)
objects = $(addsuffix .o, $(basename $(sources)))
flags = -g -W -Wall -std=c11 

all : part1.out part2.out part3.out part4.out part5.out

part1.out : part1.o
	gcc -o part1.out part1.o

part2.out : part2.o
	gcc -o part2.out part2.o

part3.out : part3.o
	gcc -o part3.out part3.o

part4.out : part4.o
	gcc -o part4.out part4.o

part5.out : part5.o
	gcc -o part5.out part5.o

%.o : %.c
	gcc -c $(flags) $< -o $@

clean :
	rm part1.out part2.out part3.out part4.out part5.out $(objects)
