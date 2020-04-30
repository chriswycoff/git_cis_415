#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>


int main(int argc, char *argv[]) {


//////////////////////////////////////////////////////////////

int num_processes = 5; 

pid_t the_ids[num_processes];


for (int process_iterator = 0; process_iterator< num_processes; process_iterator++ ){

	the_ids[process_iterator] = fork();

}




}