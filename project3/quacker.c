// Project 3 "quacker"
// Christopher Wycoff

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <unistd.h>




int main(int argc, char *argv[]){

	if (argc < 2){
		printf("USAGE ./server <int>\n");
		exit(-1);
	}


	int first_arg = atoi(argv[1]);

	printf("HELLO WORLD My int is %d\n" ,first_arg);

	exit(0);
}