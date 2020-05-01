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

	char* program = "./iobound";

	char* arguments[2] = {"./iobound", NULL};

	int num_processes = 5; 

	pid_t the_ids[num_processes];

	int signumber; 
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigset, NULL);


	for (int process_iterator = 0; process_iterator< num_processes; process_iterator++ ){

		the_ids[process_iterator] = fork();

		if (the_ids[process_iterator] == 0){

			if (sigwait(&sigset, &signumber) == 0){
				printf("Process %d wating now\n", getpid());

				// executing here
				if ( execvp(program, &arguments[2] ) < 0){
					printf("PROBLEM STARTIG PROCESS\n");
				}


			}


			else{
				printf("Error waiting the process: %d \n", getpid());
			}

		}


	}




}









