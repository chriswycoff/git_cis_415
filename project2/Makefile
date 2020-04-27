all : part1.out part2.out part3.out part4.out part5.out

sources = $(wildcard *.c)
objects = $(addsuffix .o, $(basename $(sources)))

part1.out : part1.c
	gcc -o part1.out part1.c

part2.out : part2.c
	gcc -o part2.out part2.c

part3.out : part3.c
	gcc -o part3.out part3.c

part4.out : part4.c
	gcc -o part4.out part4.c

part5.out : part5.c
	gcc -o part5.out part5.c

clean :
	rm part1.out part2.out part3.out part4.out part5.out 
