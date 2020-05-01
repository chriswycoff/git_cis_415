#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
   

	while(1){
		write(STDOUT_FILENO, "Spinning\n",16);
		sleep(3);
	}




}
