#include "header.h"

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


void signal_handler(int sig, siginfo_t * siginfo, void *context){
	write(1, "SIGNAL RECIEVED\n",16);
}




int main(int argc, char *argv[]) {


	struct sigaction sigact;

	memset(&sigact, 0, sizeof(sigact));

	sigact.sa_sigaction = &signal_handler;

	sigact.sa_flags = SA_SIGINFO;

	sigaction(SIGUSR1, &sigact, NULL);

	for(int i = 0; i < 20; i++){
			kill(getpid(),SIGUSR1);
			sleep(2);
	}




}

